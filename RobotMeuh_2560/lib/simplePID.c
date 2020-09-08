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

// LIB FROM https://github.com/uLipe/AvrSimplePID

#include "simplePID.h"

int PidControllerInit(PidController *pid, PidNewCoefficients *coeff)
{
 if(!pid)
  return -1;

 if(!coeff)
  return -1;

 //Make a IIR Z-trans filter from PID parameters:
 pid->bn[0] = (coeff->new_kp + coeff->new_ki + coeff->new_kd);
 pid->bn[1] = (coeff->new_kp + coeff->new_ki) - (coeff->new_kd + coeff->new_kd);
 pid->bn[2] = coeff->new_kd;

 pid->next_xn = 0;
 pid->next_yn = 0;

 pid->xn[0] = 0;
 pid->xn[1] = 0;
 pid->xn[2] = 0;
 pid->xn[3] = 0;

 pid->yn[0] = 0;
 pid->yn[1] = 0;
 pid->yn[2] = 0;

 return 0;
}

int PidControllerTune(PidController *pid, PidNewCoefficients *coeff)
{
 if(!pid)
  return -1;

 if(!coeff)
  return -1;

 pid->bn[0] = (coeff->new_kp + coeff->new_ki + coeff->new_kd);
 pid->bn[1] = (coeff->new_kp + coeff->new_ki) - (coeff->new_kd + coeff->new_kd);
 pid->bn[2] = coeff->new_kd;

 pid->xn[0] = 0;
 pid->xn[1] = 0;
 pid->xn[2] = 0;
 pid->xn[3] = 0;

 pid->yn[0] = 0;
 pid->yn[1] = 0;
 pid->yn[2] = 0;

 return 0;
}
