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

#include "StepperWheel.h"

// Value to drive StepperEngine
s16 L_RequestSpeed = 0;
s16 R_RequestSpeed = 0;
// StepperEngine values
s16 L_ActualSpeed = 0;
s16 R_ActualSpeed = 0;
s16 wheelActualSpeed = 0;

u32 L_StepCourse = 0;
u32 R_StepCourse = 0;

volatile u16 L_WheelPulses = 0;
volatile u16 R_WheelPulses = 0;

volatile u8 L_Prescaler = 0;
volatile u8 R_Prescaler = 0;

// PID
// Last process value, used to find derivative of process value.
s16 L_lastMeasuredValue = 0;
s16 R_lastMeasuredValue = 0;
// Summation of errors, used for integrate calculations
s32 L_sumError = 0;
s32 R_sumError = 0;
// Maximum allowed error, avoid overflow
s16 SW_maxError = INT16_MAX / (RobotMeuh.SW_P_Factor + 1);
// Maximum allowed sumerror, avoid overflow
s32 SW_maxSumError = (INT32_MAX / 2) / (RobotMeuh.SW_I_Factor + 1);

void initStepperPid()
{
 L_lastMeasuredValue = 0;
 L_sumError = 0;
 R_lastMeasuredValue = 0;
 R_sumError = 0;
 SW_maxError = INT16_MAX / (RobotMeuh.SW_P_Factor + 1);
 SW_maxSumError = (INT32_MAX / 2) / (RobotMeuh.SW_I_Factor + 1);
}

s16 L_Pid(s16 espectedValue, s16 measuredValue)
{
 s16 error, p_term, d_term;
 s32 i_term, ret, temp;

 error = espectedValue - measuredValue;

// Calculate Pterm and limit error overflow
 if (error > SW_maxError)
  {
   p_term = INT16_MAX;
  }
 else if (error < -SW_maxError)
  {
   p_term = -INT16_MAX;
  }
 else
  {
   p_term = RobotMeuh.SW_P_Factor * error;
  }

// Calculate Iterm and limit integral runaway
 temp = L_sumError + error;

 if(temp > SW_maxSumError)
  {
   i_term = (INT32_MAX / 2);
   L_sumError = SW_maxSumError;
  }
 else if(temp < -SW_maxSumError)
  {
   i_term = -(INT32_MAX / 2);
   L_sumError = -SW_maxSumError;
  }
 else
  {
   L_sumError = temp;
   i_term = RobotMeuh.SW_I_Factor * L_sumError;
  }

// Calculate Dterm
 d_term = RobotMeuh.SW_D_Factor * (L_lastMeasuredValue - measuredValue);

 L_lastMeasuredValue = measuredValue;

 ret = (p_term + i_term + d_term) / PID_SCALING_FACTOR;
 ret = limit<s32>(-MAXSTEPPERSPEED, ret, MAXSTEPPERSPEED);

 return((s16)ret);
}

s16 R_Pid(s16 espectedValue, s16 measuredValue)
{
 s16 error, p_term, d_term;
 s32 i_term, ret, temp;

 error = espectedValue - measuredValue;

// Calculate Pterm and limit error overflow
 if (error > SW_maxError)
  {
   p_term = INT16_MAX;
  }
 else if (error < -SW_maxError)
  {
   p_term = -INT16_MAX;
  }
 else
  {
   p_term = RobotMeuh.SW_P_Factor * error;
  }

// Calculate Iterm and limit integral runaway
 temp = R_sumError + error;

 if(temp > SW_maxSumError)
  {
   i_term = (INT32_MAX / 2);
   R_sumError = SW_maxSumError;
  }
 else if(temp < -SW_maxSumError)
  {
   i_term = -(INT32_MAX / 2);
   R_sumError = -SW_maxSumError;
  }
 else
  {
   R_sumError = temp;
   i_term = RobotMeuh.SW_I_Factor * R_sumError;
  }

// Calculate Dterm
 d_term = RobotMeuh.SW_D_Factor * (R_lastMeasuredValue - measuredValue);

 R_lastMeasuredValue = measuredValue;

 ret = (p_term + i_term + d_term) / PID_SCALING_FACTOR;
 ret = limit<s32>(-MAXSTEPPERSPEED, ret, MAXSTEPPERSPEED);

 return((s16)ret);
}

static void linearizePulses(u16 freq, u8 * prescaler, u16 * pulses)
{
 if (freq <  FREQ5)
  {
   * prescaler = _BV(WGM33) | _BV(WGM32)  | PRESCALER5;
   * pulses = (u16)(F_CPU / (freq * PRESCVALUE5));
  }
 else if (freq < FREQ4)
  {
   * prescaler = _BV(WGM33) | _BV(WGM32)  | PRESCALER4;
   * pulses = (u16)(F_CPU / (freq *PRESCVALUE4));
  }
 else if (freq < FREQ3)
  {
   * prescaler = _BV(WGM33) | _BV(WGM32)  | PRESCALER3;
   * pulses = (u16)(F_CPU / (freq * PRESCVALUE3));
  }
 else if (freq < FREQ2)
  {
   * prescaler = _BV(WGM33) | _BV(WGM32)  | PRESCALER2;
   * pulses = (u16)(F_CPU / (freq * PRESCVALUE2));
  }
 else
  {
   * prescaler = _BV(WGM33) | _BV(WGM32) | PRESCALER1;
   * pulses = (u16)(F_CPU / (freq * PRESCVALUE1));
  }
}

u8 computeStepperWheelSpeed() // Must be called at little interval, return 0 if final speed is set
{
 u8 skip = 0;

// Left Wheel
 if (L_RequestSpeed != L_ActualSpeed)
  {
   ++skip;
   L_ActualSpeed = L_Pid(L_RequestSpeed, L_ActualSpeed);
  }
// Right Wheel
 if (R_RequestSpeed != R_ActualSpeed)
  {
   ++skip;
   R_ActualSpeed = R_Pid(R_RequestSpeed, R_ActualSpeed);
  }

 if (skip)
  {
// Choose prescaler & Compute Timer pulses
   u8  L_presc;
   u16 L_pulses;
   linearizePulses(abs(L_ActualSpeed), &L_presc, &L_pulses);

   u8  R_presc;
   u16 R_pulses;
   linearizePulses(abs(R_ActualSpeed), &R_presc, &R_pulses);

   u8 sreg = SREG; // Set speed in atomic mode
   cli(); // ISR OFF
// stop interrupt or re enable it
   (!L_ActualSpeed)? stopL_StepperWheel() : restartL_StepperWheel();
   (!R_ActualSpeed)? stopR_StepperWheel() : restartR_StepperWheel();
// Set direction
   (L_ActualSpeed >= 0)? pin_low(L_WheelDirPin) : pin_high(L_WheelDirPin);
   (R_ActualSpeed >= 0)? pin_high(R_WheelDirPin) : pin_low(R_WheelDirPin);
// Update prescaler & pulses
   L_Prescaler = L_presc;
   L_WheelPulses = L_pulses;
   R_Prescaler = R_presc;
   R_WheelPulses = R_pulses;
   SREG = sreg; // ISR ON
   wheelActualSpeed = ((s32)L_ActualSpeed + R_ActualSpeed) / 2;
   RobotStatus.RunForward = (wheelActualSpeed < 0)? 0 : 1;
  }
 return skip;
}

u8 computeStepperWheelPulses (s16 speed, s16 turn)
{
// espected values
 s16 Speed = speed; // needed using constant values
 s16 Turn = turn;
// limit
 Speed = limit<s16>((-MAXROBOTSPEED), Speed, MAXROBOTSPEED);
 Turn = limit<s16>((-MAXROBOTTURN), Turn, MAXROBOTTURN);
// direction
 s8 direction = (speed < 0)? -1 : 1; // no speed -> turn running forward
// update values
 L_RequestSpeed = Speed + (Turn * direction);
 R_RequestSpeed = Speed - (Turn * direction);
 return computeStepperWheelSpeed();
}

u8 forceStepperWheelPulses(s16 lSpeed, s16 rSpeed)
{
 L_RequestSpeed = lSpeed;
 R_RequestSpeed = rSpeed;
 return computeStepperWheelSpeed();
}
void initStepperWeel()
{
// output
 set_output_on(L_WheelEnablePin);
 set_output_on(R_WheelEnablePin);
 set_output_on(L_WheelDirPin);
 set_output_on(R_WheelDirPin);
 set_output_off(L_WheelPulsePin);
 set_output_off(R_WheelPulsePin);

// Setup Timer 3 & 4 Mode 15 fast PWM, OCnA set TOP value, OCnB output.

// PH4 OC4B drive left motor.
 TCCR4A = _BV(COM4B1) | _BV(COM4B0) | _BV(WGM41) | _BV(WGM40);
 TCCR4B = _BV(WGM43) | _BV(WGM42);
 TCCR4C = 0;
 TIMSK4 = _BV(TOIE4);
 OCR4A = 0xF;
 OCR4B = 4;

// PE4 OC3B drive right motor.
 TCCR3A = _BV(COM3B1) | _BV(COM3B0) | _BV(WGM31) | _BV(WGM30);
 TCCR3B = _BV(WGM33) | _BV(WGM32);
 TCCR3C = 0;
 TIMSK3 = _BV(TOIE3);
 OCR3A = 0xF;
 OCR3B = 4;
}

void enableStepperWheel()
{
 initStepperPid();
 pin_low(L_WheelEnablePin);
 pin_low(R_WheelEnablePin);
 u8 sreg = SREG;
 cli();
 TCNT4 = 0; // Reset timer value
 TCNT3 = 0; // Reset timer value
 SREG = sreg;
}

void disableStepperWheel()
{
 TCCR4B &= ~PRESCALERRESETMASK; // Prescaler disabled
 TCCR3B &= ~PRESCALERRESETMASK; // Prescaler disabled
 pin_high(L_WheelEnablePin);
 pin_high(R_WheelEnablePin);
 pin_low(L_WheelPulsePin);
 pin_low(R_WheelPulsePin);
}

void stopL_StepperWheel()
{
 TCCR4B &= ~PRESCALERRESETMASK;
}

void restartL_StepperWheel()
{
 if (!(TCCR4B & 0x3)) // Not prescaler ?
  {
   TCCR4B |= PRESCALER5;
   TIFR4 |= TOV4; // Simulate isr
  }
}

void stopR_StepperWheel()
{
 TCCR3B &= ~PRESCALERRESETMASK;
}

void restartR_StepperWheel()
{
 if (!(TCCR3B & 0x3)) // Not prescaler ?
  {
   TCCR3B |= PRESCALER5;
   TIFR3 |= TOV3; // Simulate isr
  }
}

ISR(TIMER4_OVF_vect) // left motor
{
 ++L_StepCourse;
 if (!SystemBools.whellSpeedOk)
  {
   OCR4A = L_WheelPulses;
   TCCR4B = L_Prescaler;
  }
}

ISR(TIMER3_OVF_vect) // right motor
{
 ++R_StepCourse;
 if (!SystemBools.whellSpeedOk)
  {
   OCR3A = R_WheelPulses;
   TCCR3B = R_Prescaler;
  }
}
