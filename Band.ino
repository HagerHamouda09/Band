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
    Battery.begin();

////////////////////////////////////////////////////////////////////////////////////
//sensors init
    max30102_init();
    mlx90614_init();
///////////////////////////////////////////////////////////////////////////////////



    Serial.println("Band ready. Place finger on sensor.");
}

void loop()
 {
    ////////////////////////////////////////////////////////////////////////////////
    //MAX30102 AvgBPM TEST
            // process FIFO and calculate AvgBPM
    //Serial.println(max30102_getBPM());
    //Serial.println(max30102_getSpo2());
   
    max30102_update();  
    mlx90614_update();
    battery_update();
//Better way of debugging
    static unsigned long lastPrint = 0;

if (millis() - lastPrint > 500)
{
    Serial.print("BPM: ");
    Serial.println(max30102_getBPM());

    Serial.print("SpO2: ");
    Serial.println(max30102_getSpo2());
    
    float temp=mlx90614_getTemp();
    if(temp >0){
    Serial.print("Body Temp: ");
    Serial.println(temp_mlx90614);}
    else  Serial.print("temp not ready");

    Serial.print("Voltage: ");
    Serial.print(battery_getVoltage());
    Serial.print(" V  |  Percentage: ");
    Serial.print(battery_getPercentage());
    Serial.println(" %");

    lastPrint = millis();
}
    ///////////////////////////////////////////////////////////////////////////////

    delay(10);
}


