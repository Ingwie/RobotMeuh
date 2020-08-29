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


/* Includes ------------------------------------------------------------------*/
#include "HD44780.h"

/**
  Initializes Character Generator CGRAM with custom characters data
* @param  Table containing characters definition values
* @param  Number of characters defined in the table

*/
void LCD_LOAD_CGRAM(char tab[], uint8_t charnum)
{
	uint8_t index;
	/* Each character contains 8 definition values*/
	charnum = charnum * 8;
	for (index = 0;index < charnum;index++)
	{
		/* Store values in LCD*/
		LCD_printchar(tab[index]);
		_delay_ms(35);
	}
}


/**
  Activate Power Pin that supplies LCD module
*/
void LCD_PWRON (void)
{
	//	GPIO_WriteHigh(LCDPwrPort, LCDPwrPin);
}

/**
  Disable Power Pin that supplies LCD module
*/
void LCD_PWROFF (void)
{
	//	GPIO_WriteLow(LCDPwrPort, LCDPwrPin);
}

/**
  Activate Enable Pin from LCD module
*/
void LCD_ENABLE (void)
{
	pin_high(LCDPinEnable);
	_delay_ms(35);
	pin_low(LCDPinEnable);
}

/**
  Command data sent to LCD module
*/
void LCD_CMD(unsigned char cmd_data)
{
	pin_low(LCDPinRS);
	LCDPin = ((LCDPin & 0x0F) | (cmd_data & 0xF0));
	LCD_ENABLE();
	LCDPin = ((LCDPin & 0x0F) | ((cmd_data << 4) & 0xF0));
	LCD_ENABLE();
	_delay_ms(35);
}


/**
  Initializes HD44780 LCD module in 4-bit mode
*/
void LCD_INIT(void)
{
	set_output_off(LCDPinD4);
	set_output_off(LCDPinD5);
	set_output_off(LCDPinD6);
	set_output_off(LCDPinD7);
	set_output_on(LCDPinEnable);
	set_output_off(LCDPinRS);
	//Initialization of HD44780-based LCD (4-bit HW)
	_delay_ms(45);
	LCD_CMD(0x33);
	_delay_ms(5);
	LCD_CMD(0x32);
	_delay_ms(170);
	//Function Set 4-bit mode
	LCD_CMD(0x28);
	//Display On/Off Control
	LCD_CMD(0x0C);
	//Entry mode set
	LCD_CMD(0x06);
	LCD_CLEAR_DISPLAY();
	//Minimum _delay_ms to wait before driving LCD module
	_delay_ms(4);
}

/**
  Print Character on LCD module
*/
void LCD_printchar(unsigned char ascode)
{
	pin_high(LCDPinRS);
	LCDPin = ((LCDPin & 0x0F) | (ascode & 0xF0));
	LCD_ENABLE();
	LCDPin = ((LCDPin & 0x0F) | ((ascode << 4) & 0xF0));
	LCD_ENABLE();
	_delay_ms(35);
}

/**
  Display of a characters string
*/
void LCD_printstring(unsigned char *text)
{
	do
	{
		LCD_printchar(*text++);
	}
	while (*text != '\n');
}

/**
  Clear LCD module display
*/
void LCD_CLEAR_DISPLAY(void)
{
	LCD_CMD(0x01);
	_delay_ms(3);
}

/**
  Set Cursor on second row 1st digit
*/
void LCD_2ndROW(void)
{
	LCD_CMD(0xC0);
	_delay_ms(37);
}

/**
  Set Cursor to Home position
*/
void LCD_HOME(void)
{
	LCD_CMD(0x02);
	_delay_ms(37);
}

/**
  Shift display to left
*/
void LCD_LSHIFT(void)
{
	LCD_CMD(0x18);
}

/**
  Shift display to right
*/
void LCD_RSHIFT(void)
{
	LCD_CMD(0x1C);
}

/**
  Set Display On
*/
void LCD_DISP_ON(void)
{
	LCD_CMD(0x0C);
}

/**
  Set Display Off
*/
void LCD_DISP_OFF(void)
{
	LCD_CMD(0x08);
}

/**
  Set Cursor to a specified location given by row and column information
* @param  Row Number (1 to 2)
* @param  Column Number (1 to 16) Assuming a 2 X 16 characters display
*/
void LCD_LOCATE(uint8_t row, uint8_t column)
{
	column--;
	switch (row)
	{
	case 1:
		/* Set cursor to 1st row address and add index*/
		LCD_CMD(column |= 0x80);
		break;
	case 2:
		/* Set cursor to 2nd row address and add index*/
		LCD_CMD(column |= 0x40 | 0x80);
		break;
	default:
		break;
	}
}
