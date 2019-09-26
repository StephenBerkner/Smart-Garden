/*
 * LCD.h
 *
 *  Created on: Jul 21, 2019
 *      Author: sberk
 */

#ifndef LCD_H_
#define LCD_H_

#define DELAY_TIME 30000
#define WRITE_TIME 5


void lcd_command(char);
void lcd_char(char);
void outputDigits(int val);



char uf_lcd_temp;
char uf_lcd_temp2;
char uf_lcd_x;

//function provided from module's code
void lcd_command(char uf_lcd_x){
    P4DIR = 0xFF;
    uf_lcd_temp = uf_lcd_x;
    P4OUT = 0x00;
    __delay_cycles(DELAY_TIME);
    uf_lcd_x = uf_lcd_x >> 4;
    uf_lcd_x = uf_lcd_x & 0x0F;
    uf_lcd_x = uf_lcd_x | 0x20;
    P4OUT =  uf_lcd_x;
    __delay_cycles(DELAY_TIME);
    uf_lcd_x = uf_lcd_x & 0x0F;
    P4OUT =  uf_lcd_x;
    __delay_cycles(DELAY_TIME);
    P4OUT = 0x00;
    __delay_cycles(DELAY_TIME);
    uf_lcd_x = uf_lcd_temp;
    uf_lcd_x = uf_lcd_x & 0x0F;
    uf_lcd_x = uf_lcd_x | 0x20;
    P4OUT = uf_lcd_x;
    __delay_cycles(DELAY_TIME);
    uf_lcd_x = uf_lcd_x & 0x0F;
    P4OUT = uf_lcd_x;
    __delay_cycles(DELAY_TIME);
}

//function provided from module's code
void lcd_init(void){
    lcd_command(0x33);
    __delay_cycles(5);
    lcd_command(0x32);
    __delay_cycles(1);
    lcd_command(0x2C);
    __delay_cycles(1);
    lcd_command(0x0C);
    __delay_cycles(1);
    lcd_command(0x01);
    __delay_cycles(5);

}

//function provided from module's code
//This was modified slightly to reduce the delay when writing to the screen
void lcd_char(char uf_lcd_x){
    P4DIR = 0xFF;
    uf_lcd_temp = uf_lcd_x;
    P4OUT = 0x10;
    __delay_cycles(WRITE_TIME);
    uf_lcd_x = uf_lcd_x >> 4;
    uf_lcd_x = uf_lcd_x & 0x0F;
    uf_lcd_x = uf_lcd_x | 0x30;
    P4OUT =  uf_lcd_x;
    __delay_cycles(WRITE_TIME);
    uf_lcd_x = uf_lcd_x & 0x1F;
    P4OUT =  uf_lcd_x;
    __delay_cycles(WRITE_TIME);
    P4OUT = 0x10;
    __delay_cycles(WRITE_TIME);
    uf_lcd_x = uf_lcd_temp;
    uf_lcd_x = uf_lcd_x & 0x0F;
    uf_lcd_x = uf_lcd_x | 0x30;
    P4OUT = uf_lcd_x;
    __delay_cycles(WRITE_TIME);
    uf_lcd_x = uf_lcd_x & 0x1F;
    P4OUT = uf_lcd_x;
    __delay_cycles(WRITE_TIME);
}



void outputDigits(int val){
    char toOutput[3];
    toOutput[2] = '0';
    toOutput[1] = '0';
    toOutput[0] = '0';
    if (val > 99){

        int c = 2;
        while (val){
            toOutput[c--] = (val % 10) + '0';
            val /= 10;
        }

        lcd_char(toOutput[0]);
        lcd_char(toOutput[1]);
        lcd_char(toOutput[2]);

    } else if (val > 9 && val < 100){
        int c = 1;
              while (val){
                  toOutput[c--] = (val % 10) + '0';
                  val /= 10;
              }

              lcd_char(toOutput[0]);
              lcd_char(toOutput[1]);
              //lcd_char(toOutput[2]);

    } else {
          lcd_char('0');
          lcd_char(val +'0');
    }

}





#endif /* LCD_H_ */
