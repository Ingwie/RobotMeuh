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

// Last process value, used to find derivative of process value.
s16 Blade_lastMeasuredValue;
// Summation of errors, used for integrate calculations
s32 Blade_sumError;
// Maximum allowed error, avoid overflow
s16 Blade_maxError;
// Maximum allowed sumerror, avoid overflow
s32 Blade_maxSumError;

void initBladePid()
{
 Blade_lastMeasuredValue = 0;
 Blade_sumError = 0;
 Blade_maxError = INT16_MAX / (RobotMeuh.Blade_P_Factor + 1);
 Blade_maxSumError = (INT32_MAX / 2) / (RobotMeuh.Blade_I_Factor + 1);
}

void BladePid(s16 espectedValue, s16 measuredValue)
{
 s16 error, p_term, d_term;
 s32 i_term, ret, temp;

 error = espectedValue - measuredValue;

// Calculate Pterm and limit error overflow
 if (error > Blade_maxError)
  {
   p_term = INT16_MAX;
  }
 else if (error < -Blade_maxError)
  {
   p_term = -INT16_MAX;
  }
 else
  {
   p_term = RobotMeuh.Blade_P_Factor * error;
  }

// Calculate Iterm and limit integral runaway
 temp = Blade_sumError + error;
 if(temp > Blade_maxSumError)
  {
   i_term = (INT32_MAX / 2);
   Blade_sumError = Blade_maxSumError;
  }
 else if(temp < -Blade_maxSumError)
  {
   i_term = -(INT32_MAX / 2);
   Blade_sumError = -Blade_maxSumError;
  }
 else
  {
   Blade_sumError = temp;
   i_term = RobotMeuh.Blade_I_Factor * Blade_sumError;
  }

// Calculate Dterm
 d_term = RobotMeuh.Blade_D_Factor * (Blade_lastMeasuredValue - measuredValue);

 Blade_lastMeasuredValue = measuredValue;

 ret = (p_term + i_term + d_term) / PID_SCALING_FACTOR;
 ret = limit<s32>(0, ret, 9999);

 OCR5C =(u16)ret; // should work, OCR5C is double buffered
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
 set_output_on(BladeEnablePin); // Stop ! todo check brake (delay)
 TCCR5B &= ~_BV(CS50); // Disable timer 5
 OCR5C = 0; // off
 BrushlessBlade.IsCutting = false;
 BrushlessBlade.PWMValue = 0;
 initBladePid(); // reset pid values
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
 BladePid(BrushlessBlade.PWMValue, BrushlessBlade.RPM);
}

ISR(TIMER5_CAPT_vect, ISR_NOBLOCK) // Hall sensor detected (3 per turn on 42BLS03 with JY01 controler ?)
{
 ++bladeTick; // just increase tickcount ... ICR5 not used
}
