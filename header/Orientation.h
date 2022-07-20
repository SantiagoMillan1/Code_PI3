#include "Arduino.h"
#include <MPU6050_light.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
#include "Wire.h"

void orientation(Adafruit_TSL2591* tsl1, Adafruit_TSL2591* tsl2, MPU6050* mpu, const int* in1, const int* in2, const int* enA);
void MoteurControlGauche(const float* commande_pwm, const int* in1, const int* in2, const int* enA);
void MoteurControlDroite(const float* commande_pwm, const int* in1, const int* in2, const int* enA);
float luxRead(Adafruit_TSL2591* tsl);
void tcaselect_o(uint8_t i);
