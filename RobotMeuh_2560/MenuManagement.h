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

#ifndef MENUMANAGEMENT_H_INCLUDED
#define MENUMANAGEMENT_H_INCLUDED

#include "RobotMeuh.h"

// All menus
extern const void menuFirst(); // First at boot
extern const void menuStatus(); // show status
extern const void menuLcdKeys(); // show lcd report in binary format
extern const void menuDateTime(); // show date time
extern const void menuImuFusion(); // Euler datas
extern const void menuImuGyro(); // Gyroscope datas
extern const void menuImuAcc(); // Accelerometer datas
extern const void menuImuMag(); // Magnetometer datas
extern const void menuFreeSram(); // show free sram
extern const void menuWheelsPulses(); // Wheels pulses datas
extern const void menuBladeSpeed(); // Blade speed setting
extern const void menuPidBlade(); // PID blade settings
extern const void menuWheelsSpeed(); // Wheels speed setting
extern const void menuPidWheels(); // PID wheels settings
extern const void menuPidDirection(); // PID direction settings
extern const void menuWheelsRotationRate(); // Wheels rotation rate setting
extern const void menuTemperature(); // show gyro temperature
extern const void menuResetEeprom(); // restore default value
extern const void menuSetFirstAlarm(); // Set firt alarm
extern const void menuSetSecondAlarm(); // Set firt alarm
extern const void menuTestSteppers(); // Test wheels motors and controlers
extern const void menuTestBrushless(); // Test blade motor and controler
extern const void menuSetFirstZone(); // Extra zone 1 settings
extern const void menuSetSecondZone(); // Extra zone 2 settings

/////////////////////////////////////////

const p_Function menuFunctions[] PROGMEM = // Menu function array
{
 menuFirst,
 menuStatus,
 menuLcdKeys,
 menuDateTime,
 menuFreeSram,
 menuImuFusion,
 menuImuGyro,
 menuImuAcc,
 menuImuMag,
 menuWheelsPulses,
 menuBladeSpeed,
 menuPidBlade,
 menuWheelsSpeed,
 menuWheelsRotationRate,
 menuPidWheels,
 menuPidDirection,
 menuTemperature,
 menuResetEeprom,
 menuSetFirstAlarm,
 menuSetSecondAlarm,
 menuTestSteppers,
 menuTestBrushless,
 menuSetFirstZone,
 menuSetSecondZone,

};

enum menuArray // Image of menu function array
{
 M_FIRST,
 M_STATUS,
 M_LCDKEYS,
 M_DATETIME,
 M_FREESRAM,
 M_IMUFUSION,
 M_IMUGYRO,
 M_IMUACC,
 M_IMUMAG,
 M_WHEELSPULSES,
 M_BLADESPEED,
 M_PIDBLADE,
 M_WHEELSSPEED,
 M_WHEELSROTRATE,
 M_PIDWHEELS,
 M_PIDDIR,
 M_TEMPERATURE,
 M_RESETEEPROM,
 M_SETFIRSTALARM,
 M_SETSECONDTALARM,
 M_TESTSTEPPERS,
 M_TESTBRUSHLESS,
 M_SETFIRSTZONE,
 M_SETSECONDZONE,

 M_MENUNUMBER
};

PACK(struct MenuTarget_t
{
 menuArray Play;
 menuArray Enter;
 menuArray Plus;
 menuArray Minus;
});

PACK(struct MenuVar_t
{
 u8 editMode:1;
 u8 wasEdited:1;
 u8 field:4;    // 15 max
 u8 maxField:4; // 15 max
 s8 value:6;    // +-31 max
});

#define PMT(a,b,c,d)      (__extension__({static const MenuTarget_t __mt PROGMEM = {a,b,c,d}; &__mt;}))
#define PMT_t             const MenuTarget_t *

extern menuArray menuToken;
extern p_Function menuPointer; // menuPointer() call menu[menuToken] function
extern MenuVar_t menuVar;

void menuNavigation(PMT_t menuTarget);
s16 setMenuValue(s16 actual, s16 maxi, s16 mini, s16 step);
void menuCompute();
void forceMenu(menuArray num);

#endif // MENUMANAGEMENT_H_INCLUDED
