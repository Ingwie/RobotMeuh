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

#include "BrainModes.h"

brainArray brainToken;
p_Function brainPointer;
BrainDatas_t BrainData;

void brainCompute()
{
 static brainArray tokenMem = B_NUMBER;

 if (brainToken != tokenMem) // change brain ?
  {
   BrainData.init = 1; // set initialisation flag
   BrainData.prevBrainToken = tokenMem; // store previous brain
   tokenMem = brainToken;
   brainPointer = (p_Function)pgm_read_ptr_near(&brainFunctions[brainToken]); // find brainfunction[brainToken] in flash
  }

 if (brainPointer) brainPointer(); // call it
 else ERR("pointeur nul");
}

void forceBrain(brainArray num)
{
 brainToken = num;
 brainCompute();
}

//////////////////// BRAIN MODES ////////////////////////

const void brainWakeUp() // first at boot
{
 if (BrainData.init) // brain initialisation
  {
   BrainData.init = 0;
   BrainData.counter = 0;
// stop motors
   BrushlessBladeStop();
   disableStepperWheel();
  }
// determine state
 if (!checkBattery()) // battery voltage is OK
  {
   if (isTimeToWork()) // work sheduled now ?
    {
     if (readChargerVoltage() > MIN_BATTERY_VOLTAGE) // we are wirred whith the charging station
      {
       // wait 2 secondes to stabilise imus
       if (++BrainData.counter > 63)
        {
         forceBrain(B_EXITCHARGINGSTATION); // todo check zone to mow ?
        }
      }
     else // not wirred : whait for play key to mow
      {
       if (lcdReport.KeyPlay) forceBrain(B_STARTMOWING);
      }
    }
  } // no else : checkbattery auto shut down RobotMeuh
}

const void brainCharging() // manage battery voltage
{
 checkBattery();
}

const void brainExitChargingStation() // Move ~1 meter backward straight + ~1 meter @ 45° inner
{
 if (BrainData.init) // brain initialisation
  {
   BrainData.init = 0;
   BrainData.counter = 0;
   BrainData.angle = ImuValues.yaw; // store direction
   enableStepperWheel();
  }
 if (++BrainData.counter < 220)
  {
   BrainData.speed = decimeterPerMinuteToPulses(-200);
   if (BrainData.counter == 100) BrainData.angle += 450; // change direction (inner)
   if (BrainData.counter == 199) forceStepperWheelPulses(0,0); // stop todo remove ?
   Motion_FollowAngle(BrainData.speed, BrainData.angle); // move away from charging station
  }
 else
  {
   forceBrain(B_STARTMOWING);
  }
}

const void brainStartMowing() // cut grass !
{
 if (BrainData.init) // brain initialisation
  {
   BrainData.init = 0;
   //BrainData.counter = 0;
   BrainData.angle = findRelativeAngle(00); // todo random direction
   BrushlessBladeCutAt((BrainData.bladeDir)? RobotMeuh.BladeSpeed : -RobotMeuh.BladeSpeed); // every run change direction
  }
 //if (++BrainData.counter == 20) {BrainData.angle -= 100; BrainData.counter = 0;}// test
 Motion_FollowAngle(decimeterPerMinuteToPulses(RobotMeuh.WheelsSpeed), BrainData.angle); // start mowing
 //if (++BrainData.counter >= 1000) forceStepperWheelPulses(-decimeterPerMinuteToPulses(RobotMeuh.WheelsSpeed), -decimeterPerMinuteToPulses(RobotMeuh.WheelsSpeed));
}
