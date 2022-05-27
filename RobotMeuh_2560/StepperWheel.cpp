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

volatile u8 L_PulsesIn = 0;
volatile u8 R_PulsesIn = 0;

// PID
c_PID L_Pid, R_Pid;

void initStepperPid()
{
#define RM RobotMeuh
 L_Pid.init(RM.SW_P_Factor, RM.SW_I_Factor, RM.SW_D_Factor, PID_SCALING_FACTOR, MAXSTEPPERSPEED);
 R_Pid.init(RM.SW_P_Factor, RM.SW_I_Factor, RM.SW_D_Factor, PID_SCALING_FACTOR, MAXSTEPPERSPEED);
#undef RM
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

u8 computeStepperWheelSpeed() // Must be called at little interval, return 0 if actual speed == request speed
{
 u8 skip = 0;

// Left Wheel
 if (L_RequestSpeed != L_ActualSpeed)
  {
   ++skip;
   L_ActualSpeed = L_Pid.compute(L_RequestSpeed, L_ActualSpeed);
  }
// Right Wheel
 if (R_RequestSpeed != R_ActualSpeed)
  {
   ++skip;
   R_ActualSpeed = R_Pid.compute(R_RequestSpeed, R_ActualSpeed);
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
// Set direction
   (L_ActualSpeed >= 0)? pin_low(L_WheelDirPin) : pin_high(L_WheelDirPin);
   (R_ActualSpeed >= 0)? pin_high(R_WheelDirPin) : pin_low(R_WheelDirPin);
// Update prescaler & pulses
   cli(); // ISR OFF
   L_Prescaler = L_presc;
   L_WheelPulses = L_pulses;
   R_Prescaler = R_presc;
   R_WheelPulses = R_pulses;
// stop interrupt or re enable it
   (!L_ActualSpeed)? stopL_StepperWheel() : restartL_StepperWheel();
   (!R_ActualSpeed)? stopR_StepperWheel() : restartR_StepperWheel();
   SREG = sreg; // ISR ON
   wheelActualSpeed = ((s32)L_ActualSpeed + R_ActualSpeed) / 2;
   RobotStatus.RunForward = (wheelActualSpeed < 0)? 0 : 1;
  }
 return skip;
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
 set_input(R_WheelPulseInPin); // INT3
 set_input(L_WheelPulseInPin); // INT2

 // Setup INT 3 & 2 to receive pulsesin (rotation control)
 EIMSK = 0; // disable ALL external interrupts.
 EICRA = _BV(ISC30) | _BV(ISC20); // 01 = interrupt on any edge
 EIFR = _BV(INTF3) | _BV(INTF2); // clear the int. flag
 EIMSK |= _BV(INT3) | _BV(INT2); // enable interrupt for INT3 & 2

// Setup Timer 3 & 4 Mode 15 fast PWM, OCnA set TOP value, OCnB output.

// PH4 OC4B drive left motor.
 TCCR4A = _BV(COM4B1) | _BV(WGM41) | _BV(WGM40);
 TCCR4B = _BV(WGM43) | _BV(WGM42);
 TCCR4C = 0;
 TIMSK4 = _BV(TOIE4);
 OCR4A = 0xFF;
 OCR4B = 0xE;

// PE4 OC3B drive right motor.
 TCCR3A = _BV(COM3B1) | _BV(WGM31) | _BV(WGM30);
 TCCR3B = _BV(WGM33) | _BV(WGM32);
 TCCR3C = 0;
 TIMSK3 = _BV(TOIE3);
 OCR3A = 0xFF;
 OCR3B = 0xE;
}

void enableStepperWheel()
{
 initDirPid();
 initStepperPid();
 pin_low(L_WheelEnablePin);
 pin_low(R_WheelEnablePin);
 u8 sreg = SREG;
 cli();
 TCNT4 = 0; // Reset timer value
 TCNT3 = 0; // Reset timer value
 SREG = sreg;
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

void disableStepperWheel()
{
 stopL_StepperWheel(); // Prescaler disabled
 stopR_StepperWheel(); // Prescaler disabled
 pin_high(L_WheelEnablePin);
 pin_high(R_WheelEnablePin);
 pin_low(L_WheelPulsePin);
 pin_low(R_WheelPulsePin);
}

ISR(TIMER4_OVF_vect) // left motor
{
 ++L_StepCourse;
 OCR4A = L_WheelPulses;
 TCCR4B = L_Prescaler;
}

ISR(TIMER3_OVF_vect) // right motor
{
 ++R_StepCourse;
 OCR3A = R_WheelPulses;
 TCCR3B = R_Prescaler;
}

ISR(INT3_vect) // L wheel pulse in
{
 ++L_PulsesIn;
}

ISR(INT2_vect) // R wheel pulse in
{
 ++R_PulsesIn;
}

s16 pulsesToDecimeterPerMinute(s16 pulses)
{
 s32 result = (pulses * WHEELDIAMETER * 314 * 6);
 result /= (100 * GEARREDUCTION * STEPPERREV * MICROSTEP);
 return result;
}

s16 decimeterPerMinuteToPulses(s16 dmpm)
{
 s32 result = dmpm * (100 * GEARREDUCTION * STEPPERREV * MICROSTEP);
 result /= (WHEELDIAMETER * 314 * 6);
 return result;
}
