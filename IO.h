/*
 * IO.h
 *
 *  Created on: Jul 29, 2019
 *      Author: sberk
 */

#ifndef IO_H_
#define IO_H_

void IOInit(void);




void IOInit(){

    P1DIR &= ~(BIT3 | BIT2 | BIT1 | BIT0);

    P2DIR |= BIT0 | BIT1 | BIT2 |BIT5 | BIT6 | BIT7;
    //clear PSEL to use XTAL pins
    P2SEL &= ~0xC7;
    P2SEL |= 0x18;

    P2OUT = 0x00;
}


#endif /* IO_H_ */
