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


#include "RobotMeuh.h"

//ROBOTMEUH
RobotMeuh_t RobotMeuh;
Status_t RobotStatus = {0};
DataLcdToMain_t lcdReport = {0};
SystemBools_t SystemBools = {0};
ImuValues_t ImuValues = {0};

//SPI (LCD com.)
char SpiBuf[SPI_BUFFER_LENGHT] = {SPI_EOT};
volatile u8 SpiBufNum = 0;

//TIME
time_t rtcTime;
u8 counter8mS; // Updated in TaskScheduler (ISR(TIMER0_COMPA_vect))


void initRobotMeuh()
{
 initSerialCli();
 initSerialLcd();
 initSpiMasterMode(); // master SPI
 initI2C();           // I2C
//readEeprom();      // Todo Fram ?
 initRTC();
 initImus();
 initFusionImu();
 initStepperWeel();   // timer 3 & 4
 initBrushlessBlade();// timer 5
 initTaskScheduler(); // timer 0
}

int main(void)
{
 _delay_ms(500); // Wake up !
 initRobotMeuh(); // global initialisation
 sei(); // Enable interrupt
 lcdDispOffClear();
 for (u8 i = 0; i < 10; ++i)
  {
   lcdLedOff();
   _delay_ms(100);
   lcdLedOn();
   _delay_ms(100);
  }
 forceMenu(M_FIRST);
 _delay_ms(500);



 u8 onetime = 0;
 do
  {
   _delay_ms(300);
   lcdDispOffClear();

// TEST //
#define TESTMENU

   if (!onetime)
    {
     BrushlessBladeCutAt(9000);
     enableStepperWheel();
     onetime = 1;
    }

#if defined (TESTMENU)
   // Show menus
   do
    {
     for (u16 i = 0; i < 50; ++i)
      {
       SerialCliPrint("totototototo");SerialCliPrint("tatatatatata");SerialCliPrint("titititititi");
       SerialCliSend();

       forceMenu(M_LCDREPORT);
       //forceMenu((menuArray)onetime);
       _delay_ms(100);
      }
     if (++onetime == M_MENUNUMBER) onetime = M_FIRST;
    }
   while (1);
#endif
#if defined (EULER)
   forceMenu(M_IMUFUSION);
#endif
#if defined (GYRO)
   forceMenu(M_IMUGYRO);
#endif
#if defined (ACC)
   forceMenu(M_IMUACC);
#endif
#if defined (MAG)
   forceMenu(M_IMUMAG);
#endif
#if defined (SW)
   lcdPrintf(0, 0, PSTR("L%6i A%6i"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("R%6i A%6i"), R_RequestSpeed, R_ActualSpeed);
   //lcdPrintf(1, 0, PSTR("V%4i St%8u"), wheelActualSpeed, L_StepCourse);
#endif
   lcdDispOn();
  }
 while(1);


 return 0;
}

// Fast task
void Task8mS()
{
 if (!SystemBools.whellSpeedOk) // if we need to compute wheels acceleration
  {
   SystemBools.whellSpeedOk = (computeStepperWheelSpeed())? 0 : 1;
  }
}

// Mid task
void Task32mS()
{
 computeFusionImu(); // Compute imus (approx 150uS)
 Motion_FollowAngle(20000, 900);
 //Motion_Turn(100, 0);
}

// Slow task
void Task1S() // ISR mode
{
 if (BrushlessBlade.IsCutting)
  {
   BrushlessBladeUpdate(); // reset ISR mode
  }

}
