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

#include "BrushlessBlade.h"

BrushlessBlade_t BrushlessBlade;
volatile u16 bladeTick;  // hall sensor tick

c_PID BladePid;

void initBladePid()
{
#define RM RobotMeuh
 BladePid.init(RM.Blade_P_Factor, RM.Blade_I_Factor, RM.Blade_D_Factor, PID_SCALING_FACTOR, 9999);
#undef RM
}

void initBrushlessBlade()
{
 set_output_on(BladeEnablePin);
 set_output_off(BladeClockwisePin);
 set_output_off(BladePwmOutPin);
 set_input(BladePulsesinPin);
 memset(&BrushlessBlade, 0, sizeof(BrushlessBlade)); // Reset all fields

// Pid
 initBladePid();

// Setup Timer 5 Mode 15 fast PWM, OCnA set TOP value, OCnC output & ICR5 capture input.
 TCCR5A = _BV(COM5C1) | _BV(COM5B0) | _BV(WGM51) | _BV(WGM50);
// Input rising edge & noise canceler on, prescaler 1
 TCCR5B = _BV(ICNC5) | _BV(ICES5) | _BV(WGM53) | _BV(WGM52); // | _BV(CS50) -> do not run yet ...
 TCCR5C = 0;
 TIMSK5 = _BV(ICIE5); // Active capture ISR
 OCR5A = 10000; // 1.6 Khz 625uS (62.5nS tick)
 OCR5C = 0; // initial value (off)
//ICR5 = 0; // reset input counter (not used)
}

void BrushlessBladeStop()
{
 TCCR5B &= ~_BV(CS50); // Disable timer 5
 OCR5C = 0; // off
 set_output_on(BladeEnablePin); // Stop ! todo check brake (delay)
 BrushlessBlade.IsCutting = false;
 BrushlessBlade.PWMValue = 0;
}

void BrushlessBladeCutAt(s16 speed)
{
 if (!speed) BrushlessBladeStop();
 if (speed > 0) // CW | CCW ?
  {
   set_output_on(BladeClockwisePin);
   BrushlessBlade.Clockwise = true;
  }
 else
  {
   set_output_off(BladeClockwisePin);
   BrushlessBlade.Clockwise = false;
  }
 limit<s16>(-9999, speed, 9999); // 10000 stop output (Todo : test)
 OCR5C = abs(speed); // Set PPM average
 TCCR5B |= _BV(CS50); // Enable timer 5
 set_output_off(BladeEnablePin); // Cut !
 BrushlessBlade.IsCutting = true;
 BrushlessBlade.PWMValue = speed*2;
}

void BrushlessBladeReadRPM()
{
//  3 tick per turn...
// -> One tick per Sec = 1/3 Turn/Sec

 bladeTick *= 20; // /=3 (Turn/Sec) and *=60 (secondes)
 bladeTick = ((BrushlessBlade.RPM << 2) + bladeTick) / 5; // Poor low pass filter
 BrushlessBlade.RPM = bladeTick;
 bladeTick = 0; // Reset counter
}

void BrushlessBladeUpdate() // called every 1 seconde (ISR mode)
{
 BrushlessBladeReadRPM();
 sei(); // reset ISR mode
 BladePid.compute(BrushlessBlade.PWMValue, BrushlessBlade.RPM);
}

ISR(TIMER5_CAPT_vect, ISR_NOBLOCK) // Hall sensor detected (3 per turn on 42BLS03 with JY01 controler ?)
{
 ++bladeTick; // just increase tickcount ... ICR5 not used
}
