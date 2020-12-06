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
int16_t L_RequestSpeed = 0;
int16_t R_RequestSpeed = 0;
// StepperEngine values
int16_t L_ActualSpeed = 0;
int16_t R_ActualSpeed = 0;
int16_t wheelActualSpeed = 0;

uint32_t L_StepCourse = 0;
uint32_t R_StepCourse = 0;

uint8_t  wheelAcceleration = STEPPERACCELDEF;
uint8_t  wheelDeceleration = STEPPERDECELDEF;

static uint16_t L_WheelPulses = 0;
static uint16_t R_WheelPulses = 0;

uint8_t L_Prescaler = 0;
uint8_t R_Prescaler = 0;

static void linearizePulses(uint16_t freq, uint8_t * prescaler, uint16_t * pulses)
{
 if (freq <  FREQ5)
  {
   * prescaler = _BV(WGM33) | _BV(WGM32)  | PRESCALER5;
   * pulses = (uint16_t)(F_CPU / (freq * PRESCVALUE5));
  }
 else if (freq < FREQ4)
  {
   * prescaler = _BV(WGM33) | _BV(WGM32)  | PRESCALER4;
   * pulses = (uint16_t)(F_CPU / (freq *PRESCVALUE4));
  }
 else if (freq < FREQ3)
  {
   * prescaler = _BV(WGM33) | _BV(WGM32)  | PRESCALER3;
   * pulses = (uint16_t)(F_CPU / (freq * PRESCVALUE3));
  }
 else if (freq < FREQ2)
  {
   * prescaler = _BV(WGM33) | _BV(WGM32)  | PRESCALER2;
   * pulses = (uint16_t)(F_CPU / (freq * PRESCVALUE2));
  }
 else
  {
   * prescaler = _BV(WGM33) | _BV(WGM32) | PRESCALER1;
   * pulses = (uint16_t)(F_CPU / (freq * PRESCVALUE1));
  }
}

static int16_t computeWheelAcceleration(int16_t ActualSpeed, int16_t RequestSpeed, int8_t Comp) // Poor acceleration management but should work fine
{
 int16_t ret;
 if (!Comp) // we change direction -> deceleration to 0
  {
   if (ActualSpeed > 0)
    {
     ret = ActualSpeed - wheelDeceleration;
     ret = max(ret, 0);
    }
   else
    {
     ret = ActualSpeed + wheelDeceleration;
     ret = min(ret, 0);
    }
  }
 else if (RequestSpeed > ActualSpeed) // Accelerate in +to+ , decelerate in -to-
  {
   ret = (Comp > 0)?/* +to+ */ min(ActualSpeed + wheelAcceleration, RequestSpeed) : min(ActualSpeed + wheelDeceleration, RequestSpeed);
  }
 else // Decelerate in +to+ , accelerate in -to-
  {
   ret = (Comp > 0)?/* +to+ */ max(ActualSpeed - wheelDeceleration, RequestSpeed) : max(ActualSpeed - wheelAcceleration, RequestSpeed);
  }
 return ret;
}

uint8_t computeStepperWheelSpeed() // Must be called at little interval, return 0 if final speed is set
{
 uint8_t skip = 0;

// Left Wheel
 if (L_RequestSpeed != L_ActualSpeed)
  {
   ++skip;
   L_RequestSpeed = limit<int16_t>(-MAXSTEPPERSPEED, L_RequestSpeed, MAXSTEPPERSPEED);
   int8_t L_Comp = sgn(L_RequestSpeed) + sgn(L_ActualSpeed); // == 0 if we will change direction
   L_ActualSpeed = computeWheelAcceleration(L_ActualSpeed, L_RequestSpeed, L_Comp);
  }
// Right Wheel
 if (R_RequestSpeed != R_ActualSpeed)
  {
   ++skip;
   R_RequestSpeed = limit<int16_t>(-MAXSTEPPERSPEED, R_RequestSpeed, MAXSTEPPERSPEED);
   int8_t R_Comp = sgn(R_RequestSpeed) + sgn(R_ActualSpeed); // == 0 if we will change direction
   R_ActualSpeed = computeWheelAcceleration(R_ActualSpeed, R_RequestSpeed, R_Comp);
  }

 if (skip)
  {
// Choose prescaler & Compute Timer pulses
   uint8_t  L_presc;
   uint16_t L_pulses;
   linearizePulses(abs(L_ActualSpeed), &L_presc, &L_pulses);

   uint8_t  R_presc;
   uint16_t R_pulses;
   linearizePulses(abs(R_ActualSpeed), &R_presc, &R_pulses);

   uint8_t sreg = SREG; // Set speed in atomic mode
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
   wheelActualSpeed = ((int32_t)L_ActualSpeed + R_ActualSpeed) / 2;
   RobotStatus.RunForward = (wheelActualSpeed < 0)? 0 : 1;
  }
 return skip;
}

uint8_t computeStepperWheelDirection(int16_t speed, int16_t turn)
{
// espected values
 int16_t Speed = speed; // needed using constant values
 int16_t Turn = turn;
// limit
 Speed = limit<int16_t>((-MAXROBOTSPEED), Speed, MAXROBOTSPEED);
 Turn = limit<int16_t>((-MAXROBOTTURN), Turn, MAXROBOTTURN);
 int8_t direction = sgn(Speed);
 int16_t Wl = Speed + (Turn * direction);
 int16_t Wr = Speed - (Turn * direction);
// write value
 L_RequestSpeed = Wl;
 R_RequestSpeed = Wr;
 return computeStepperWheelSpeed();
}

void initStepperWeel()
{
 set_output_on(L_WheelEnablePin);
 set_output_off(R_WheelEnablePin);
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
 pin_low(L_WheelEnablePin);
 pin_low(R_WheelEnablePin);
 uint8_t sreg = SREG;
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


