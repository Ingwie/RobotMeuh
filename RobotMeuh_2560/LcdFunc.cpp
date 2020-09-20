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

uint8_t lcdSpiXferBuff()
{
// Full Duplex (4 wire) spi
 pin_low(SpiLcdSSPin);
 for (uint8_t i = 0; i <= SpiBufNum; i++)
  {
   SPDR = SpiBuf[i];
   /* Wait for transfer to complete */
   while (!(SPSR & (1<<SPIF)));
  }
 pin_high(SpiLcdSSPin);
 return SPDR;
}

uint8_t lcdAction(uint8_t action)
{
 RobotStatus.RequestAction = A_lcdFunction;
 memcpy(&SpiBuf[0], &RobotStatus, 1);
 SpiBuf[1] = action;
 SpiBuf[2] = SPI_EOT;
 SpiBufNum = 2;
 return lcdSpiXferBuff();
}

uint8_t lcdPrintString(uint8_t row, uint8_t column, const char *text)
{
 RobotStatus.RequestAction = A_printString;
 memcpy(&SpiBuf[0], &RobotStatus, 1);
 SpiBuf[1] = row;
 SpiBuf[2] = column;
 uint8_t len = strlen(text);
 memcpy(&SpiBuf[3], text, len);
 SpiBuf[len += 4] = SPI_EOT;
 SpiBufNum = len;
 return lcdSpiXferBuff();
}

//lcdSpiXfer(uint8_t value)
