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

#include "Pid.h"

c_PID::c_PID()
{
}

void c_PID::init(s16 K_P, s16 K_I, s16 K_D, u16 K_factor, s16 output_limit)
{
#define KF 10
 KP = (float)K_P / (K_factor * KF);
 KI = (float)K_I / (K_factor * KF);
 KD = (float)K_D / (K_factor * KF);
 Output_limit = output_limit;
 PrevError = 0; // reset, used for D term
 SumError = 0; // reset, used for I term
 Output = 0;
}

void c_PID::changeLimit(s16 output_limit)
{
 Output_limit = output_limit;
}

s16 c_PID::compute(s16 espectedValue, s16 measuredValue)
{
 s32 error = espectedValue - measuredValue;

// Calculate Pterm
 float value = KP * error;

// Calculate Iterm and limit integral
 SumError += error;
 s32 sumErrorMax = Output_limit * 10;
 SumError = limit<s32>((s32)-sumErrorMax, SumError, sumErrorMax);
 value += KI * SumError;

// Calculate Dterm and save error
 value += KD * (error - PrevError);
 PrevError = error;

// Calculate and limit output
 value += Output;
 Output = limit<s16>(-Output_limit, (s16)value, Output_limit);
 return Output;
}
