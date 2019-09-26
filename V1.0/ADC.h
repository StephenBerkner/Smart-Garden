/*
 * ADC.h
 *
 *  Created on: Jul 21, 2019
 *      Author: sberk
 */

#ifndef ADC_H_
#define ADC_H_

#define SETTINGCONFIGDELAY 1500000

#include <math.h>

void ADCInit(void);

void ReadSensors(void);

void ConfigureSettings(void);

volatile double celsius;
volatile double farenheit;
volatile double kelvin;

volatile int soilProbeReading = 0;
volatile int thermistorReading = 0;

volatile const double inverseBeta = 1.00 / 3380.00;
volatile const double beta = 3380.00;
volatile const double nominalTemperature = 25.00;
volatile const double thermistorResistance = 10000.00;
volatile const double roomTempKelvin = 1.00 / 298.15;

volatile int ADCBuffer[5] = {0};

volatile int lowerSoilLimit = 0;
volatile int lowerTempLimit = 0;
volatile int upperTempLimit = 100;







#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR (void){

    /*
    if (ADCCount == 3){
        soilProbeReading = (int)ADC10MEM;
        ADCCount--;
    } else if (ADCCount == 2){
        thermistorReading = (int)ADC10MEM;
        ADCCount--;
    } else if (ADCCount == 0){
        ADCCount = 3;
    } else {
        ADCCount--;
    }
    */

    __bic_SR_register_on_exit(CPUOFF);

}

void ADCInit(void){

    ADC10CTL1 |= CONSEQ_1 | INCH_4; //conversion sequence select 1, read a sequence of channels ONCE

    ADC10CTL0 |=  SREF_0 | ADC10SHT_1 | ADC10ON | ADC10IE | MSC; // to use internal reference at 2.5V add + REF2_5V + REFON

    //perform 5 conversions
    ADC10DTC1 = 0x02;
    ADC10AE0 |= BIT4 | BIT3;//0x18;   //use A4 and A3
    P2DIR &= ~(BIT4 | BIT3);
    P2SEL |= BIT4 | BIT3;
//
//
//    ADC10CTL1 |= INCH_3 + CONSEQ1 + ADC10DIV_3; //conversion sequence select 1, channel 3, clock divider 3
//     ADC10CTL0 |= SREF_0 + ADC10SHT_1 + ADC10ON + MSC;  //SREF_0 set proper references, sample and hold time 1, turn the ADC on and continuously sample
//     ADC10AE0 |= BIT3; //using channel 3
//     P2DIR &= ~BIT3;
//     P2SEL|= BIT3;
//     ADC10CTL0 |= ADC10SC + ENC;    //begin ADC conversions


//    P2DIR &= ~(BIT4 | BIT3);
//    P2SEL |= BIT4 | BIT3;





}

//method to read sensors to control the rate at which they are read
void ReadSensors(void){
    //initialize buffer

    //read from ADC
    ADC10CTL0 &= ~ENC;

    //wait for ADC to be ready
    while (ADC10CTL1 & BUSY);
    ADC10SA = (unsigned int) ADCBuffer;

    ADC10CTL0 |= (ADC10SC | ENC);    //begin conversions
    while (ADC10CTL1 & BUSY);

    soilProbeReading = ADCBuffer[0];
    thermistorReading = ADCBuffer[1];

    double voltageOut = 3.3 * (thermistorReading / 1023.00) - 0.2;
    double thermistorResistance = 5500.011 / voltageOut - 1666.67;//33000.00 / voltageOut - 10000.00;
    ///double thermistorResistance = //1000 * ((3.3 * (10.00 / ((thermistorReading * 3.3)/1023.0))) - 10);
//
//    //temperature = ( 1 / ( 0.001129148 + ( 0.000234125 * therm_res_ln ) + ( 0.0000000876741 * therm_res_ln * therm_res_ln * therm_res_ln ) ) ); /* Temperature in Kelvin */
//
    kelvin = ( 1.00 / (0.001129148 + (0.000234125 * log(thermistorResistance)) + ( 0.0000000876741 * log(thermistorResistance) * log(thermistorResistance) * log(thermistorResistance))));

//    double resistance = 1023.00 / thermistorReading - 1.00;
//    resistance = 10000.00 / resistance; //measured resistance of reference resistor
//
//    kelvin = resistance / 10000.00;
//    kelvin = log(kelvin);
//    kelvin /= beta;
//    kelvin += 1.0 / (nominalTemperature + 273.15);
//    kelvin = 1.0 / kelvin;
//



//    kelvin = 1.00 / (roomTempKelvin + inverseBeta*(log10(1023.00 / (double) thermistorReading - 1.00)));
    celsius = kelvin - 273.15;
    farenheit = ((9.0 * celsius)/ 5.00) + 32.00;

//    __bis_SR_register(CPUOFF + GIE);


}


//get settings for alerts
void ConfigureSettings(void){

    int setting = 80;
    //clear lcd
    lcd_command(0x01);
//
//    lcd_char('C');
//    lcd_char('O');
//    lcd_char('N');
//    lcd_char('F');
//    lcd_char('I');
//    lcd_char('G');
//

    lcd_char('S');
    lcd_char('E');
    lcd_char('T');
    lcd_char(' ');
    lcd_char('A');
    lcd_char('L');
    lcd_char('E');
    lcd_char('R');
    lcd_char('T');
    lcd_char(' ');
    lcd_char('L');
    lcd_char('E');
    lcd_char('V');
    lcd_char('E');
    lcd_char('L');
    lcd_char('S');
    __delay_cycles(SETTINGCONFIGDELAY);

    lcd_command(0x01);

    //configure settings for temperature alerts

    //take in high limit first
    lcd_char('T');
    lcd_char('E');
    lcd_char('M');
    lcd_char('P');

    lcd_char(' ');

    lcd_char('H');
    lcd_char('I');
    lcd_char('G');
    lcd_char('H');
    lcd_char(' ');
    lcd_char('=');
    lcd_char(' ');


    while(!(P1IN & BIT0)){
        //put cursor to position 11 on line 1
        lcd_command(0x8C);

        outputDigits(setting);

        if ((P1IN & BIT1) && setting < 100){
            setting++;
            while (P1IN & BIT1);
        }

        if ((P1IN & BIT2) && setting > 0 && setting > lowerTempLimit){
            setting--;

            while(P1IN & BIT2);
        }


    }

    upperTempLimit = setting;


    //clear screen
    lcd_command(0x01);

    //reset setting
    setting = 60;


    //take in low limit first
    lcd_char('T');
    lcd_char('E');
    lcd_char('M');
    lcd_char('P');

    lcd_char(' ');

    lcd_char('L');
    lcd_char('O');
    lcd_char('W');
    lcd_char(' ');
    lcd_char('=');
    lcd_char(' ');


    while(!(P1IN & BIT0)){
        //put cursor to position 11 on line 1
        lcd_command(0x8B);

        outputDigits(setting);

        if ((P1IN & BIT1) && setting < 100 && setting < upperTempLimit){
            setting++;
            while (P1IN & BIT1);
        }

        if ((P1IN & BIT2) && setting > 0){
            setting--;

            while(P1IN & BIT2);
        }


    }

    lowerTempLimit = setting;

    //clear screen
    lcd_command(0x01);

    //reset setting
    setting = 30;

    //take in low limit  for H2O


    lcd_char('H');
    lcd_char('2');
    lcd_char('O');
    lcd_char(' ');

    lcd_char('L');
    lcd_char('O');
    lcd_char('W');
    lcd_char(' ');
    lcd_char('=');
    lcd_char(' ');


    while(!(P1IN & BIT0)){
        //put cursor to position 11 on line 1
        lcd_command(0x8A);

        outputDigits(setting);
        lcd_char('%');

        if ((P1IN & BIT1) && setting < 100){
            setting++;
            while (P1IN & BIT1);
        }

        if ((P1IN & BIT2) && setting > 0){
            setting--;

            while(P1IN & BIT2);
        }


    }

       lowerSoilLimit = setting;



    //configure settings for soil moisture alert
}


#endif /* ADC_H_ */
