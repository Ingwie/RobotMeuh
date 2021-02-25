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

c_PID DirPid;

void initDirPid()
{
#define RM RobotMeuh
 DirPid.init(RM.Dir_P_Factor, RM.Dir_I_Factor, RM.Dir_D_Factor, PID_SCALING_FACTOR, MAXROBOTSPEED);
#undef RM
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

s16 constrainAngle(s16 angle) // keep value in range : degres X 10 (-1799 to 1800)
{
 s16 ret = angle;
 ret += (ret > MAXANGLE) ? (-2 * MAXANGLE) : (ret < MINANGLE) ? (2 * MAXANGLE) : 0;
 return ret;
}

s16 findShortestAngle(s16 from, s16 to) // angles in degres X 10 (-1799 to 1800)
{
 return constrainAngle(to - from);
}

s16 findRelativeAngle(s16 offset) // offset in degres X 10 (-1800 to 1800)
{
 return constrainAngle(ImuValues.yaw + offset);
}

/////// MOTION

u8 Motion_FollowAngle(s16 speed, s16 angle)
{
// update regulator's turn limit
 s32 turnLimit = ((s32)speed * RobotMeuh.WheelsRotationSpeedRate) / 100;
 DirPid.changeLimit((s16)abs(turnLimit));
// compute wheels speed dif.
 s16 diffAngle = findShortestAngle(ImuValues.yaw, constrainAngle(angle));
 DynData.deltaDir = diffAngle;
 s16 turn = DirPid.compute(diffAngle, 0);
// update values
 s16 lSpeed = speed - turn;
 s16 rSpeed = speed + turn;
 return forceStepperWheelPulses(lSpeed, rSpeed);
}

u8 Motion_Turn(s16 rate, s16 radius) // radius in cM conterclockwise (Direction +)
{
 DynData.deltaDir = 450 * sgn(radius);
 s16 lSpeed = rate * (radius - (WHELLBASE / 2));
 s16 rSpeed = rate * (radius + (WHELLBASE / 2));
 return forceStepperWheelPulses(lSpeed, rSpeed);
}
