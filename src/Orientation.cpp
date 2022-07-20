#include "Orientation.h"

// Addresse (registre) du multiplexeur
#define TCAADDR 0x70


//-------------------------------Orientation-------------------------------
void orientation(Adafruit_TSL2591* tsl1, Adafruit_TSL2591* tsl2, MPU6050* mpu, const int* in1, const int* in2, const int* enA) {
    bool orientated = true;
    float kp = 7, ki = 15, kd = .5, N = 200, T =0.01;
    double commande = 45;
    unsigned long currentTime = 0, previousTime = 0;
    double elapsedTime;
    double error;
    double lastError;
    float commande_pwm;
    double input, output, setPoint;
    double cumError, rateError;

    while (orientated) {
        tcaselect_o(7);
        float tsl1_lux = luxRead(tsl1);
        tcaselect_o(4);
        float tsl2_lux = luxRead(tsl2);
        float precision = 5;
        float erreur_lux = tsl1_lux - tsl2_lux;
        mpu->update();
        
        currentTime = millis();                //get current time
        elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
        
        error = commande - mpu->getAngleX();                                // determine error
        cumError += error * elapsedTime;                // compute integral
        rateError = (error - lastError)/elapsedTime;   // compute derivative
 
        double out = kp*error + ki*cumError + kd*rateError;                //PID output               
        commande_pwm = out*255/7.4;
 
        lastError = error;                                //remember current error
        previousTime = currentTime;                        //remember current time
        
        //erreur = angle_voulu - mpu->getAngleX();

        if (commande_pwm >= 255)
            commande_pwm = 255;
        else if (commande_pwm <= -255)
            commande_pwm = -255;


        if (commande_pwm > 0) {
            MoteurControlGauche(&commande_pwm, in1, in2, enA);
        }
        else if (commande_pwm < 0) {
            MoteurControlDroite(&commande_pwm, in1, in2, enA);
        }
        if (error < 3) {
            orientated = false;
        }

    //    else if (erreur_lux < precision) {
    //        MoteurControlDroite(&commande_pwm, in1, in2, enA);
    //    }

    //    else
    //        orientateted = false;
    }


}



void MoteurControlGauche(const float* commande_pwm, const int* in1, const int* in2, const int* enA) {
    // Set vitesse du moteur
    analogWrite(*enA, *commande_pwm);

     // Turn on motor A 
    digitalWrite(*in1, HIGH);
    digitalWrite(*in2, LOW);
    delay(40);

    // Turn off motors
    digitalWrite(*in1, LOW);
    digitalWrite(*in2, LOW);
}

void MoteurControlDroite(const float* commande_pwm, const int* in1, const int* in2, const int* enA) {
    // Set motors to maximum speed
    // For PWM maximum possible values are 0 to 255
    analogWrite(*enA, *commande_pwm);

     // Now change motor directions
    digitalWrite(*in1, LOW);
    digitalWrite(*in2, HIGH);
    delay(40);

    // Turn off motors
    digitalWrite(*in1, LOW);
    digitalWrite(*in2, LOW);
}


float luxRead(Adafruit_TSL2591* tsl)
{
    // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
    // That way you can do whatever math and comparisons you want!
    uint32_t lum = tsl->getFullLuminosity();
    uint16_t ir, full;
    ir = lum >> 16;
    full = lum & 0xFFFF;
    return tsl->calculateLux(full, ir);
}

void tcaselect_o(uint8_t i) {
    if (i > 7) return;

    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}
