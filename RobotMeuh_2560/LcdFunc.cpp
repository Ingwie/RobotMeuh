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


#include "LcdFunc.h"

char localBuf[SERIAL_LCD_BUF_LENGHT]; // static buffer -> Faster

// glyphs

const char customChars[] PROGMEM = {
0x00,0x14,0x16,0x17,0x16,0x14,0x00,0x00, //playpause \b
0x1e,0x01,0x05,0x09,0x1e,0x08,0x04,0x00, //enter     \1
0x00,0x0e,0x15,0x17,0x11,0x0e,0x00,0x00, //clock     \2
0x0e,0x1f,0x11,0x11,0x11,0x11,0x1f,0x00, //battery   \3
0x00,0x00,0x0a,0x00,0x11,0x0e,0x00,0x00, //smilley   \4
0x04,0x0e,0x0e,0x0e,0x1f,0x00,0x04,0x00, //alarm     \5
0x04,0x0e,0x1f,0x1f,0x02,0x0a,0x04,0x00, //umbrella  \6
0x1f,0x11,0x11,0x11,0x11,0x11,0x1f,0x00, //perimeter \7
};

void lcdAction(lcdFunction action)
{
 RobotStatus.RequestAction = A_lcdFunction;
 memcpy(&localBuf[0], &RobotStatus, 1);
 localBuf[1] = action;
 localBuf[2] = SERIAL_LCD_EOL; // end of packet
 SerialLcdPrint(localBuf);
}

void lcdLocate(u8 row, u8 column)
{
 RobotStatus.RequestAction = A_locate;
 memcpy(&localBuf[0], &RobotStatus, 1);
 localBuf[1] = ((row << 4) | (column & 0XF));
 localBuf[2] = SERIAL_LCD_EOL; // end of packet
 SerialLcdPrint(localBuf);
}

void lcdPrintchar(char code)
{
 RobotStatus.RequestAction = A_printChar;
 memcpy(&localBuf[0], &RobotStatus, 1);
 localBuf[1] = code;
 localBuf[2] = SERIAL_LCD_EOL; // end of packet
 SerialLcdPrint(localBuf);
}

void lcdPrintString(u8 row, u8 column, char *text)
{
 RobotStatus.RequestAction = A_printString;
 memcpy(&localBuf[0], &RobotStatus, 1);
 localBuf[1] = ((row << 4) | (column & 0XF));
 u8 len = strlcpy(&localBuf[2], text, SERIAL_LCD_BUF_LENGHT - 2);
 localBuf[len + 3] = SERIAL_LCD_EOL; // end of packet
 SerialLcdPrint(localBuf);
}

void lcdPrintString_P(u8 row, u8 column, const char *text)
{
 RobotStatus.RequestAction = A_printString;
 memcpy(&localBuf[0], &RobotStatus, 1);
 localBuf[1] = ((row << 4) | (column & 0XF));
 u8 len = strlcpy_P(&localBuf[2], text, SERIAL_LCD_BUF_LENGHT - 2);
 localBuf[len + 3] = SERIAL_LCD_EOL; // end of packet
 SerialLcdPrint(localBuf);
}

/* Full options ...
%c	character
%d	signed integers
%i	signed integers
%e	scientific notation, with a lowercase "e"
%E	scientific notation, with a uppercase "E"
%f	floating point
%g	use %e or %f, whichever is shorter
%G	use %E or %f, whichever is shorter
%o	octal
%s	a string of characters
%u	unsigned integer
%x	unsigned hexadecimal, with lowercase letters
%X	unsigned hexadecimal, with uppercase letters
*/

void lcdPrintf(u8 row, u8 column, PGM_P fmt, ...)
{
 va_list args;
 RobotStatus.RequestAction = A_printString;
 memcpy(&localBuf[0], &RobotStatus, 1);
 localBuf[1] = ((row << 4) | (column & 0XF));
 va_start(args, fmt);
 u8 len = vsprintf_P(&localBuf[2], fmt, args);
 va_end(args);
 localBuf[len + 3] = SERIAL_LCD_EOL; // end of packet
 SerialLcdPrint(localBuf);
}

void lcdLoadCgram(const char * tab, u8 charnum)
{
 sendLcdCmdLoadCg(); // send command
// Each character contains 8 definition values
 charnum *= 8;
 for (u8 index = 0; index < charnum; index++)
  {
   _delay_ms(10); // don't overflow serial buffers
   // Store values in LCD
   lcdPrintchar(pgm_read_byte_near(&tab[index]));
   SerialLcdSend();
  }
}
