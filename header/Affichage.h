#include "Arduino.h"
#include <MPU6050_light.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"


int DisplayMenu();

void DisplayMpu(MPU6050* mpu);

void simpleRead(Adafruit_TSL2591* tsl);

/**************************************************************************/
/*
    Show how to read IR and Full Spectrum at once and convert to lux
*/
/**************************************************************************/
void advancedRead(Adafruit_TSL2591* tsl);


/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(Adafruit_TSL2591* tsl);

