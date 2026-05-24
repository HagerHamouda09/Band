// // #include <Wire.h>
// // #include "MAX30105.h"
// // #include "heartRate.h"
// // #include "spo2_algorithm.h"
// // #include "config.h"

// // MAX30105 particleSensor;

// // // ================= CONFIG =================

// // #define BUFFER_SIZE      100

// // #define LED_BRIGHTNESS   0x2F
// // #define SAMPLE_AVERAGE   16
// // #define LED_MODE         2
// // #define SAMPLE_RATE      100
// // #define PULSE_WIDTH      411
// // #define ADC_RANGE        16384

// // // ================= VARIABLES =================

// // uint32_t irBuffer[BUFFER_SIZE];
// // uint32_t redBuffer[BUFFER_SIZE];

// // int bufferIndex = 0;

// // float avgBPM = 0;
// // float bpmSum = 0;
// // int bpmCount = 0;

// // int32_t spo2 = 0;

// // long lastBeat = 0;
// // long sampleCounter = 0;

// // long filteredIR = 0;

// // // ================= SETUP =================

// // void setup()
// // {
// //     Serial.begin(9600);
// //     delay(1000);

// //     Wire.begin(SDA_PIN,SCL_PIN);

// //     Serial.println("================================");
// //     Serial.println("Initializing MAX30102...");
// //     Serial.println("================================");

// //     if (!particleSensor.begin(Wire))
// //     {
// //         Serial.println("MAX30102 NOT FOUND");
// //         while (1);
// //     }

// //     Serial.println("MAX30102 Connected");

// //     particleSensor.setup(
// //         LED_BRIGHTNESS,
// //         SAMPLE_AVERAGE,
// //         LED_MODE,
// //         SAMPLE_RATE,
// //         PULSE_WIDTH,
// //         ADC_RANGE
// //     );

// //     // LED power
// //     particleSensor.setPulseAmplitudeRed(0x3F);
// //     particleSensor.setPulseAmplitudeIR(0x3F);

// //     delay(1000);

// //     Serial.println("Place finger gently on sensor");
// //     Serial.println("Wait 10-15 seconds for stable readings");
// // }

// // // ================= LOOP =================

// // void loop()
// // {
// //     particleSensor.check();

// //     while (particleSensor.available())
// //     {
// //         uint32_t irValue = particleSensor.getIR();
// //         uint32_t redValue = particleSensor.getRed();

// //         particleSensor.nextSample();

// //         // ================= NO FINGER =================

// //         if (irValue < 10000)
// //         {
// //             Serial.println("No finger detected");

// //             avgBPM = 0;
// //             spo2 = 0;

// //             bpmSum = 0;
// //             bpmCount = 0;

// //             bufferIndex = 0;

// //             return;
// //         }

// //         // ================= FILTER =================

// //         //filteredIR = (filteredIR * 8 + irValue) / 8;

// //         // ================= STORE SAMPLES =================

// //         if (bufferIndex < BUFFER_SIZE)
// //         {
// //             irBuffer[bufferIndex] = irValue;
// //             redBuffer[bufferIndex] = redValue;

// //             bufferIndex++;
// //         }

// //         // ================= BEAT DETECTION =================

// //         if (checkForBeat(irValue) == true)
// //         {
// //             long delta = sampleCounter - lastBeat;

// //             lastBeat = sampleCounter;

// //             if (delta > 0)
// //             {
// //                 float bpm = (60.0 * SAMPLE_RATE) / delta;

// //                 if (bpm > 40 && bpm < 220)
// //                 {
// //                     bpmSum += bpm;
// //                     bpmCount++;

// //                     avgBPM = bpmSum / bpmCount;

// //                     Serial.println();
// //                     Serial.println("===== BEAT DETECTED =====");

// //                     Serial.print("Current BPM: ");
// //                     Serial.println(bpm);

// //                     Serial.print("Average BPM: ");
// //                     Serial.println(avgBPM);

// //                     Serial.println("=========================");
// //                     Serial.println();
// //                 }
// //             }
// //         }

// //         sampleCounter++;

// //         // ================= SPO2 CALC =================

// //         if (bufferIndex >= BUFFER_SIZE)
// //         {
// //             int32_t heartRate;
// //             int8_t validHeartRate;
// //             int8_t validSPO2;

// //             maxim_heart_rate_and_oxygen_saturation(
// //                 irBuffer,
// //                 BUFFER_SIZE,
// //                 redBuffer,
// //                 &spo2,
// //                 &validSPO2,
// //                 &heartRate,
// //                 &validHeartRate
// //             );

// //             Serial.println("================================");

// //             Serial.print("IR Value: ");
// //             Serial.println(irValue);

// //             Serial.print("RED Value: ");
// //             Serial.println(redValue);

// //             Serial.print("Average BPM: ");
// //             Serial.println(avgBPM);

// //             if (validSPO2)
// //             {
// //                 Serial.print("SpO2: ");
// //                 Serial.print(spo2);
// //                 Serial.println("%");
// //             }
// //             else
// //             {
// //                 Serial.println("SpO2: INVALID");
// //             }

// //             Serial.println("================================");
// //             Serial.println();

// //             bufferIndex = 0;
// //         }
// //     }
// // }







// #include <Wire.h>
// #include "MAX30105.h"
// #include "heartRate.h"
// #include "spo2_algorithm.h"
// #include "config.h"

// MAX30105 particleSensor;

// // ================= CONFIG =================

// #define BUFFER_SIZE      100

// #define LED_BRIGHTNESS   0x2F
// #define SAMPLE_AVERAGE   16
// #define LED_MODE         2
// #define SAMPLE_RATE      100
// #define PULSE_WIDTH      411
// #define ADC_RANGE        16384

// #define MAX30102_INT_PIN 5

// // ================= VARIABLES =================

// uint32_t irBuffer[BUFFER_SIZE];
// uint32_t redBuffer[BUFFER_SIZE];

// volatile bool dataReady = false;

// int bufferIndex = 0;

// float avgBPM = 0;
// float bpmSum = 0;
// int bpmCount = 0;

// int32_t spo2 = 0;

// long lastBeat = 0;
// long sampleCounter = 0;

// // ================= INTERRUPT =================

// void IRAM_ATTR max30102Interrupt()
// {
//     Serial.println("Interrupt");
//     dataReady = true;
// }

// // ================= SETUP =================

// void setup()
// {
//     Serial.begin(115200);
//     delay(1000);

//     Wire.begin(SDA_PIN, SCL_PIN);

//     Serial.println("================================");
//     Serial.println("Initializing MAX30102...");
//     Serial.println("================================");

//     if (!particleSensor.begin(Wire))
//     {
//         Serial.println("MAX30102 NOT FOUND");

//         while (1);
//     }

//     Serial.println("MAX30102 Connected");

//     particleSensor.setup(
//         LED_BRIGHTNESS,
//         SAMPLE_AVERAGE,
//         LED_MODE,
//         SAMPLE_RATE,
//         PULSE_WIDTH,
//         ADC_RANGE
//     );

//     particleSensor.setPulseAmplitudeRed(0x3F);
//     particleSensor.setPulseAmplitudeIR(0x3F);

//     // Enable interrupt pin  
//       particleSensor.enableAFULL();     // optional
// particleSensor.enableDATARDY();   // IMPORTANT
//     pinMode(MAX30102_INT_PIN, INPUT_PULLUP);


//     attachInterrupt(
//         digitalPinToInterrupt(MAX30102_INT_PIN),
//         max30102Interrupt,
//         FALLING
//     );

//     delay(1000);

//     Serial.println("Place finger gently on sensor");
//     Serial.println("Wait 10-15 seconds for stable readings");
// }

// // ================= LOOP =================

// void loop()
// {
//     // Wait for interrupt
//     if (!dataReady){
//         //Serial.println("data not ready");
//         return;
//     }
        
//     Serial.println("data ready");
//     dataReady = false;

//     // Read sensor FIFO
//     particleSensor.check();

//     while (particleSensor.available())
//     {
//         Serial.println("read from buffer");
//         uint32_t irValue = particleSensor.getIR();
//         uint32_t redValue = particleSensor.getRed();

//         particleSensor.nextSample();

//         // ================= NO FINGER =================

//         if (irValue < 10000)
//         {
//             Serial.println("No finger detected");

//             avgBPM = 0;
//             spo2 = 0;

//             bpmSum = 0;
//             bpmCount = 0;

//             bufferIndex = 0;

//             return;
//         }

//         // ================= STORE SAMPLES =================

//         if (bufferIndex < BUFFER_SIZE)
//         {
//             irBuffer[bufferIndex] = irValue;
//             redBuffer[bufferIndex] = redValue;

//             bufferIndex++;
//         }

//         // ================= BEAT DETECTION =================

//         if (checkForBeat(irValue))
//         {
//             long delta = sampleCounter - lastBeat;

//             lastBeat = sampleCounter;

//             if (delta > 0)
//             {
//                 float bpm = (60.0 * SAMPLE_RATE) / delta;

//                 if (bpm > 40 && bpm < 220)
//                 {
//                     bpmSum += bpm;
//                     bpmCount++;

//                     avgBPM = bpmSum / bpmCount;

//                     Serial.println();
//                     Serial.println("===== BEAT DETECTED =====");

//                     Serial.print("Current BPM: ");
//                     Serial.println(bpm);

//                     Serial.print("Average BPM: ");
//                     Serial.println(avgBPM);

//                     Serial.println("=========================");
//                     Serial.println();
//                 }
//             }
//         }

//         sampleCounter++;

//         // ================= SPO2 CALC =================

//         if (bufferIndex >= BUFFER_SIZE)
//         {
//             int32_t heartRate;
//             int8_t validHeartRate;
//             int8_t validSPO2;

//             maxim_heart_rate_and_oxygen_saturation(
//                 irBuffer,
//                 BUFFER_SIZE,
//                 redBuffer,
//                 &spo2,
//                 &validSPO2,
//                 &heartRate,
//                 &validHeartRate
//             );

//             Serial.println("================================");

//             Serial.print("IR Value: ");
//             Serial.println(irValue);

//             Serial.print("RED Value: ");
//             Serial.println(redValue);

//             Serial.print("Average BPM: ");
//             Serial.println(avgBPM);

//             if (validSPO2)
//             {
//                 Serial.print("SpO2: ");
//                 Serial.print(spo2);
//                 Serial.println("%");
//             }
//             else
//             {
//                 Serial.println("SpO2: INVALID");
//             }

//             Serial.println("================================");
//             Serial.println();

//             bufferIndex = 0;
//         }
//     }
// }


// #include <Wire.h>
// #include "MAX30105.h"
// #include "config.h"



// void setup()
// {
//     Serial.begin(115200);

//     Wire.begin(SDA_PIN,SCL_PIN);

//     Serial.println("Starting...");

//     if (!sensor.begin(Wire))
//     {
//         Serial.println("MAX30102 not found");

//         while (1);
//     }

//     Serial.println("MAX30102 connected");

//     // Basic setup
//     sensor.setup();

//     // Enable DATA READY interrupt
//     sensor.enableDATARDY();

//     // GPIO interrupt pin
//     pinMode(INT_PIN, INPUT_PULLUP);

//     attachInterrupt(
//         digitalPinToInterrupt(INT_PIN),
//         max30102ISR,
//         FALLING
//     );

//     Serial.println("Interrupt attached");
// }

// // ================= LOOP =================

// void loop()
// {
//     // Show actual pin state
//     static unsigned long lastPrint = 0;

//     if (millis() - lastPrint > 500)
//     {
//         lastPrint = millis();

//         Serial.print("GPIO5 State = ");
//         Serial.println(digitalRead(INT_PIN));
//     }

//     // Interrupt happened
//     if (interruptTriggered)
//     {
//         interruptTriggered = false;

//         Serial.println("INTERRUPT DETECTED");

//         // VERY IMPORTANT
//         // Clear MAX30102 interrupt flags
//         byte int1 = sensor.getINT1();
//         byte int2 = sensor.getINT2();

//         Serial.print("INT1 = 0x");
//         Serial.println(int1, HEX);

//         Serial.print("INT2 = 0x");
//         Serial.println(int2, HEX);

//         // Read FIFO
//         sensor.check();

//         while (sensor.available())
//         {
//             uint32_t ir = sensor.getIR();
//             uint32_t red = sensor.getRed();

//             Serial.print("IR: ");
//             Serial.print(ir);

//             Serial.print("   RED: ");
//             Serial.println(red);

//             sensor.nextSample();
//         }

//         Serial.println();
//     }
// }
// #include <Wire.h>
// #include "config.h"

// // Sensors
// #include "MAX30102Module.h"
// #include "MLX90614Module.h"

// // Power
// #include "Battery.h"

// // Communication
// #include "BLEModule.h"

// void setup() {
//     Serial.begin(115200);
//     Wire.begin(SDA_PIN,SCL_PIN);
//     battery_begin();

// ////////////////////////////////////////////////////////////////////////////////////
// //sensors init
//     max30102_init();
//     mlx90614_init();
// ///////////////////////////////////////////////////////////////////////////////////



//     Serial.println("Band ready. Place finger on sensor.");
// }

// void loop()
//  {
//     ////////////////////////////////////////////////////////////////////////////////
//     //MAX30102 AvgBPM TEST
//             // process FIFO and calculate AvgBPM
//     //Serial.println(max30102_getBPM());
//     //Serial.println(max30102_getSpo2());
   
//     max30102_update();  
//     mlx90614_update();
//     battery_update();
// //Better way of debugging
//     static unsigned long lastPrint = 0;

// if (millis() - lastPrint > 500)
// {
//     Serial.print("BPM: ");
//     Serial.println(max30102_getBPM());

//     Serial.print("SpO2: ");
//     Serial.println(max30102_getSpo2());
    
//     float temp=mlx90614_getTemp();
//     if(temp >0){
//     Serial.print("Body Temp: ");
//     Serial.println(temp);}
//     else  Serial.print("temp not ready");

//     Serial.print("Voltage: ");
//     Serial.print(battery_getVoltage());
//     Serial.print(" V  |  Percentage: ");
//     Serial.print(battery_getPercentage());
//     Serial.println(" %");

//     lastPrint = millis();
// }
//     ///////////////////////////////////////////////////////////////////////////////

//     delay(10);
// }
// #include <Wire.h>
// #include "config.h"

// // Sensors
// #include "MAX30102Module.h"
// #include "MLX90614Module.h"

// // Power
// #include "Battery.h"

// // Communication
// #include "BLEModule.h"

// void setup() {
//     Serial.begin(115200);
//     Wire.begin(SDA_PIN,SCL_PIN);
//    // battery_begin();

// ////////////////////////////////////////////////////////////////////////////////////
// //sensors init
//     max30102_init();
//    // mlx90614_init();
// ///////////////////////////////////////////////////////////////////////////////////



//     Serial.println("Band ready. Place finger on sensor.");
// }
// unsigned long lastRead = 0;
// void loop()
//  {
//     ////////////////////////////////////////////////////////////////////////////////
//     //MAX30102 AvgBPM TEST
//             // process FIFO and calculate AvgBPM
//     //Serial.println(max30102_getBPM());
//     //Serial.println(max30102_getSpo2());
// if (millis() - lastRead >= 2000)
//     {
//         lastRead = millis();
//     max30102_update();  
//     // mlx90614_update();
//     // battery_update();
// //Better way of debugging
//     static unsigned long lastPrint = 0;

// // if (millis() - lastPrint > 500)
// // {

//     /////el mafrod badal el prints neb3at bl bluetooth
//     Serial.print("BPM: ");
//     Serial.println(max30102_getBPM());

//     Serial.print("SpO2: ");
//     Serial.println(max30102_getSpo2());
    
//     // float temp=mlx90614_getTemp();
//     // if(temp >0){
//     // Serial.print("Body Temp: ");
//     // Serial.println(temp);}
//     // else  Serial.print("temp not ready");

//     // Serial.print("Voltage: ");
//     // Serial.print(battery_getVoltage());
//     // Serial.print(" V  |  Percentage: ");
//     // Serial.print(battery_getPercentage());
//     // Serial.println(" %");

//     //lastPrint = millis();
// // }
//     ///////////////////////////////////////////////////////////////////////////////
//     }
//     delay(10);
// }


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
    battery_begin();

////////////////////////////////////////////////////////////////////////////////////
//sensors init
    max30102_init();
   // mlx90614_init();
///////////////////////////////////////////////////////////////////////////////////

ble_begin();

    Serial.println("Band ready. Place finger on sensor.");
}
unsigned long lastRead = 0;
void loop()
 {
    ////////////////////////////////////////////////////////////////////////////////
    //MAX30102 AvgBPM TEST
            // process FIFO and calculate AvgBPM
    //Serial.println(max30102_getBPM());
    //Serial.println(max30102_getSpo2());

    // BUG FIX 1: update() called every tick so FIFO drains continuously
    max30102_update();

if (millis() - lastRead >= 2000)
    {
        lastRead = millis();
    // mlx90614_update();
    battery_update();
    ble_sendData(battery_getPercentage());
    
    ble_update();
    delay(10);
    //battery_update();
//Better way of debugging
    static unsigned long lastPrint = 0;

// if (millis() - lastPrint > 500)
// {

    /////el mafrod badal el prints neb3at bl bluetooth
    Serial.print("BPM: ");
    Serial.println(max30102_getBPM());

    Serial.print("SpO2: ");
    Serial.println(max30102_getSpo2());
    
    // float temp=mlx90614_getTemp();
    // if(temp >0){
    // Serial.print("Body Temp: ");
    // Serial.println(temp);}
    // else  Serial.print("temp not ready");

    Serial.print("Voltage: ");
    Serial.print(battery_getVoltage());
    Serial.print(" V  |  Percentage: ");
    Serial.print(battery_getPercentage());
    Serial.println(" %");

    //lastPrint = millis();
// }
    ///////////////////////////////////////////////////////////////////////////////
    }
    delay(10);
}