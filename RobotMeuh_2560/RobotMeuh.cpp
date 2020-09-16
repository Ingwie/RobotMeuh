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


#include "RobotMeuh.h"

//ROBOTMEUH
Status_t RobotStatus = {0};
DataLcdToMain_t Report = {0};

//SPI (LCD com.)
uint8_t SpiRet = 0;
volatile char SpiBuf[SPI_BUFFER_LENGHT] = {SPI_EOT};
volatile uint8_t SpiBufNum = 0;

//TIME
time_t rtcTime;

//IMU
FusionBias fusionBias;
FusionAhrs fusionAhrs;
float samplePeriod = 0.01f; // sample period in seconds
FusionVector3 gyroscopeSensitivity = {GYRO_RATE_XYZ, GYRO_RATE_XYZ, GYRO_RATE_XYZ};// sensitivity in degrees per second per lsb
FusionVector3 accelerometerSensitivity = {ACC_RATE_XYZ, ACC_RATE_XYZ, ACC_RATE_XYZ}; // Sensitivity in g per lsb
FusionVector3 hardIronBias = {0.0f, 0.0f, 0.0f}; //  bias in uT


void initFusionImu()
{
// Initialise gyroscope bias correction algorithm
 FusionBiasInitialise(&fusionBias, 0.5f, samplePeriod); // stationary threshold = 0.5 degrees per second

// Initialise AHRS algorithm
 FusionAhrsInitialise(&fusionAhrs, 0.5f); // gain = 0.5

// Set optional magnetic field limits
 FusionAhrsSetMagneticField(&fusionAhrs, 20.0f, 70.0f); // valid magnetic field range = 20 uT to 70 uT
}

void computeFusionImu()
{

// Measure


// Calibrate gyroscope
 FusionVector3 uncalibratedGyroscope =
 {
  0.0f, //x /* replace this value with actual gyroscope x axis measurement in lsb */
  0.0f, //y /* replace this value with actual gyroscope y axis measurement in lsb */
  0.0f, //z /* replace this value with actual gyroscope z axis measurement in lsb */
 };
 FusionVector3 calibratedGyroscope = FusionCalibrationInertial(uncalibratedGyroscope, FUSION_ROTATION_MATRIX_IDENTITY, gyroscopeSensitivity, FUSION_VECTOR3_ZERO);

// Calibrate accelerometer
 FusionVector3 uncalibratedAccelerometer =
 {
  0.0f, //x /* replace this value with actual accelerometer x axis measurement in lsb */
  0.0f, //y /* replace this value with actual accelerometer y axis measurement in lsb */
  1.0f, //z /* replace this value with actual accelerometer z axis measurement in lsb */
 };
 FusionVector3 calibratedAccelerometer = FusionCalibrationInertial(uncalibratedAccelerometer, FUSION_ROTATION_MATRIX_IDENTITY, accelerometerSensitivity, FUSION_VECTOR3_ZERO);

// Calibrate magnetometer
 FusionVector3 uncalibratedMagnetometer =
 {
  0.5f, //x /* replace this value with actual magnetometer x axis measurement in uT */
  0.0f, //y /* replace this value with actual magnetometer y axis measurement in uT */
  0.0f, //z /* replace this value with actual magnetometer z axis measurement in uT */
 }; // measurement in uT
 FusionVector3 calibratedMagnetometer = FusionCalibrationMagnetic(uncalibratedMagnetometer, FUSION_ROTATION_MATRIX_IDENTITY, hardIronBias);

// Update gyroscope bias correction algorithm
 calibratedGyroscope = FusionBiasUpdate(&fusionBias, calibratedGyroscope);

// Update AHRS algorithm
 FusionAhrsUpdate(&fusionAhrs, calibratedGyroscope, calibratedAccelerometer, calibratedMagnetometer, samplePeriod);

// Print Euler angles
 FusionEulerAngles eulerAngles = FusionQuaternionToEulerAngles(FusionAhrsGetQuaternion(&fusionAhrs));
//printf("Roll = %0.1f, Pitch = %0.1f, Yaw = %0.1f\r\n", eulerAngles.angle.roll, eulerAngles.angle.pitch, eulerAngles.angle.yaw);
// Calculate heading
 float heading = FusionCompassCalculateHeading(calibratedAccelerometer, calibratedMagnetometer);

}

int main(void)
{

// Insert code

 while(1)
  ;

 return 0;
}

