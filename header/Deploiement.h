#include "Arduino.h"
#include <MPU6050_light.h>



bool deploiement(const int *angle_voulu, MPU6050 *mpu, const int *in1, const int *in2, const int *enA);

void MoteurControlGauche(const byte *commande_pwm, const int* in1, const int* in2, const int* enA);

void MoteurControlDroite(const byte *commande_pwm, const int* in1, const int* in2, const int* enA);

