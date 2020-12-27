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
volatile u8 SpiRet = 0;
char SpiBuf[SPI_BUFFER_NUM][SPI_BUFFER_LENGHT] = {SPI_EOT};
volatile u8 SpiBufWrite = 0;
u8 SpiBufRead = 0;

void computeSpiBuf(u8 bufferNum)
{
 memcpy(&RobotStatus, &SpiBuf[bufferNum][0], 1);

 switch (RobotStatus.RequestAction)
  {
  case A_lcdFunction :
   switch (SpiBuf[bufferNum][1])
    {
    case B_DispOff_Clear :
     lcdClear();
     lcdDispOff();
     break;
    case  B_DispOn :
     lcdDispOn();
     break;
    case B_Clear :
     lcdClear();
     break;
    case  B_DispOff :
     lcdDispOff();
     break;
    case  B_BlinkOn :
     lcdBlinkOn();
     break;
    case  B_BlinkOff :
     lcdBlinkOff();
     break;
    case  B_LedOn :
     lcdLedOn();
     break;
    case  B_LedOff :
     lcdLedOff();
     break;
    case  B_LShift :
     lcdLShift();
     break;
    case  B_RShift :
     lcdRShift();
     break;
    case  B_2ndRow :
     lcd2ndRow();
     break;
    case  B_Home :
     lcdHome();
     break;

    }
   break;

  case A_printString :
   lcd_printStringAt((SpiBuf[bufferNum][1] >> 4), (SpiBuf[bufferNum][1] & 0xF), &SpiBuf[bufferNum][2]);
   break;

  case A_rainTriggerValue :
   //todo
   break;

  default : // case A_none ...
   break;
  }
}

void checkSpiBuf()
{
 u8 cont = 1;
 do
  {
   if (SpiBufRead != SpiBufWrite) // there is some datas arrived
    {
     computeSpiBuf(SpiBufRead); // Compute them !
     if(++SpiBufRead >= SPI_BUFFER_NUM) SpiBufRead = 0; // Change buffer
    }
   else
    {
     cont = 0;
    }
  }
 while (cont);
}

int main()
{
// Init All
 lcdInit();
 initKey();
 updateKeys(); //Update Report for keys
 memcpy((u8*)&SpiRet, &Report, 1); // Update Spiret
 initSpiSlaveMode();
 adcInit();
 initTimer8mS();

 lcdLedOn();
 lcd_printStringAt(0, 3, "ROBOT MEUH");
 lcd_printStringAt(1, 2, "Connection...");

 do
  {
   checkSpiBuf();
   //TODO
   //u8 toremove = GETRAINSENSORVOLTAGE();
   _delay_ms(10);
  }

 while(1);

 return 0;
}
