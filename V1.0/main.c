#include <msp430.h> 
#include "LCD.h"
#include "ADC.h"
#include "SPI.h"
#include "IO.h"
#include "StepperMotor.h"
/**
 * main.c
 */

#define SHOWTIME 1000000

volatile double soilMoisturePercentage = 0;
volatile int alertCount = 0;


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	//initialize LCD
	lcd_init();

	//clear screen
	lcd_command(0x20);



	//init ADC
	ADCInit();


	//initialize and zero stepper motors
	motorInit();

	centerMotorOne();
///	__delay_cycles(MOTORDELAY);

	centerMotorTwo();

	zeroMotorOne();

	zeroMotorTwo();


	IOInit();

	__enable_interrupt();

	ReadSensors();
	dac_write(0x00);

	while (1){

	    lcd_command(0x01);
	    //read from sensors
	    ReadSensors();

	    lcd_char('T');
	    lcd_char('=');
	    outputDigits((int)farenheit);
	    //outputDigits(thermistorReading);
	    lcd_char('F');
	    lcd_char(' ');
	    lcd_char('H');
	    lcd_char('2');
	    lcd_char('O');
	    lcd_char('=');
	    soilMoisturePercentage = soilProbeReading - 447.00; //447 is the value of the sensor when completely dry, 860 when totally wet
	    soilMoisturePercentage = soilMoisturePercentage/ 420.00; //867 - 447 is 420 which is the total range of values

	    //reverse soil moisture percentage so that 0 is dry and 1 is wet
	    soilMoisturePercentage =((1.00 - soilMoisturePercentage));

	    outputDigits((int)(soilMoisturePercentage * 100.00));
	    lcd_char('%');

	    moveMotorOne(farenheit * 4.15);
	    moveMotorTwo(soilMoisturePercentage* 420);




	    if (P1IN & BIT3){
//	        lcd_char('3');

	        ConfigureSettings();
	        continue;
	    }




	    //check for alerts
       if ((soilMoisturePercentage * 100.00) < lowerSoilLimit){
           //turn on water LED
           P2OUT |= BIT7;
           alertCount++;
           //outputAlert();
       } else{
           P2OUT &= ~BIT7;
       }

       if ((int) farenheit > upperTempLimit){
           //turn on heat LED
           P2OUT |= BIT5;
           alertCount++;
       } else {
           P2OUT &= ~BIT5;

       }

       if ((int) farenheit < lowerTempLimit){
           P2OUT |= BIT6;
           alertCount++;
       } else {
           P2OUT &= ~BIT6;
       }

       if (alertCount > 1)
           outputAlert();
       else
           __delay_cycles(ALERTDELAY * 64);

       alertCount = 0;




	}

	//return 0;
}
