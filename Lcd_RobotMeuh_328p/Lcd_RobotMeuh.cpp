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
/*       Copyright 2020-2022 by Ingwie (Bracame)       */
/*   Licence: GPLV3 see <http://www.gnu.org/licenses   */
/*        Compile with AVR GCC + Code::Blocks          */
/*    https://www.mediafire.com/file/cahqfrm90h7c7fy/  */
/*    Setup_OAVRCBuilder3.exe/file (Pswd : OpenAVRc)   */

#include "Lcd_RobotMeuh.h"

//ROBOTMEUH
Status_t RobotStatus = {0};
DataLcdToMain_t Report = {0};
u8 serialRet[NUM_BYTE_RET];

int main()
{
// Init All
 lcdInit();
 initKey();
 updateKeys(); //Update Report for keys
 initSerialLcd();
 memcpy((u8*)&serialRet, &Report, 1); // Update Serialret
 serialRet[NUM_BYTE_RET - 1] = 0; // end of string
 adcInit();
 initTimer8mS();
 sei(); // allow interrupts

 lcdLedOn();
 lcd_printStringAt(0, 3, "ROBOT MEUH");
 lcd_printStringAt(1, 2, "Connection...");

 do
  {
   checkSerialLcdRXBuf();
   //TODO
   //u8 toremove = GETRAINSENSORVOLTAGE();
   _delay_ms(10);
  }

 while(1);

 return 0;
}

/////////////////// INCLUDE ALL CPP AND C FILES HERE //////////////////////// -> Faster compilation

#include "HD44780.cpp"
#include "SerialLcd.cpp"
#include "Keypad.cpp"
#include "AnalogSensor.cpp"
#include "Timer8mS.cpp"
