/*
 * StepperMotor.h
 *
 *  Created on: Aug 1, 2019
 *      Author: sberk
 */

#ifndef STEPPERMOTOR_H_
#define STEPPERMOTOR_H_


#include <stdlib.h>

#define MOTORDELAY 1100
volatile char counterClockwise[8] = { 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09 };
volatile char clockwise[8] = {0x09, 0x08, 0x0C, 0x04, 0x06, 0x02, 0x03, 0x01};

volatile int i;
volatile int frequencyIndex = 0;
unsigned long buffer[4];
volatile int index;
volatile int frequency;
volatile int oldFrequency;
volatile unsigned char check;
volatile unsigned char zeroHold;
volatile int toOutput;
volatile int avgADC = 0;
volatile int ADCaccumulator = 0;
volatile int avgs[20];
volatile int oldAvgADC = 0;
volatile int ADCcheck = 0;

volatile int motorOnePosition = -1;
volatile int motorTwoPosition = -1;

//initialize stepper motor ports
void motorInit(void){
    P3DIR = 0xFF;
    P3OUT = 0x00;
    P4DIR &= ~(BIT6 | BIT7);
}

//rotates motor until it breaks opto-gate beam
void centerMotorOne(void){

    int count = 0;
        while(!(P4IN & BIT7)) {
            if (count > 512)
                break;
            else
                count++;

            for (i = 0; i < 8; i++){
                P3OUT = (counterClockwise[i] << 4);
                __delay_cycles(MOTORDELAY);
            }
        }

}


//rotates motor until it breaks opto-gate beam
void centerMotorTwo(void) {
    int count = 0;

    while(!(P4IN & BIT6)) {
        if (count > 512)
            break;
        else
            count++;

        for (i = 0; i < 8; i++){
            P3OUT = (counterClockwise[i] );
            __delay_cycles(MOTORDELAY);
        }
    }

}

//brings motor to zero position on dial
void zeroMotorOne(void){
    int count = 40;
    while(count > 0) {

        for (i = 0; i < 8; i++){
            P3OUT = (clockwise[i] << 4);
            __delay_cycles(MOTORDELAY);
        }
        count--;
    }

    motorOnePosition = 0;
}

//brings motor to zero position on dial
void zeroMotorTwo(void){
    int count = 40;
    while(count > 0) {

        for (i = 0; i < 8; i++){
            P3OUT = (clockwise[i]);
            __delay_cycles(MOTORDELAY);
        }
        count--;
    }

    motorTwoPosition = 0;
}

//moves motor to desired step position on dial based on 512 steps in full rotation
void moveMotorOne(int position){
    if (position < 0 || position > 425)
        return;



    //get differences in position
    int difference = position > motorOnePosition ? position - motorOnePosition : motorOnePosition - position;

    //determine count based on 512 steps around a circle
    int count = difference > 256 ? 512 - difference: difference;

    //move to new position via shortest path
    if (position > motorOnePosition && difference <= 256){
      while(count > 0) {

                for (i = 0; i < 8; i++){
                    P3OUT = (clockwise[i] << 4);
                    __delay_cycles(MOTORDELAY);
                }
                count--;
            }

                motorOnePosition = position;

    } else if (position < motorOnePosition && difference <= 256){

        while(count > 0) {

            for (i = 0; i < 8; i++){
                P3OUT = (counterClockwise[i] << 4);
                __delay_cycles(MOTORDELAY);
            }
            count--;
        }

                motorOnePosition = position;

    } else if (difference > 256 && position > motorOnePosition){
        while(count > 0) {

        for (i = 0; i < 8; i++){
            P3OUT = (counterClockwise[i] << 4);
            __delay_cycles(MOTORDELAY);
        }
            count--;
        }

                motorOnePosition = position;

    } else if (difference >  256 && position < motorOnePosition){
        while(count > 0) {

        for (i = 0; i < 8; i++){
            P3OUT = (clockwise[i] << 4);
            __delay_cycles(MOTORDELAY);
        }
            count--;
        }

                motorOnePosition = position;

    }



}

//moves motor to desired step position on dial based on 512 steps in full rotation
void moveMotorTwo(int position){

    if (position < 0 || position > 425)
            return;


    //get differences in position
    int difference = position > motorTwoPosition ? position - motorTwoPosition: motorTwoPosition - position;

    //determine count based on 512 steps around
    int count = difference > 256 ? 512 - difference: difference;

    //move to new position via shortest path
    if (position > motorTwoPosition && difference <= 256){
            while(count > 0) {

                for (i = 0; i < 8; i++){
                    P3OUT = (clockwise[i]);
                    __delay_cycles(MOTORDELAY);
                }
                count--;
            }

            motorTwoPosition = position;

    } else if (position < motorTwoPosition && difference <= 256){

        while(count > 0) {

        for (i = 0; i < 8; i++){
            P3OUT = (counterClockwise[i]);
            __delay_cycles(MOTORDELAY);
        }
            count--;
        }

        motorTwoPosition = position;

    } else if (difference > 256 && position > motorTwoPosition){

        while(count > 0) {

        for (i = 0; i < 8; i++){
            P3OUT = (counterClockwise[i]);
            __delay_cycles(MOTORDELAY);
        }
            count--;
        }

        motorTwoPosition = position;

    } else if (difference >  256 && position < motorTwoPosition){

        while(count > 0) {

        for (i = 0; i < 8; i++){
            P3OUT = (clockwise[i]);
            __delay_cycles(MOTORDELAY);
        }
            count--;
        }

        motorTwoPosition = position;

    }
}

#endif /* STEPPERMOTOR_H_ */
