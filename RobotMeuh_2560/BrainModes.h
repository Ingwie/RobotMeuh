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

#ifndef BRAINMODES_H_INCLUDED
#define BRAINMODES_H_INCLUDED

#include "RobotMeuh.h"

// All brains
extern const void brainWakeUp(); // first brain
extern const void brainCharging(); // manage battery voltage
extern const void brainExitChargingStation(); // Move 1 meter backward
extern const void brainStartMowing(); // cut grass !

/////////////////////////////////////////

const p_Function brainFunctions[] PROGMEM = // Brain function array
{
 brainWakeUp,
 brainCharging,
 brainExitChargingStation,
 brainStartMowing,

};

enum brainArray // Image of brain function array
{
 B_WAKEUP,
 B_CHARGING,
 B_EXITCHARGINGSTATION,
 B_STARTMOWING,

 B_NUMBER
};

PACK(typedef struct
{
 brainArray prevBrainToken; // to remember last state
 u8  init:1; // used to init brain values (is reseted in brainCompute())
 u8  unused:2;
 u8  bladeDir; // Cw CCw
 s16 angle; // angle :12
 u16 deltaDist; // distance
 s16 speed; // speed
 u16 counter; // counter
}) BrainDatas_t;

extern brainArray brainToken;
extern p_Function brainPointer; // brainPointer() call brain[brainToken] function
extern BrainDatas_t BrainData;

void brainCompute();
void forceBrain(brainArray num);

#endif // BRAINMODES_H_INCLUDED
