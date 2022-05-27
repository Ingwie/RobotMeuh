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

#include "FusionImu.h"

FusionBias fusionBias;
FusionAhrs fusionAhrs;
#define FUSIONPERIOD 0.032f // sample period in seconde

// gyroscope
#define gyroscopeSensitivity    {GYRO_RATE_XYZ, GYRO_RATE_XYZ, GYRO_RATE_XYZ} // sensitivity in degrees per second per lsb
#define  uncalibratedGyroscope  {(float)imuGyro.x, (float)imuGyro.y, (float)imuGyro.z}
FusionVector3 calibratedGyroscope;

// accelerometer
#define accelerometerSensitivity  {ACC_RATE_XYZ, ACC_RATE_XYZ, ACC_RATE_XYZ} // Sensitivity in g per lsb
#define uncalibratedAccelerometer {(float)imuAcc.x, (float)imuAcc.y, (float)imuAcc.z}
FusionVector3 calibratedAccelerometer;

// magnetometer
#define hardIronBias               {0.0f, 0.0f, 0.0f} //  bias in uT
#define  uncalibratedMagnetometer  {(float)imuMag.x*MAG_RATE_XYZ, (float)imuMag.y*MAG_RATE_XYZ, (float)imuMag.z*MAG_RATE_XYZ} // measurement in uT
FusionVector3 calibratedMagnetometer;

// Euler angles
FusionEulerAngles eulerAngles;

// Compas
float fusionHeading;

void initFusionImu()
{
// Initialise gyroscope bias correction algorithm
 FusionBiasInitialise(&fusionBias, 0.5f, FUSIONPERIOD); // stationary threshold = 0.5 degrees per second
// Initialise AHRS algorithm
 FusionAhrsInitialise(&fusionAhrs, 0.7f); // set gain low gain use less Acc and Mag and then can drift
// Set optional magnetic field limits
 FusionAhrsSetMagneticField(&fusionAhrs, 20.0f, 70.0f); // valid magnetic field range = 20 uT to 70 uT
}

void computeFusionImu()
{
// Measure
 if (readGyro()) ERR("GYRO HS");
 if (readAcc()) ERR("ACC HS");
 if (readMag()) ERR("MAG HS");

// Calibrate gyroscope
 calibratedGyroscope = FusionCalibrationInertial(uncalibratedGyroscope, FUSION_ROTATION_MATRIX_IDENTITY, gyroscopeSensitivity, FUSION_VECTOR3_ZERO);

// Calibrate accelerometer
 calibratedAccelerometer = FusionCalibrationInertial(uncalibratedAccelerometer, FUSION_ROTATION_MATRIX_IDENTITY, accelerometerSensitivity, FUSION_VECTOR3_ZERO);

// Calibrate magnetometer
 calibratedMagnetometer = FusionCalibrationMagnetic(uncalibratedMagnetometer, FUSION_ROTATION_MATRIX_IDENTITY, hardIronBias);

// Update gyroscope bias correction algorithm
 calibratedGyroscope = FusionBiasUpdate(&fusionBias, calibratedGyroscope);

// Update AHRS algorithm
 FusionAhrsUpdate(&fusionAhrs, calibratedGyroscope, calibratedAccelerometer, calibratedMagnetometer, FUSIONPERIOD);

// Calculate Euler angles
 eulerAngles = FusionQuaternionToEulerAngles(FusionAhrsGetQuaternion(&fusionAhrs));

// Calculate heading
 fusionHeading = FusionCompassCalculateHeading(calibratedAccelerometer, calibratedMagnetometer);

// Convert to s16
 ImuValues.roll = eulerAngles.angle.roll * 10;
 ImuValues.pitch = eulerAngles.angle.pitch * 10;
 ImuValues.yaw = eulerAngles.angle.yaw * 10;
 ImuValues.heading = fusionHeading * 10;
}

