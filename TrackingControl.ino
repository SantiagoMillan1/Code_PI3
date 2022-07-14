//----------------------------------------------------------------------------------
// Aurteur  : Santiago Millan
// PROJECT : PI3 : Syteme de controle pour le deploiment et orientation des panneaux solaires d'un cubesat

//----------------------------------------------------------------------------------
/* Références : 
 Une partie du code vient de adafruit TSL2591 Digital Light Sensor exemple
 Une partie du code vient de https://github.com/rfetick/MPU6050_light/blob/master/examples/GetAngle/GetAngle.ino
 */

//---------------------------------Libraries téléchargées------------------------------------
// Libreries nécessaires
#include "Wire.h"
#include <MPU6050_light.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

//-----------------------------------Libraries custom--------------------------------------
#include "Deploiement.h"
#include "Orientation.h"
#include "Affichage.h"

// Addresse (registre) du multiplexeur
#define TCAADDR 0x70

//---------------------------Definitions des variables et objects---------------------------
// Definition des objects (capteurs)
MPU6050 mpu(Wire);
Adafruit_TSL2591 tsl = Adafruit_TSL2591(1); 
Adafruit_TSL2591 tsl2 = Adafruit_TSL2591(2);
 
// Definition des variables générales
unsigned long timer = 0;
bool go = true;

// Variables pour le déploiment
int angle_voulu;
//byte commande_pwm = 80;
bool deploiementStatut = false;

//Variables pour l'orientation des panneaux vers le soleil

 
// Pins pour le Motor A (déploiment)
int enA = 3;
int in1 = 2;
int in2 = 4;

// Pins pour le Motor B (orientation)
int enB = 5;
int in3 = 6;
int in4 = 7;

//----------------------------SETUP du programme-----------------------------------
 void setup() {

  //Serial Monitor
   Serial.begin(9600);

   //Wire object pour I2C bus
   Wire.begin();
   
//------------Vérification d'une bonne connexion avec les capteurs---------------

   Serial.println(F("Vérification d'une bonne connexion avec les capteurs"));
   bool status_mpu = mpu.begin(); // 0 = good
   
   tcaselect(7);
   bool status_tsl= tsl.begin();
   
   tcaselect(4);
   bool status_tsl2= tsl2.begin();
   
   if (status_tsl && !status_mpu && status_tsl2)
    Serial.println(F("Tous les capteurs sont bien connectés\n"));
   else 
   {
      if (!status_tsl)
        Serial.println(F("Capteur TSL2591 mal connecté ou non fonctionnel ... "));
      else if (!status_tsl2)
        Serial.println(F("Capteur TSL2591 mal connecté ou non fonctionnel ... "));
      else if (status_mpu!=0)
        Serial.println(F("Capteur MPU6050 mal connecté ou non fonctionnel ... "));
      else
        Serial.println(F("Aucun capteur est connecté ou fonctionnel ... "));
    while (1);
   }
   
//-----------------------------Calibration des capteurs-----------------------------
   
   //MPU6050
   Serial.println(F("Calibration du MPU6050, NE PAS LE TOUCHER"));
   delay(10000); //peutetre besoin de plus A VOIR!!
   mpu.calcOffsets(); // gyro and accelero
   Serial.println("Done!\n");

   //TSL2591 1
   tcaselect(7);
   displaySensorDetails(&tsl);
   
   tcaselect(7);
   configureSensor(&tsl);

   //TSL2591 2
   tcaselect(4);
   displaySensorDetails(&tsl2);

   tcaselect(4);
   configureSensor(&tsl2);

//-----------------------------------Set up des moteurs--------------------------------

  // Set les pins des moteurs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Turn off motors - État initial
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

 // Serial.println("Entrer l'angle de déploiment désirée: ");
  //while (Serial.available() == 0) {}
  //angle_voulu = Serial.parseInt();
 }


//-------------------------Debut du programme principal-------------------------
 void loop() {

  // On initie communication I2C bus et on lit les valeurs d'angle
  // ca se peut que ca chie pcq on relache pas le bus
  int ProgrammeExecuter = DisplayMenu();

  switch (ProgrammeExecuter){
    case 1 :
      Serial.println(F("Entrez l'angle voulu: "));
      while (Serial.available() == 0);
      angle_voulu = Serial.parseInt();
      while (Serial.available())
        Serial.read();
      deploiementStatut = deploiement(&angle_voulu,&mpu,&in1,&in2,&enA);
      if (deploiementStatut)
        Serial.println("Le déploiement a été un succès! ");
      else
        Serial.println("Erreur! ");
      break;
    case 2 : 
      if (deploiementStatut)
        orientation();
      else
        Serial.println("Erreur : Vous devez deploiyez les panneaux solaires avant! ");
      break;
    case 3 :
    while (go){
      DisplayMpu(&mpu);
      //tcaselect(7);
      //advancedRead(&tsl);
      //tcaselect(4);
      //advancedRead(&tsl2);
      if (Serial.available())
        go = false;
    }
    default :
      break;
  }
   

   while (Serial.available())
    Serial.read();
 }

//--------------------------------------------------------------------------


/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2591
*/
/**************************************************************************/
void configureSensor(Adafruit_TSL2591 *tsl)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl->setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl->setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */  
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Gain:         "));
  tsl2591Gain_t gain = tsl->getGain();
  switch(gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println(F("1x (Low)"));
      break;
    case TSL2591_GAIN_MED:
      Serial.println(F("25x (Medium)"));
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println(F("428x (High)"));
      break;
    case TSL2591_GAIN_MAX:
      Serial.println(F("9876x (Max)"));
      break;
  }
  Serial.print  (F("Timing:       "));
  Serial.print((tsl->getTiming() + 1) * 100, DEC); 
  Serial.println(F(" ms"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}



/**************************************************************************/
/*
    Performs a read using the Adafruit Unified Sensor API.
*/
/**************************************************************************/

void unifiedSensorAPIRead(Adafruit_TSL2591 *tsl)
{
  /* Get a new sensor event */ 
  sensors_event_t event;
  tsl->getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  Serial.print(F("[ ")); Serial.print(event.timestamp); Serial.print(F(" ms ] "));
  if ((event.light == 0) |
      (event.light > 4294966000.0) | 
      (event.light <-4294966000.0))
  {
    /* If event.light = 0 lux the sensor is probably saturated */
    /* and no reliable data could be generated! */
    /* if event.light is +/- 4294967040 there was a float over/underflow */
    Serial.println(F("Invalid data (adjust gain or timing)"));
  }
  else
  {
    Serial.print(event.light); Serial.println(F(" lux"));
  }
}


void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}
