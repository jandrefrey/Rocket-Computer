/**
 *******************************************************************************
 * \file        sensors.cpp
 * \brief       Sensors library source file
 * \date        2024/07/29
 * \author      Jandre Frey
 *******************************************************************************
 */

/*** Includes ***/
#include "sensors.h"

/*** Constructor implementation ***/
Sensors::Sensors()
{
    // Initialize constructor logic here
}

/*** Destructor implementation ***/
Sensors::~Sensors()
{
    // Cleanup resources if any
}

/*** Public Functions definitions ***/
int Sensors::init() {
    Serial.begin(9600);
    lsm32Init();
    bmp390Init();
    mmc598Init();

    return 0;
}

int Sensors::calib() {
    return 0;
}

int Sensors::measure() {

    //Accel and Gyro
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    dso32.getEvent(&accel, &gyro, &temp);
    float accelx = accel.acceleration.x;        //acceleration is measured in m/s^2
    float gyrox = gyro.gyro.x;                 //rotation is measured in rad/s
    Serial.println("LSM32 Measured");

    //Baro
    if (bmp.performReading()) {
        float pressure = bmp.pressure / 100.0;      //measured in hPa
        float altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    }

    //Magnetometer
    uint32_t currentX = 0;
    uint32_t currentY = 0;
    uint32_t currentZ = 0;
    double scaledX = 0;
    double scaledY = 0;
    double scaledZ = 0;
    myMag.getMeasurementXYZ(&currentX, &currentY, &currentZ);
    scaledX = (double)currentX - 131072.0;
    scaledX /= 131072.0;
    scaledY = (double)currentY - 131072.0;
    scaledY /= 131072.0;
    scaledZ = (double)currentZ - 131072.0;
    scaledZ /= 131072.0;

    //We can take a time measurement here, depending on how long all the measurements take to execute.
    //Then we gotta pack this into a buffer? Or maybe just a public variable that some upper (app) layer can use.

    return 0;
}

int Sensors::filter() {
    return 0;
}


/*** Private Functions definitions ***/
int Sensors::lsm32Init() {
    if (!dso32.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
        Serial.println("LSM32 Error");
    }

    dso32.setAccelRange(LSM6DSO32_ACCEL_RANGE_32_G);
    dso32.setGyroRange(LSM6DS_GYRO_RANGE_1000_DPS);

    dso32.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
    dso32.setGyroDataRate(LSM6DS_RATE_12_5_HZ);

    Serial.println("LSM6DSO32 init done");

    return 0;
}

int Sensors::bmp390Init() {
    if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
        Serial.println("BMP390 Error");
    }

    // Set up oversampling and filter initialization
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    bmp.setOutputDataRate(BMP3_ODR_200_HZ);
}

int Sensors::mmc598Init() {
    if( !myMag.begin(MMC_CS)) {
        Serial.println("MMC5983MA Error");
    }

    myMag.softReset();
}

Sensors m_sensors;