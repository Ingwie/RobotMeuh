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
volatile uint32_t bladeTick;  // hall sensor tick value (62.5nS tick)
volatile uint8_t slowBladeTick; // hall sensor overflow tick value (625uS)

void initBrushlessBlade()
{
 set_output_on(BladeEnablePin);
 set_output_off(BladeClockwisePin);
 set_output_off(BladePwmOutPin);
 set_input(BladePulsesinPin);
 memset(&BrushlessBlade, 0, sizeof(BrushlessBlade)); // Reset all fields

// Setup Timer 5 Mode 15 fast PWM, OCnA set TOP value, OCnC output & ICR5 capture input.
 TCCR5A = _BV(COM5C1) | _BV(COM5B0) | _BV(WGM51) | _BV(WGM50);
// Input rising edge & noise canceler on, prescaler 1
 TCCR5B = _BV(ICNC5) | _BV(ICES5) | _BV(WGM53) | _BV(WGM52); // | _BV(CS50) -> do not run yet ...
 TCCR5C = 0;
 TIMSK5 = _BV(ICIE5) | _BV(TOIE5); // Active capture and top ISR (TODO need top ?)
 OCR5A = 10000; // 1.6 Khz 625uS (62.5nS tick)
 OCR5C = 0; // initial value (off)
 ICR5 = 0; // reset input counter
}

void BrushlessBladeStop()
{
 set_output_on(BladeEnablePin); // Stop !
 OCR5C = 0; // off
 TCCR5B &= ~_BV(CS50); // Disable timer 5
 BrushlessBlade.IsRunnig = false;
 BrushlessBlade.PWMValue = 0;
}

void BrushlessBladeCutAt(int16_t speed)
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
//limit<int16_t>(-9999, speed, 9999); // 10000 stop output (Todo : test)
 OCR5C = abs(speed); // Set PPM average
 TCCR5B |= _BV(CS50); // Enable timer 5
 set_output_off(BladeEnablePin); // Cut !
 BrushlessBlade.IsRunnig = true;
 BrushlessBlade.PWMValue = speed;
}

void BrushlessBladeUpdateRPM()
{
// One tick is 62.5nS, 3 tick per turn.
// -> One tick per ISR ~ 5333333 RPSec
 if (bladeTick)
  {
   uint32_t val = 5333333UL / bladeTick;
   val /= 60;
   val = ((BrushlessBlade.RPM << 2) + val) / 5; // Poor low pass filter
   BrushlessBlade.RPM = val;
  }
  else
  {
   BrushlessBlade.RPM = 0;
  }
}

ISR(TIMER5_CAPT_vect, ISR_BLOCK) // Hall sensor detected (3 per turn on 42BLS03 with JY01 controler ?)
{
 static uint16_t lastTick = 0;

 uint16_t NewTick = ICR5;
 if (slowBladeTick != 0xFF)
  {
   if (NewTick > lastTick)
    {
     bladeTick = NewTick - lastTick;
    }
   else
    {
     bladeTick = (0xFFFF - lastTick) + NewTick;
    }
   lastTick = NewTick;
   bladeTick += (uint32_t)(slowBladeTick * 1000);
  }
 else
  {
   bladeTick = 0; //reset
  }
 slowBladeTick = 0;
}

ISR(TIMER5_OVF_vect)
{
 if (slowBladeTick != 0xFF) // 1.6Khz/255 = 6.27Hz min -> 125.5 RPM min
  {
   ++slowBladeTick;
  }
}
