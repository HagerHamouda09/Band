#include <Wire.h>
#include "config.h"
#include "MAX30102Module.h"
#include "MLX90614Module.h"
#include "Battery.h"
#include "BLEModule.h"

int sendcounter=0;
unsigned long lastRead = 0;
SYSTEM_CASES systemstate=SYSTEM_CHECK;

void vitalSending(){
    max30102_update();
    battery_update();
    ble_update(); 
    mlx90614_update();
    if (millis() - lastRead >= 2000)
    {   
        lastRead = millis();
        ble_sendData(battery_getPercentage(),max30102_getBPM(),max30102_getSpo2(),mlx90614_getTemp());
        delay(10);
        static unsigned long lastPrint = 0;
        Serial.print("BPM: ");
        Serial.println(max30102_getBPM());

        Serial.print("SpO2: ");
        Serial.println(max30102_getSpo2());
        
        float temp=mlx90614_getTemp();
        if(temp >0){
        Serial.print("Body Temp: ");
        Serial.println(temp);}
        else  Serial.print("temp not ready");

        Serial.print("Voltage: ");
        Serial.print(battery_getVoltage());
        Serial.print(" V  |  Percentage: ");
        Serial.print(battery_getPercentage());
        Serial.println(" %");
        }
        delay(10);
        sendcounter++;
}


void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN,SCL_PIN);

    battery_begin();
    max30102_init();
   mlx90614_init();
    ble_begin();
    delay(2000);
    Serial.println("Band ready. Place finger on sensor.");
}


void loop()
 {  battery_update();
    switch (systemstate){
        case SYSTEM_CHECK :
        //Battery is commented to test while powered from Laptop
            if ( ble_isConnected() && maxReady()  &&  mlxReady() /* && !batteryIsDrained()*/ ){          
                ble_send_systemcheck(true);
            }else 
                ble_send_systemcheck(false);

        break;

        case SYSTEM_NORMAL:
            if (exceededTrials()){
                systemstate=SYSTEM_EXCEEDED_TRIALS;
            }else
            vitalSending();

        break;
        case SYSTEM_EXCEEDED_TRIALS:
            ble_send_exceeded_trials();
            delay(1000);
            max30102_update();
            if (!exceededTrials() && !maximumTrials()){
                systemstate=SYSTEM_NORMAL;
            }
            if(maximumTrials()){
                //c is very large we should notify mobile(fleet admin) to end trip-> driver not wearning band 
                ////C IS VERY LARGE END SYSTEM
                ble_send_maximumTrials();
            }
        break;
        case SYSTEM_END:
            Serial.println("[SYS] Entering deep sleep...");
            Serial.flush();
            delay(200);
            esp_deep_sleep_start();
        break;
    }
   
}