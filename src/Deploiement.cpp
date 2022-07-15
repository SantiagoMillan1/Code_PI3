#include "Deploiement.h"


//-------------------------------Deploiement-------------------------------
bool deploiement(const int* angle_voulu, MPU6050* mpu, const int* in1, const int* in2, const int* enA) {
    mpu->update();
    unsigned long timer = millis();
    int erreur = *angle_voulu - mpu->getAngleX();
    int precision = 1;
    byte commande_pwm = 70;

    while (abs(erreur) > precision) {
        if (erreur > 0)
            MoteurControlGauche(&commande_pwm, in1, in2, enA);
        else
            MoteurControlDroite(&commande_pwm, in1, in2, enA);
        mpu->update();
        erreur = *angle_voulu - mpu->getAngleX();
        Serial.print("X : ");
        Serial.println(mpu->getAngleX());
        Serial.print("Erreur : ");
        Serial.println(erreur);

        if ((millis() - timer) > 20000) // si apres 20 sec fonctionne pas abort
            return false;
    }
    return true;
}

void MoteurControlGauche(const byte *commande_pwm, const int* in1, const int* in2, const int* enA) {
    // Set vitesse du moteur
    analogWrite(*enA, *commande_pwm);
   // analogWrite(enB, commande_pwm);

    // Turn on motor A 
    digitalWrite(*in1, HIGH);
    digitalWrite(*in2, LOW);
    //digitalWrite(in3, HIGH);
   // digitalWrite(in4, LOW);
    delay(40);

    // Turn off motors
    digitalWrite(*in1, LOW);
    digitalWrite(*in2, LOW);
   // digitalWrite(in3, LOW);
   // digitalWrite(in4, LOW);
}

void MoteurControlDroite(const byte *commande_pwm, const int* in1, const int* in2, const int* enA) {
    // Set motors to maximum speed
    // For PWM maximum possible values are 0 to 255
    analogWrite(*enA, *commande_pwm);
   // analogWrite(enB, commande_pwm);

    // Now change motor directions
    digitalWrite(*in1, LOW);
    digitalWrite(*in2, HIGH);
   // digitalWrite(in3, LOW);
   // digitalWrite(in4, HIGH);
    delay(40);

    // Turn off motors
    digitalWrite(*in1, LOW);
    digitalWrite(*in2, LOW);
   // digitalWrite(in3, LOW);
   // digitalWrite(in4, LOW);
}
