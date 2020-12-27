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

u8 lcdSpiXferBuff()
{
// Full Duplex (4 wire) spi
 u8 ret;
 pin_low(SpiLcdSSPin);
 for (u8 i = 0; i <= SpiBufNum; i++)
  {
   SPDR = SpiBuf[i];
   /* Wait for transfer to complete */
   while (!(SPSR & (1<<SPIF)));
   if (!ret) ret = SPDR;
  }
 pin_high(SpiLcdSSPin);
 return ret;
}

void updateLcdReport(u8 byte)
{
   memcpy(&lcdReport, &byte, 1); // update lcdReport
}


void lcdAction(lcdFunction action)
{
 RobotStatus.RequestAction = A_lcdFunction;
 memcpy(&SpiBuf[0], &RobotStatus, 1);
 SpiBuf[1] = action;
 SpiBuf[2] = SPI_EOT;
 SpiBufNum = 2;
 updateLcdReport(lcdSpiXferBuff());
}

void lcdPrintString(u8 row, u8 column, const char *text)
{
 RobotStatus.RequestAction = A_printString;
 memcpy(&SpiBuf[0], &RobotStatus, 1);
 SpiBuf[1] = ((row << 4) | (column & 0XF));
 u8 len = strlcpy_P(&SpiBuf[2], text, SPI_BUFFER_LENGHT);
 SpiBuf[len += 3] = SPI_EOT;
 SpiBufNum = len;
 updateLcdReport(lcdSpiXferBuff());
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
 memcpy(&SpiBuf[0], &RobotStatus, 1);
 SpiBuf[1] = ((row << 4) | (column & 0XF));
 va_start(args, fmt);
 u8 len = vsprintf_P(&SpiBuf[2], fmt, args);
 va_end(args);
 SpiBuf[len += 3] = SPI_EOT;
 SpiBufNum = len;
 updateLcdReport(lcdSpiXferBuff());
}
