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

#define WHEELDIAMETER       270.0f  // mm
#define WEELPERIMETER       WHEELDIAMETER * M_PI

#define MICROSTEP           32.0f  // DRV8825 uStepping used
#define STEPPERREV          200.0f
#define REDUCTION           5.0f   // To test

#define MAXPULSEFREQ        (62500.0f / 2.0f)
#define MINPULSEFREQ        ((62500.0f / 65535.0f) / 2.0f)
#define MAXROBOTSPEED       (WEELPERIMETER / (REDUCTION * STEPPERREV * MICROSTEP)) * MAXPULSEFREQ // 828.35 mm/Sec
#define MINROBOTSPEED       (WEELPERIMETER / (REDUCTION * STEPPERREV * MICROSTEP)) * MINPULSEFREQ //0.0126 mm/Sec

#define MAXSTEPPERSPEED      32000u
#define ACCELDEF             100
#define DECELDEF             255

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
#define FREQ1            (FREQR1/0xFFF)
#define FREQ2            (FREQR2/0xFFF)
#define FREQ3            (FREQR3/0xFFF)
#define FREQ4            (FREQR4/0xFFF)
#define FREQ5            (FREQR5/0xFFF)

// Value to drive StepperEngine
extern int16_t L_RequestSpeed;
extern int16_t R_RequestSpeed;
// StepperEngine values
extern int16_t L_ActualSpeed;
extern int16_t R_ActualSpeed;
extern uint8_t  wheelAcceleration;
extern uint8_t  wheelDeceleration;
// Total pulses sended
extern uint32_t L_StepCourse;
extern uint32_t R_StepCourse;

extern uint8_t L_Prescaler; // todo remove

uint8_t computeStepperWheelSpeed();
void initStepperWeel();
void enableStepperWheel();
void disableStepperWheel();
void stopL_StepperWheel();
void restartL_StepperWheel();
void stopR_StepperWheel();
void restartR_StepperWheel();

#endif // STEPPERWHEEL_H_INCLUDED
/*
*
*
*
*
*
/
/
/
/
/
*/
