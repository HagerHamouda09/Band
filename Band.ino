#include <Wire.h>
#include "config.h"

// Sensors
#include "MAX30102Module.h"
#include "MLX90614Module.h"

// Power
#include "Battery.h"

// Communication
#include "BLEModule.h"

void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN,SCL_PIN);

////////////////////////////////////////////////////////////////////////////////////
//Max init
    max30102_init();
///////////////////////////////////////////////////////////////////////////////////



    Serial.println("Band ready. Place finger on sensor.");
}

void loop()
 {
    ////////////////////////////////////////////////////////////////////////////////
    //MAX30102 AvgBPM TEST
    max30102_update();                   // process FIFO and calculate AvgBPM
    //Serial.println(max30102_getBPM());
    //Serial.println(max30102_getSpo2());

//Better way of debugging
    static unsigned long lastPrint = 0;

if (millis() - lastPrint > 500)
{
    Serial.print("BPM: ");
    Serial.println(max30102_getBPM());

    Serial.print("SpO2: ");
    Serial.println(max30102_getSpo2());

    lastPrint = millis();
}
    ///////////////////////////////////////////////////////////////////////////////

    delay(10);
}