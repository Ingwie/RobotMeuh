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
int16_t Dir_lastMeasuredValue = 0;
// Summation of errors, used for integrate calculations
int32_t Dir_sumError = 0;
// Maximum allowed error, avoid overflow
int16_t Dir_maxError = INT16_MAX / (RobotMeuh.Dir_P_Factor + 1);
// Maximum allowed sumerror, avoid overflow
int32_t Dir_maxSumError = (INT32_MAX / 2) / (RobotMeuh.Dir_I_Factor + 1);

void initDirPid()
{
 Dir_lastMeasuredValue = 0;
 Dir_sumError = 0;
 Dir_maxError = INT16_MAX / (RobotMeuh.Dir_P_Factor + 1);
 Dir_maxSumError = (INT32_MAX / 2) / (RobotMeuh.Dir_I_Factor + 1);
}

int16_t DirPid(int16_t espectedValue, int16_t measuredValue)
{
 int16_t error, p_term, d_term;
 int32_t i_term, ret, temp;

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
 ret = limit<int32_t>(INT16_MIN, ret, INT16_MAX);

 return((int16_t)ret);
}

int16_t findShortestAngle(int16_t from, int16_t to) // angles in degres X 10 (-1800 to 1800)
{
 int16_t ret = from - to;
 if (ret < -1800)
  {
   ret = ret + 2 * 1800;
  }
 else if (ret > 1800)
  {
   ret = ret - 2 * 1800;
  }
 return ret;
}

uint8_t followCourse(int16_t speed, int16_t course)
{
 int16_t courseDiff = findShortestAngle(ImuValues.yaw, course);
 return computeStepperWheelDirection(speed, DirPid(courseDiff, 0));
}
