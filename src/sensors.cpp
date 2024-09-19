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

IntervalTimer sensorTimer;

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
    lsm32Init();
    bmp390Init();
    mmc598Init();

    //sensorTimer.begin(Sensors::getMeasures, SENSOR_READ_RATE);      //This causes problems with the buzzer timer, 
    //possibly since the interrupt function is so large. Not really sure if there is a solution here.

    //sensorTimer.priority(255);

    return 0;
}

int Sensors::calib() {
    while (!bmp.performReading()) {
        //do nothing
    }
    mymeasurements.bpresurre_filtered_prev = bmp.pressure / 100.0;      //set the filtered pressure to the init pressure. Else it will be 0.
    return 0;
}

int Sensors::measure() {

    //uint32_t time = micros();

    //Accel and Gyro
    if(dso32.accelerationAvailable() && dso32.gyroscopeAvailable()) {
        dso32.getEvent(&accel, &gyro, &temp);
    }
    mymeasurements.accel[0] = accel.acceleration.x;  //acceleration is measured in m/s^2
    mymeasurements.accel[1] = accel.acceleration.y;
    mymeasurements.accel[2] = accel.acceleration.z;
    mymeasurements.gyro[0] = gyro.gyro.x;            //rotation is measured in rad/s
    mymeasurements.gyro[1] = gyro.gyro.y;
    mymeasurements.gyro[2] = gyro.gyro.z;

    //Baro   
    if (bmp.performReading()) {
        mymeasurements.bpressure = bmp.pressure / 100.0;      //measured in hPa
        //Serial.println(mymeasurements.bpressure);
        //mymeasurements.alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    }
    filter();

    //Magnetometer
    if (myMag.isDataReady())
    {
        currentX = 0;
        currentY = 0;
        currentZ = 0;
        scaledX = 0;
        scaledY = 0;
        scaledZ = 0;
        myMag.readFieldsXYZ(&currentX, &currentY, &currentZ);
        myMag.startMeasurement();
    }
    scaledX = (double)currentX - 131072.0;
    scaledX /= 131072.0;
    mymeasurements.mag[0] = (float)scaledX * 8; // The magnetometer full scale is +/- 8 Gauss. Multiply the scaled values by 8 to convert to Gauss
    //Serial.println(m_sensors.mymeasurements.mag[0]);
    scaledY = (double)currentY - 131072.0;
    scaledY /= 131072.0;
    mymeasurements.mag[1] = (float)scaledY * 8;
    //Serial.println(m_sensors.mymeasurements.mag[1]);
    scaledZ = (double)currentZ - 131072.0;
    scaledZ /= 131072.0;
    mymeasurements.mag[2] = (float)scaledZ * 8;

    //time
    mymeasurements.time = millis();

    //Serial.println(micros() - time);           //takes about 350us
    return 0;
}

void Sensors::filter() {
    // mymeasurements.bpresurre_filtered = mymeasurements.bpresurre_filtered_prev + ((mymeasurements.bpressure-mymeasurements.bpresurre_filtered_prev)/32);
    mymeasurements.bpresurre_filtered = mymeasurements.bpresurre_filtered_prev + ((mymeasurements.bpressure-mymeasurements.bpresurre_filtered_prev)/128);
    mymeasurements.bpresurre_filtered_prev = mymeasurements.bpresurre_filtered;
}


/*** Private Functions definitions ***/
int Sensors::lsm32Init() {
    if (!dso32.begin_SPI(LSM_CS)) {
        Serial.println("LSM32 Error");
    } else {
        dso32.setAccelRange(LSM6DSO32_ACCEL_RANGE_32_G);
        dso32.setGyroRange(LSM6DS_GYRO_RANGE_1000_DPS);

        dso32.setAccelDataRate(LSM6DS_RATE_416_HZ);
        dso32.setGyroDataRate(LSM6DS_RATE_416_HZ);

        Serial.println("LSM6DSO32 init done");
    }
    return 0;
}

int Sensors::bmp390Init() {
    if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
        Serial.println("BMP390 Error");
    } else {
        bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
        bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
        bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_15);
        bmp.setOutputDataRate(BMP3_ODR_200_HZ);

        bmp.setSensorSettings();

        Serial.println("BMP390 init done");
    }

    return 0;
}

int Sensors::mmc598Init() {
    if( !myMag.begin(MMC_CS)) {
        Serial.println("MMC5983MA Error");
    }
    else {
        myMag.softReset();
        myMag.startMeasurement();

        Serial.println("MMC598 init done");
    }

    return 0;
}

// int Sensors::integrateMeasures() {
//     _time_new = mymeasurements.time;

//     myTelemetry.attitude[0] = (_gyro_old[0] + mymeasurements.gyro[0])/2 * (_time_new - _time_old);  //have to remove gravity
//     myTelemetry.attitude[1] = (_gyro_old[1] + mymeasurements.gyro[1])/2 * (_time_new - _time_old);
//     myTelemetry.attitude[2] = (_gyro_old[2] + mymeasurements.gyro[2])/2 * (_time_new - _time_old);

//     _gyro_old[0] = mymeasurements.gyro[0];
//     _gyro_old[1] = mymeasurements.gyro[1];
//     _gyro_old[2] = mymeasurements.gyro[2];

//     myTelemetry.velocity[0] = (_accel_old[0] + mymeasurements.accel[0])/2 * (_time_new - _time_old);
//     myTelemetry.velocity[1] = (_accel_old[1] + mymeasurements.accel[1])/2 * (_time_new - _time_old);
//     myTelemetry.velocity[2] = (_accel_old[2] + mymeasurements.accel[2])/2 * (_time_new - _time_old);

//     _accel_old[0] = mymeasurements.accel[0];
//     _accel_old[1] = mymeasurements.accel[1];
//     _accel_old[2] = mymeasurements.accel[2];

//     myTelemetry.position[0] = (_vel_old[0] + myTelemetry.velocity[0])/2 * (_time_new - _time_old);
//     myTelemetry.position[1] = (_vel_old[1] + myTelemetry.velocity[1])/2 * (_time_new - _time_old);
//     myTelemetry.position[2] = (_vel_old[2] + myTelemetry.velocity[2])/2 * (_time_new - _time_old);    

//     _vel_old[0] = myTelemetry.velocity[0];
//     _vel_old[1] = myTelemetry.velocity[1];
//     _vel_old[2] = myTelemetry.velocity[2];

//     _time_old = mymeasurements.time;
// }

void Sensors::getMeasures() {

    //uint32_t time = micros();

    //Accel and Gyro
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    if(m_sensors.dso32.accelerationAvailable() && m_sensors.dso32.gyroscopeAvailable()) {
        m_sensors.dso32.getEvent(&accel, &gyro, &temp);
    }
    m_sensors.mymeasurements.accel[0] = accel.acceleration.x;  //acceleration is measured in m/s^2
    m_sensors.mymeasurements.accel[1] = accel.acceleration.y;
    m_sensors.mymeasurements.accel[2] = accel.acceleration.z;
    m_sensors.mymeasurements.gyro[0] = gyro.gyro.x;            //rotation is measured in rad/s
    m_sensors.mymeasurements.gyro[1] = gyro.gyro.y;
    m_sensors.mymeasurements.gyro[2] = gyro.gyro.z;

    //Baro   
    if (m_sensors.bmp.performReading()) {
        m_sensors.mymeasurements.bpressure = m_sensors.bmp.pressure / 100.0;      //measured in hPa
        //mymeasurements.alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    }

    //Magnetometer
    uint32_t currentX = 0;
    uint32_t currentY = 0;
    uint32_t currentZ = 0;
    double scaledX = 0;
    double scaledY = 0;
    double scaledZ = 0;
    if (m_sensors.myMag.isDataReady())
    {
        m_sensors.myMag.readFieldsXYZ(&currentX, &currentY, &currentZ);
        m_sensors.myMag.startMeasurement();
    }
    scaledX = (double)currentX - 131072.0;
    scaledX /= 131072.0;
    m_sensors.mymeasurements.mag[0] = scaledX * 8; // The magnetometer full scale is +/- 8 Gauss. Multiply the scaled values by 8 to convert to Gauss
    //Serial.println(m_sensors.mymeasurements.mag[0]);
    scaledY = (double)currentY - 131072.0;
    scaledY /= 131072.0;
    m_sensors.mymeasurements.mag[1] = scaledY * 8;
    //Serial.println(m_sensors.mymeasurements.mag[1]);
    scaledZ = (double)currentZ - 131072.0;
    scaledZ /= 131072.0;
    m_sensors.mymeasurements.mag[2] = scaledZ * 8;
    //Serial.println(m_sensors.mymeasurements.mag[2]);

    //time
    m_sensors.mymeasurements.time = millis();

    //Serial.println(micros() - time);           //takes about 350us
}

Sensors m_sensors;