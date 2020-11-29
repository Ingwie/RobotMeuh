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


#include "rtc.h"

///// PCF8563T Code /////

#define RTC_ADRESS  (0x51 << 1) // PCF8563T

void initRTC()
{
 /* Read RTC registers */
 uint8_t powerlost;

 if (i2c_readReg(RTC_ADRESS, 0x02, &powerlost, 1)) // Read VL bit in secondes register
  {
   lcdDispOn();
   ERR("RTC HS");
   return;	/* Check data corruption and return on IIC error */
  }

 if (powerlost >> 7)  	/* When data has been volatiled, set default time */
  {
   uint8_t buf[7];	/* RTC buffer */
   /* Reset time to  01 01 2021. (I can't stand 2020 !) Reg[2..8] */
   buf[0] = 0x00;
   buf[1] = 0x00;
   buf[2] = 0x00;
   buf[3] = 0x01;
   buf[4] = 0x01;
   buf[5] = 0x01;
   buf[6] = 0x21;
   i2c_writeReg(RTC_ADRESS, 0x02, buf, 7);
   ERR("PILE RTC HS");
  }

 struct tm utm;
 rtcReadTime(&utm);
 rtcTime = mk_gmtime(&utm);
}

void rtcSetTime(struct tm * t)
{
 uint8_t buf[7];

 buf[0] = bin2bcd(t->tm_sec);         // 0 - 59 from seconds after the minute - [ 0 from 59 ]
 buf[1] = bin2bcd(t->tm_min);         // 0 - 59 from minutes after the hour - [ 0 from 59 ]
 buf[2] = bin2bcd(t->tm_hour);        // 0 - 23 from hours since midnight - [ 0 from 23 ]
 buf[3] = bin2bcd(t->tm_wday);        // 1 - 31 from day of the month - [ 1 from 31 ]
 buf[4] = bin2bcd(t->tm_mday);        // 0 - 6  from days since Sunday - [ 0 from 6 ]
 buf[5] = bin2bcd(t->tm_mon + 1);     // 1 - 12 from months since January - [ 0 from 11 ]
 buf[6] = bin2bcd(t->tm_year)-100;    // 0 - 99 from years since 1900

 i2c_writeReg(RTC_ADRESS, 0x02, buf, 7);
}

void rtcReadTime(struct tm * t)
{
 uint8_t buf[7];

 if (i2c_readReg(RTC_ADRESS, 0x02, buf, 7))
  {
   t->tm_year = 0; // error detection
   return;
  }
 t->tm_sec =  bcd2bin(buf[0]);             // 0 - 59 to seconds after the minute - [ 0 to 59 ]
 t->tm_min =  bcd2bin(buf[1] & 0x7F);      // 0 - 59 to minutes after the hour - [ 0 to 59 ]
 t->tm_hour = bcd2bin(buf[2] & 0x3F);      // 0 - 23 to hours since midnight - [ 0 to 23 ]
 t->tm_mday = bcd2bin(buf[4] & 0x3F);      // 1 - 31 to day of the month - [ 1 to 31 ]
 t->tm_wday = bcd2bin((buf[3] & 0x07));    // 0 - 6  to days since Sunday - [ 0 to 6 ]
 t->tm_mon =  bcd2bin((buf[5] & 0x1F)-1);  // 1 - 12 to months since January - [ 0 to 11 ]
 t->tm_year = bcd2bin(buf[6]) + 100;       // 0 - 99 to years since 1900
}

void rtcSetAlarm(uint8_t minutes, uint8_t hour, uint8_t wday)
{
#define EnableAlarmField  _BV(7)
// Set time
 uint8_t buf[4];
 buf[0] = bin2bcd(minutes) | EnableAlarmField; // 0 - 59
 buf[1] = bin2bcd(hour) | EnableAlarmField;    // 0 - 23
 buf[2] = 0x00;                                // reset day, use only wday
 buf[3] = bin2bcd(wday) | EnableAlarmField;    // 0 - 6  Alarm when wday, hours, and minutes match
 i2c_writeReg(RTC_ADRESS, 0x09, buf, 4);

// activate ISR
 uint8_t controlStatus2Reg;
 i2c_readReg(RTC_ADRESS, 0x01, &controlStatus2Reg, 1);
 controlStatus2Reg |= 0x02; // AIE activation
 i2c_writeReg(RTC_ADRESS, 0x01, &controlStatus2Reg, 1);
}

void rtcResetAlarm()
{
// desactivate ISR
 uint8_t controlStatus2Reg;
 i2c_readReg(RTC_ADRESS, 0x01, &controlStatus2Reg, 1);
 controlStatus2Reg &= ~0x02; // AIE desactivation
 i2c_writeReg(RTC_ADRESS, 0x01, &controlStatus2Reg, 1);
}
