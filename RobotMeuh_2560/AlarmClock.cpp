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


#include "AlarmClock.h"

void sheduleNextAlarm()
{
 struct tm * nowTm = localtime(&rtcTime);
// build and sort hour representation. No duration == no alarm
 s16 nowHour = nowTm->tm_hour << 8 | nowTm->tm_min;
 s16 firstAlarmHour = (RobotMeuh.FirstAlarm.duration)? RobotMeuh.FirstAlarm.hour << 8 | RobotMeuh.FirstAlarm.minute : 0;
 s16 secondAlarmHour = (RobotMeuh.FirstAlarm.duration)? RobotMeuh.SecondAlarm.hour << 8 | RobotMeuh.SecondAlarm.minute : 0;
 u8 firstAlarmDayMask = RobotMeuh.FirstAlarm.dayMask;
 u8 secondAlarmDayMask = RobotMeuh.SecondAlarm.dayMask;
// re order alarms
 if (firstAlarmHour > secondAlarmHour)
  {
   u16 temp = firstAlarmHour;
   firstAlarmHour = secondAlarmHour;
   secondAlarmHour = temp;
   temp = firstAlarmDayMask;
   firstAlarmDayMask = secondAlarmDayMask;
   secondAlarmDayMask = temp;
  }
// check this day
 if (DAYISSCHEDULED(nowTm->tm_wday, firstAlarmDayMask) && (firstAlarmHour > nowHour))
  {
   rtcSetAlarm(firstAlarmHour & 0xFF, firstAlarmHour >> 8, nowTm->tm_wday);
  }
 else if (DAYISSCHEDULED(nowTm->tm_wday, secondAlarmDayMask) && (secondAlarmHour > nowHour))
  {
   rtcSetAlarm(secondAlarmHour & 0xFF, secondAlarmHour >> 8, nowTm->tm_wday);
  }
// now find first next day
 else
  {
   u8 wday = nowTm->tm_wday;
   for (u8 i=0; i<7; ++i)
    {
     if ((wday >>= 1) > _BV(6)) wday = 0; // don't overflow and roll saturday to sunday (6 to 0)
     if (DAYISSCHEDULED(wday, firstAlarmDayMask) && (firstAlarmHour > nowHour))
      {
       rtcSetAlarm(firstAlarmHour & 0xFF, firstAlarmHour >> 8, nowTm->tm_wday);
       break;
      }
     else if (DAYISSCHEDULED(wday, secondAlarmDayMask) && (secondAlarmHour > nowHour))
      {
       rtcSetAlarm(secondAlarmHour & 0xFF, secondAlarmHour >> 8, nowTm->tm_wday);
       break;
      }
    }
  }
}
