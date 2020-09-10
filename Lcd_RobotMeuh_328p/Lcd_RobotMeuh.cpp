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

#include "Lcd_RobotMeuh.h"

//ROBOTMEUH
Status_t RobotStatus = {0};
DataLcdToMain_t Report = {0};

//SPI
uint8_t SpiRet = 0;
char SpiBuf[SPI_BUFFER_LENGHT] = {SPI_EOT};
uint8_t SpiBufNum = 0;

void computeSpiBuf()
{
 memcpy(&RobotStatus, &SpiBuf[0], 1);

 switch (RobotStatus.RequestAction)
  {

  case A_printString :
   // print todo
   break;

  case A_rainTriggerValue :
   //todo
   break;

  default : // case A_none ...
   break;
  }
 cli();
 SpiBufNum = 0; // reset buffer
 sei();
}

int main()
{
// Init All
 lcdInit();
 initKey();
 memcpy(&SpiRet, &Report, 1); // Update Spiret
 initSpiSlaveMode();
 adcInit();
 initTimer8mS();

//Update Report for keys
 updateKeys();

 pin_high(LCDPinLed);
 lcd_printStringAt(1, 4, "ROBOT MEUH");
 lcd_printStringAt(2, 2, "Connection...");

 do
  {
   //TODO
   uint8_t toremove = GETRAINSENSORVOLTAGE();
   _delay_ms(400);
   lcdRShift();
  }
 while(1);

 return 0;
}
