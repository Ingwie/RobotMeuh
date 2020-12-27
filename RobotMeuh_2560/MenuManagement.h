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

#ifndef MENUMANAGEMENT_H_INCLUDED
#define MENUMANAGEMENT_H_INCLUDED

#include "RobotMeuh.h"

// All menus
extern void menuFirst(); // First at boot
extern void menuStatus(); // show status
extern void menuLcdReport(); // show lcd report in binary format
extern void menuDateTime(); // show date time
extern void menuImuFusion(); // Euler datas
extern void menuImuGyro(); // Gyroscope datas
extern void menuImuAcc(); // Accelerometer datas
extern void menuImuMag(); // Magnetometer datas
extern void menuFreeSram(); // show free sram

/////////////////////////////////////////

typedef void (*p_Function)(void);

const p_Function MenuFunctions[] PROGMEM = // Menu function array
{
 menuFirst,
 menuStatus,
 menuLcdReport,
 menuDateTime,
 menuFreeSram,
 menuImuFusion,
 menuImuGyro,
 menuImuAcc,
 menuImuMag,

};

enum menuArray // Image of menu function array
{
 M_FIRST,
 M_STATUS,
 M_LCDREPORT,
 M_DATETIME,
 M_FREESRAM,
 M_IMUFUSION,
 M_IMUGYRO,
 M_IMUACC,
 M_IMUMAG,


 M_MENUNUMBER
};

PACK(struct MenuTarget_t
{
 menuArray PlayPause;
 menuArray Enter;
 menuArray Plus;
 menuArray Minus;
});

#define PMT(a,b,c,d)      (__extension__({const MenuTarget_t __mt PROGMEM = {a,b,c,d}; &__mt;}))
#define PMT_t             const MenuTarget_t *

extern menuArray menuToken;
extern p_Function MenuPointer; // MenuPointer() call menu[menuToken] function

void menuNavigation(PMT_t menuTarget);
void menuCompute();
void forceMenu(menuArray num);

#endif // MENUMANAGEMENT_H_INCLUDED
