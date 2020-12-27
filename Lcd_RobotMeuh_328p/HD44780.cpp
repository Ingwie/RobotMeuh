/*
.-------.        ,-----.     _______       ,-----.  ,---------. ,---.    ,---.    .-''-.    ___    _ .---.  .---.
|  _ _   \     .'  .-,  '.  \  ____  \   .'  .-,  '.\          \|    \  /    |  .'_ _   \ .'   |  | ||   |  |_ _|
| ( ' )  |    / ,-.|  \ _ \ | |    \ |  / ,-.|  \ _ \`--.  ,---'|  ,  \/  ,  | / ( ` )   '|   .'  | ||   |  ( ' )
|(_ o _) /   ;  \  '_ /  | :| |____/ / ;  \  '_ /  | :  |   \   |  |\_   /|  |. (_ o _)  |.'  '_  | ||   '-(_{;}_)
| (_,_).' __ |  _`,/ \ _/  ||   _ _ '. |  _`,/ \ _/  |  :_ _:   |  _( )_/ |  ||  (_,_)___|'   ( \.-.||      (_,_)
|  |\ \  |  |: (  '\_/ \   ;|  ( ' )  \: (  '\_/ \   ;  (_I_)   | (_ o _) |  |'  \   .---.' (`. _` /|| _ _--.   |
|  | \ `'   / \ `"/  \  ) / | (_{;}_) | \ `"/  \  ) /  (_(=)_)  |  (_,_)  |  | \  `-'    /| (_ (_) _)|( ' ) |   |
|  |  \    /   '. \_/``".'  |  (_,_)  /  '. \_/``".'    (_I_)   |  |      |  |  \       /  \ /  . \ /(_{;}_)|   |
''-'   `'-'      '-----'    /_______.'     '-----'      '---'   '--'      '--'   `'-..-'    ``-'`-'' '(_,_) '---'
*/
/*         Copyright 2020 by Ingwie (Bracame)          */
/*   Licence: GPLV3 see <http://www.gnu.org/licenses   */
/*        Compile with AVR GCC + Code::Blocks          */
/*    https://www.mediafire.com/file/cahqfrm90h7c7fy/  */
/*    Setup_OAVRCBuilder3.exe/file (Pswd : OpenAVRc)   */


#include "HD44780.h"

/**
  Initializes Character Generator CGRAM with custom characters data
* @param  Table containing characters definition values
* @param  Number of characters defined in the table

*/
void lcdLoadCgram(char tab[], u8 charnum)
{
 u8 index;
 /* Each character contains 8 definition values*/
 charnum = charnum * 8;
 for (index = 0; index < charnum; index++)
  {
   /* Store values in LCD*/
   lcdPrintchar(tab[index]);
   _delay_us(37);
  }
}


/**
  Activate Power Pin that supplies LCD module
*/
void lcdLedOn()
{
 pin_high(LCDPinLed);
}

/**
  Disable Power Pin that supplies LCD module
*/
void lcdLedOff()
{
 pin_low(LCDPinLed);
}

/**
  Activate Enable Pin from LCD module
*/
void lcdEnable()
{
 pin_high(LCDPinEnable);
 _delay_us(37);
 pin_low(LCDPinEnable);
}

/**
  Command data sent to LCD module
*/
void lcdCmd(char cmd_data)
{
 pin_low(LCDPinRS);
 u8 temp = (LCDPort & 0x0F);
 LCDPort = (temp | (cmd_data & 0xF0));
 lcdEnable();
 LCDPort = (temp | (cmd_data << 4));
 lcdEnable();
 _delay_us(37);
}


/**
  Initializes HD44780 LCD module in 4-bit mode
*/
void lcdInit()
{
//Init Pin
 set_output_off(LCDPinD4);
 set_output_off(LCDPinD5);
 set_output_off(LCDPinD6);
 set_output_off(LCDPinD7);
 set_output_off(LCDPinRS);
 set_output_on(LCDPinEnable);
 set_output_off(LCDPinLed);

//Initialization of HD44780-based LCD (4-bit HW)
 _delay_ms(45);
 lcdCmd(0x33);
 _delay_ms(6);
 lcdCmd(0x32);
 _delay_us(170);
//Function Set 4-bit mode
 lcdCmd(0x28);
//Display On/Off Control
 lcdCmd(0x0C);
//Entry mode set
 lcdCmd(0x06);
 lcdClear();
//Minimum delay to wait before driving LCD module
 _delay_ms(4);
}

/**
  Print Character on LCD module
*/
void lcdPrintchar(char ascode)
{
 pin_high(LCDPinRS);
 u8 temp = (LCDPort & 0x0F);
 LCDPort = (temp | (ascode & 0xF0));
 lcdEnable();
 LCDPort = (temp | (ascode << 4));
 lcdEnable();
}

/**
  Display of a characters string
*/
void lcdPrintstring(const char *text)
{
 do
  {
   lcdPrintchar(*text++);
  }
 while (*text != '\0');
}

/**
  Clear LCD module display
*/
void lcdClear()
{
 lcdCmd(0x01);
 _delay_us(1530);
}

/**
  Set Cursor on second row 1st digit
*/
void lcd2ndRow()
{
 lcdCmd(0xC0);
 _delay_us(37);
}

/**
  Set Cursor to Home position
*/
void lcdHome()
{
 lcdCmd(0x02);
 _delay_us(1530);
}

/**
  Shift display to left
*/
void lcdLShift()
{
 lcdCmd(0x18);
}

/**
  Shift display to right
*/
void lcdRShift()
{
 lcdCmd(0x1C);
}

/**
  Set Display On
*/
void lcdDispOn()
{
 lcdCmd(0x0C);
}

/**
  Set Display Off
*/
void lcdDispOff()
{
 lcdCmd(0x08);
}

/**
  Set Blink Block On
*/
void lcdBlinkOn()
{
 lcdCmd(0x0F);
}

/**
  Set Set Blink Block Off
*/
void lcdBlinkOff()
{
 lcdCmd(0x0C);
}

/**
  Set Cursor to a specified location given by row and column information
* @param  Row Number (0 to 1)
* @param  Column Number (0 to 15) Assuming a 2 X 16 characters display
*/
void lcdLocate(u8 row, u8 column)
{
 switch (row)
  {
  case 0:
   /* Set cursor to 1st row address and add index*/
   lcdCmd(column |= 0x80);
   break;
  case 1:
   /* Set cursor to 2nd row address and add index*/
   lcdCmd(column |= 0x40 | 0x80);
   break;
  default:
   break;
  }
}

void lcd_printStringAt(u8 row, u8 column, const char *text)
{
	lcdLocate(row,column);
	lcdPrintstring(text);
}
