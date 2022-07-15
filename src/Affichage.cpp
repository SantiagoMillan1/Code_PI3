#include "Affichage.h"
// Addresse (registre) du multiplexeur
#define TCAADDR 0x70
//#include "Arduino.h"

/**************************************************************************/
/*
    Affiche le menu d'options
*/
/**************************************************************************/
int DisplayMenu() {
    Serial.println(F("----------------MENU----------------"));
    Serial.println(F("Entrez le numéro de la tache voulu: "));
    Serial.println(F("1. Deploiement"));
    Serial.println(F("2. Orientation"));
    Serial.println(F("3. Valeurs des capteurs"));
    Serial.println(F("------------------------------------"));

    while (Serial.available() == 0);
    int choix = Serial.parseInt();
    //if (choix == 1) {
    Serial.print(F("Votre choix est :"));
    Serial.print(choix); Serial.println(F(". XXXXX"));
    //}
    while (Serial.available())
        Serial.read();
    return choix;
}

/**************************************************************************/
/*
    Affiche les valeurs obtenus de tous les capteurs
*/
/**************************************************************************/
void DisplayMpu(MPU6050 *mpu) {
    // Capteur d'angle
    mpu->update();
    Serial.print("X : ");
    Serial.print(mpu->getAngleX());
    Serial.print("\tY : ");
    Serial.print(mpu->getAngleY());
    Serial.print("\tZ : ");
    Serial.println(mpu->getAngleZ());
    
}

// -------CODE SUIVANT DES CAPTEURS DE LUMIERE: PROVIENT D'ADAFRUIT---------

/**************************************************************************/
/*
    Shows how to perform a basic read on visible, full spectrum or
    infrared light (returns raw 16-bit ADC values)
*/
/**************************************************************************/
void simpleRead(Adafruit_TSL2591* tsl)
{
    // Simple data read example. Just read the infrared, fullspecrtrum diode 
    // or 'visible' (difference between the two) channels.
    // This can take 100-600 milliseconds! Uncomment whichever of the following you want to read
    uint16_t x = tsl->getLuminosity(TSL2591_VISIBLE);
    //uint16_t x = tsl.getLuminosity(TSL2591_FULLSPECTRUM);
    //uint16_t x = tsl.getLuminosity(TSL2591_INFRARED);

    Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
    Serial.print(F("Luminosity: "));
    Serial.println(x, DEC);
}

/**************************************************************************/
/*
    Show how to read IR and Full Spectrum at once and convert to lux
*/
/**************************************************************************/
void advancedRead(Adafruit_TSL2591* tsl)
{
    // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
    // That way you can do whatever math and comparisons you want!
    uint32_t lum = tsl->getFullLuminosity();
    uint16_t ir, full;
    ir = lum >> 16;
    full = lum & 0xFFFF;
    Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
    Serial.print(F("IR: ")); Serial.print(ir);  Serial.print(F("  "));
    Serial.print(F("Full: ")); Serial.print(full); Serial.print(F("  "));
    Serial.print(F("Visible: ")); Serial.print(full - ir); Serial.print(F("  "));
    Serial.print(F("Lux: ")); Serial.println(tsl->calculateLux(full, ir), 6);
}


/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(Adafruit_TSL2591* tsl)
{
    sensor_t sensor;
    tsl->getSensor(&sensor);
    Serial.println(F("------------------------------------"));
    Serial.print(F("Sensor:       ")); Serial.println(sensor.name);
    Serial.print(F("Driver Ver:   ")); Serial.println(sensor.version);
    Serial.print(F("Unique ID:    ")); Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
    Serial.print(F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
    Serial.print(F("Resolution:   ")); Serial.print(sensor.resolution, 4); Serial.println(F(" lux"));
    Serial.println(F("------------------------------------"));
    Serial.println(F(""));
    delay(500);
}
