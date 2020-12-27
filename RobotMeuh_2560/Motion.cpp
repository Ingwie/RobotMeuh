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

#include "Motion.h"

// Last process value, used to find derivative of process value.
s16 Dir_lastMeasuredValue = 0;
// Summation of errors, used for integrate calculations
s32 Dir_sumError = 0;
// Maximum allowed error, avoid overflow
s16 Dir_maxError = INT16_MAX / (RobotMeuh.Dir_P_Factor + 1);
// Maximum allowed sumerror, avoid overflow
s32 Dir_maxSumError = (INT32_MAX / 2) / (RobotMeuh.Dir_I_Factor + 1);

void initDirPid()
{
 Dir_lastMeasuredValue = 0;
 Dir_sumError = 0;
 Dir_maxError = INT16_MAX / (RobotMeuh.Dir_P_Factor + 1);
 Dir_maxSumError = (INT32_MAX / 2) / (RobotMeuh.Dir_I_Factor + 1);
}

s16 DirPid(s16 espectedValue, s16 measuredValue)
{
 s16 error, p_term, d_term;
 s32 i_term, ret, temp;

 error = espectedValue - measuredValue;

// Calculate Pterm and limit error overflow
 if (error > Dir_maxError)
  {
   p_term = INT16_MAX;
  }
 else if (error < -Dir_maxError)
  {
   p_term = -INT16_MAX;
  }
 else
  {
   p_term = RobotMeuh.Dir_P_Factor * error;
  }

// Calculate Iterm and limit integral runaway
 temp = Dir_sumError + error;
 if(temp > Dir_maxSumError)
  {
   i_term = (INT32_MAX / 2);
   Dir_sumError = Dir_maxSumError;
  }
 else if(temp < -Dir_maxSumError)
  {
   i_term = -(INT32_MAX / 2);
   Dir_sumError = -Dir_maxSumError;
  }
 else
  {
   Dir_sumError = temp;
   i_term = RobotMeuh.Dir_I_Factor * Dir_sumError;
  }

// Calculate Dterm
 d_term = RobotMeuh.Dir_D_Factor * (Dir_lastMeasuredValue - measuredValue);

 Dir_lastMeasuredValue = measuredValue;

 ret = (p_term + i_term + d_term) / PID_SCALING_FACTOR;
 ret = limit<s32>(INT16_MIN, ret, INT16_MAX);

 return((s16)ret);
}

s16 findShortestAngle(s16 from, s16 to) // angles in degres X 10 (-1800 to 1800)
{
 s16 ret = from - to;
 if (ret < MINANGLE)
  {
   ret = ret + 2 * MAXANGLE;
  }
 else if (ret > MAXANGLE)
  {
   ret = ret - 2 * MAXANGLE;
  }
 return ret;
}

/*
s16 findActualrRadius(s16 L_Speed, s16 R_Speed)
{
 s16 ret = 0;
 if (L_Speed != R_Speed) // same speed radius is "0" else compute it
  {
   ret = ((WHELLBASE / 2) * ((L_Speed + R_Speed) / (L_Speed - R_Speed)));
  }
 return ret;
}

s16 findSpeedFactorForRadius(s16 radius) // factor * 1024
{
 s16 ret = 0;
 s16 dblRadius = radius << 1; // X 2
 if (dblRadius = WHELLBASE)
  {
    ++dblRadius; // avoid div by 0 = one static wheel
  }
   return = (((dblRadius + WHELLBASE) * 1024) / (dblRadius - WHELLBASE));
}
*/

/////// MOTION

u8 Motion_FollowAngle(s16 speed, s16 angle)
{
 s16 diffAngle = findShortestAngle(ImuValues.yaw, angle);
 return computeStepperWheelPulses(speed, DirPid(diffAngle, 0));
}

u8 Motion_Turn(s16 rate, s16 radius) // radius in cM clockwise
{
 s16 lSpeed = rate * (radius + (WHELLBASE / 2));
 s16 rSpeed = rate * (radius - (WHELLBASE / 2));
 return forceStepperWheelPulses(lSpeed, rSpeed);
}
