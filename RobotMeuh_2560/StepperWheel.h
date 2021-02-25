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


#ifndef STEPPERWHEEL_H_INCLUDED
#define STEPPERWHEEL_H_INCLUDED

#include "RobotMeuh.h"

// Prescaler
#define PRESCVALUE1         1u
#define PRESCVALUE2         8u
#define PRESCVALUE3         64u
#define PRESCVALUE4         256u
#define PRESCVALUE5         1024u
#define PRESCALERRESETMASK  (_BV(CS12) | _BV(CS11) | _BV(CS10))
#define PRESCALER1          (_BV(CS10))
#define PRESCALER2          (_BV(CS11))
#define PRESCALER3          (_BV(CS11) | _BV(CS10))
#define PRESCALER4          (_BV(CS12))
#define PRESCALER5          (_BV(CS12) | _BV(CS10))
// Frequency raw
#define FREQR1              (F_CPU/PRESCVALUE1)
#define FREQR2              (F_CPU/PRESCVALUE2)
#define FREQR3              (F_CPU/PRESCVALUE3)
#define FREQR4              (F_CPU/PRESCVALUE4)
#define FREQR5              (F_CPU/PRESCVALUE5)
// Timer (16bits) min frequency
#define FREQMIN1            (FREQR1/0xFFFE)
#define FREQMIN2            (FREQR2/0xFFFE)
#define FREQMIN3            (FREQR3/0xFFFE)
#define FREQMIN4            (FREQR4/0xFFFE)
#define FREQMIN5            (FREQR5/0xFFFE)
// Timer (16bits) switch frequency
#define FREQ1               (FREQR1/0xFFF)
#define FREQ2               (FREQR2/0xFFF)
#define FREQ3               (FREQR3/0xFFF)
#define FREQ4               (FREQR4/0xFFF)
#define FREQ5               (FREQR5/0xFFF)

// Value to drive StepperEngine

// StepperEngine values
extern s16 L_RequestSpeed;
extern s16 R_RequestSpeed;
extern s16 L_ActualSpeed;
extern s16 R_ActualSpeed;
extern s16 wheelActualSpeed;
// Total pulses sended
extern u32 L_StepCourse;
extern u32 R_StepCourse;


u8 forceStepperWheelPulses(s16 lSpeed, s16 rSpeed);
u8 computeStepperWheelSpeed();
void initStepperWeel();
void enableStepperWheel();
void disableStepperWheel();
void stopL_StepperWheel();
void restartL_StepperWheel();
void stopR_StepperWheel();
void restartR_StepperWheel();
void initStepperPid();


s16 pulsesToDecimeterPerMinute(s16 pulses);
s16 decimeterPerMinuteToPulses(s16 cmpm);

#endif // STEPPERWHEEL_H_INCLUDED
