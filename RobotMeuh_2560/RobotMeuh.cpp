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
char SpiBuf[SPI_BUFFER_LENGHT] = {SPI_EOT};
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
 if (readGyro()) ERR("GYRO HS");
 if (readAcc()) ERR("ACC HS");
 if (readMag()) ERR("MAG HS");

// Calibrate gyroscope
 FusionVector3 uncalibratedGyroscope = {(float)gyro.x, (float)gyro.y, (float)gyro.z};
 FusionVector3 calibratedGyroscope = FusionCalibrationInertial(uncalibratedGyroscope, FUSION_ROTATION_MATRIX_IDENTITY, gyroscopeSensitivity, FUSION_VECTOR3_ZERO);

// Calibrate accelerometer
 FusionVector3 uncalibratedAccelerometer = {(float)acc.x, (float)acc.y, (float)acc.z};
 FusionVector3 calibratedAccelerometer = FusionCalibrationInertial(uncalibratedAccelerometer, FUSION_ROTATION_MATRIX_IDENTITY, accelerometerSensitivity, FUSION_VECTOR3_ZERO);

// Calibrate magnetometer
 FusionVector3 uncalibratedMagnetometer =  {(float)mag.x/13.7, (float)mag.y/13.7, (float)mag.z/13.7}; // measurement in uT (/13.7)
 FusionVector3 calibratedMagnetometer = FusionCalibrationMagnetic(uncalibratedMagnetometer, FUSION_ROTATION_MATRIX_IDENTITY, hardIronBias);

// Update gyroscope bias correction algorithm
 calibratedGyroscope = FusionBiasUpdate(&fusionBias, calibratedGyroscope);

// Update AHRS algorithm
 FusionAhrsUpdate(&fusionAhrs, calibratedGyroscope, calibratedAccelerometer, calibratedMagnetometer, samplePeriod);



// Print Euler angles
//FusionEulerAngles eulerAngles = FusionQuaternionToEulerAngles(FusionAhrsGetQuaternion(&fusionAhrs));
//printf("Roll = %0.1f, Pitch = %0.1f, Yaw = %0.1f\r\n", eulerAngles.angle.roll, eulerAngles.angle.pitch, eulerAngles.angle.yaw);
// Calculate heading
//float heading = FusionCompassCalculateHeading(calibratedAccelerometer, calibratedMagnetometer);
}

void initRobotMeuh()
{
 i2c_init();
 initSpiMasterMode();
 initStepperWeel();
 rtcInit();
}

int main(void)
{

 initRobotMeuh();

 do
  {
   _delay_ms(800);
   lcdClear();
   _delay_ms(800);
   lcdPrintString(1, 1, "test ... OK");
   _delay_ms(800);
   lcdLedOff();
   _delay_ms(800);
   lcdPrintString(1, 5, "OUI ... OK ");
   _delay_ms(800);
   lcdLedOn();
   _delay_ms(800);
   lcdPrintString(2, 2, "NON ... OK ");

  }
 while(1)
  ;

 return 0;
}

