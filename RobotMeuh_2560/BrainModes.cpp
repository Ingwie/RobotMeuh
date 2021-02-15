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
 static u8 tokenMem = 0xFF;

 if (brainToken != tokenMem) // change brain ?
  {
   BrainData.init = 1; // reset initialisation flag
   BrainData.prevBrainToken = (brainArray)tokenMem; // store previous brain
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
         forceBrain(B_EXITCHARGINGSTATION);
        }
      }
     else // not wirred : whait for play key to mow
      {
       //forceBrain(B_STARTMOWING);
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
 if (++BrainData.counter < 200)
  {
   BrainData.speed = decimeterPerMinuteToPulses(-200);
   Motion_FollowAngle(BrainData.speed, BrainData.angle); // move away from charging station
   if (BrainData.counter == 100) BrainData.angle += 450; // change direction (inner)
   if (BrainData.counter == 199) forceStepperWheelPulses(0,0); // stop todo remove ?
  }
 else
  {
   //forceBrain(B_STARTMOWING);

  }
}
