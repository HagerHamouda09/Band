// #include "MAX30102Module.h"
// #include <Wire.h>
// #include "MAX30105.h"
// #include "heartRate.h"
// #include "config.h"
// #include "spo2_algorithm.h"


// #define BUFFER_SIZE 112
// MAX30105 maxSensor;

// //volatile bool fifoReady = false;
// long lastBeat = -1;
// volatile long  SampleIndex=0;
// float AvgBPM = 0.0f;
// int32_t SPO2_Reading=0;
// int c=0;

// uint32_t IR_Buffer[BUFFER_SIZE];
// uint32_t RED_Buffer[BUFFER_SIZE];


// int ReadingIndex=0;


// // void IRAM_ATTR MaxISR()
// // {
// //     fifoReady = true;
// // }

// void max30102_init()
// {
//     if (!maxSensor.begin(Wire))
//     {
//         Serial.println("MAX30102 not found");
//         while (1);
//     }

//     maxSensor.setup(LED_BRIGHTNESS, SAMPLE_AVERAGE, LED_MODE,
//                  SAMPLE_RATE, PULSE_WIDTH, ADC_RANGE);

//     // maxSensor.enableAFULL();
//     // maxSensor.setFIFOAlmostFull(16);

//    // pinMode(MAX_INT, INPUT);
//    //Make it pullup as the interrupt line is open drain
//       //pinMode(MAX_INT, INPUT_PULLUP);
//     //attachInterrupt(digitalPinToInterrupt(MAX_INT), MaxISR, FALLING);

//     //Added Delay
//         delay(1000);

// }

// void max30102_update()
// {
//     //Threshold reached?
//     // if (!fifoReady) 
//     // return;

//     // fifoReady = false;

// //Moved to Global but will check why later
//     //int index=-1;
//     float sum=0;
//     int beatCount=0;
//      lastBeat = -1;
//        SampleIndex=0;
//      AvgBPM = 0.0f;
//      SPO2_Reading=0;
//     ReadingIndex=0;

//     //SparkFun library requires checking to pull data from FIFO 
//     maxSensor.check();
//     while (maxSensor.available())
//     {
//         // index+=1;

//         uint32_t  irValue = maxSensor.getIR();
//         uint32_t  redValue = maxSensor.getRed();
//         maxSensor.nextSample();

//     //  Skin detection check
    
//         if (irValue < 10000)
//             {
//                 // AvgBPM = 0;
//                 // SPO2_Reading = 0;
//                 // ReadingIndex = 0;   // reset buffer
//                 // lastBeat = -1;      // reset beat detection
//                 // return;
//                 Serial.println("Adjust the band");
//                 c++;
//                 if( c == 10 ){
//                     Serial.println("Driver is not wearing the band correctly");
//                 }
//             }
//         //Bound Check before writing to avoid overflow
//         if (ReadingIndex < BUFFER_SIZE)
//         {
//         RED_Buffer[ReadingIndex]=redValue;
//         IR_Buffer[ReadingIndex]=irValue;
//         ReadingIndex++;
//         }
//         if (checkForBeat(irValue))
//         {
//             if(lastBeat==-1)
//             {
//              lastBeat=SampleIndex;   
//             }
//             else
//             {
//                 //delta between peaks
//                // float delta = (SampleIndex - lastBeat) * (1.0 /SAMPLE_RATE);   
               
//                //Less floating point noise
//                long diff = SampleIndex - lastBeat;
//                 if(diff>0)
//                 {
//                 float instantBPM = (60.0 * SAMPLE_RATE) / (diff);
//                 lastBeat=SampleIndex;
//                  sum+=instantBPM;
//                  beatCount++;

// //Check that the readings are not noise spikes
// // can be altered later when handling different conditions
// //now just a verification of sensor
//                     // if (instantBPM > 30 && instantBPM < 220)
//                     // {
//                     // sum+=instantBPM;
//                     // beatCount++;
//                     // }
//                 }
//             }
//         }
//         SampleIndex++;
//     }

//         if (beatCount > 0)
//         {
//         AvgBPM=sum/beatCount;
//         }

//     if(ReadingIndex>=BUFFER_SIZE)
//     {
//         int32_t spo2, heartrateCalc;
//     int8_t validSPO2, validHeartRate;

//     maxim_heart_rate_and_oxygen_saturation(
//         IR_Buffer, BUFFER_SIZE , RED_Buffer,
//         &spo2, &validSPO2, &heartrateCalc, &validHeartRate
//     );

//     if (validSPO2)
//     {
//       Serial.print("SpO2: "), Serial.println(spo2);
//     SPO2_Reading=spo2;
//     }
//     else
//       Serial.println("SpO2 invalid");

//     // if (validHeartRate)
//     //   Serial.print("BPM (from SpO2 calc): "), Serial.println(heartrateCalc);

// //     ReadingIndex = 0;

// // //Reset
// //     SampleIndex=-1;
// //     lastBeat=-1;
//         }

// }

// float max30102_getBPM()
// {
//     return AvgBPM;
// }

// int32_t max30102_getSpo2()
// {
//     return SPO2_Reading;
// }





#include "MAX30102Module.h"
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "config.h"
#include "spo2_algorithm.h"


#define BUFFER_SIZE 112
MAX30105 maxSensor;

//volatile bool fifoReady = false;
static long lastBeat = -1;
static volatile long  SampleIndex=0;
static float AvgBPM = 0.0f;
static int32_t SPO2_Reading=0;
static int c=0;

static float bpmSum = 0;
static int beatCount = 0;

uint32_t IR_Buffer[BUFFER_SIZE];
uint32_t RED_Buffer[BUFFER_SIZE];

static int ReadingIndex=0;

// void IRAM_ATTR MaxISR()
// {
//     fifoReady = true;
// }

void max30102_init()
{
    if (!maxSensor.begin(Wire))
    {
        Serial.println("MAX30102 not found");
        while (1);
    }

    maxSensor.setup(LED_BRIGHTNESS, SAMPLE_AVERAGE, LED_MODE,
                 SAMPLE_RATE, PULSE_WIDTH, ADC_RANGE);

    maxSensor.setPulseAmplitudeRed(0x3F);
    maxSensor.setPulseAmplitudeIR(0x3F);


    // maxSensor.enableAFULL();
    // maxSensor.setFIFOAlmostFull(16);

   // pinMode(MAX_INT, INPUT);
   //Make it pullup as the interrupt line is open drain
      //pinMode(MAX_INT, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(MAX_INT), MaxISR, FALLING);

    //Added Delay
        delay(1000);

}

void max30102_update()
{
    //Threshold reached?
    // if (!fifoReady) 
    // return;

    // fifoReady = false;

    //SparkFun library requires checking to pull data from FIFO 
    maxSensor.check();

    int samplesThisCall = 0;  // DEBUG

    while (maxSensor.available())
    {
        uint32_t  irValue = maxSensor.getIR();
        uint32_t  redValue = maxSensor.getRed();
        maxSensor.nextSample();

        samplesThisCall++;  // DEBUG

    //  Skin detection check
        if (irValue < 10000)
            {
                lastBeat = -1;
                SampleIndex = 0;
                ReadingIndex = 0;
                bpmSum = 0;
                beatCount = 0;
                AvgBPM = 0;
                SPO2_Reading = 0;
                Serial.println("Adjust the band");
                c++;
                delay(1000);
                if( c == 10 ){
                    Serial.println("Driver is not wearing the band correctly");
                }
                continue;
            }
        //Bound Check before writing to avoid overflow
        if (ReadingIndex < BUFFER_SIZE)
        {
        RED_Buffer[ReadingIndex]=redValue;
        IR_Buffer[ReadingIndex]=irValue;
        ReadingIndex++;
        }
        if (checkForBeat(irValue))
        {
            // Serial.print("BEAT DETECTED at SampleIndex=");  // DEBUG
            // Serial.println(SampleIndex);                     // DEBUG

            if(lastBeat==-1)
            {
             lastBeat=SampleIndex;   
            }
            else
            {
                //delta between peaks
               // float delta = (SampleIndex - lastBeat) * (1.0 /SAMPLE_RATE);   
               
               //Less floating point noise
               long diff = SampleIndex - lastBeat;
                if(diff>0)
                {
                float instantBPM = (60.0 * SAMPLE_RATE) / (diff);
                lastBeat=SampleIndex;

                    if (instantBPM > 30 && instantBPM < 220)
                    {
                    bpmSum+=instantBPM;
                    beatCount++;
                    AvgBPM = bpmSum / beatCount;
                    // Serial.print("instantBPM=");   // DEBUG
                    // Serial.println(instantBPM);    // DEBUG
                    }
                }
            }
        }
        SampleIndex++;
    }

    // DEBUG: show how many samples arrived this call and buffer fill progress
    // Serial.print("samples=");
    // Serial.print(samplesThisCall);
    // Serial.print("  ReadingIndex=");
    // Serial.println(ReadingIndex);

    if(ReadingIndex>=BUFFER_SIZE)
    {
        int32_t spo2, heartrateCalc;
    int8_t validSPO2, validHeartRate;

    maxim_heart_rate_and_oxygen_saturation(
        IR_Buffer, BUFFER_SIZE , RED_Buffer,
        &spo2, &validSPO2, &heartrateCalc, &validHeartRate
    );

    if (validSPO2)
    {
      Serial.print("SpO2: "), Serial.println(spo2);
    SPO2_Reading=spo2;
    }
    else
      Serial.println("SpO2 invalid");

    // if (validHeartRate)
    //   Serial.print("BPM (from SpO2 calc): "), Serial.println(heartrateCalc);

    ReadingIndex = 0;

// //Reset
//     SampleIndex=-1;
//     lastBeat=-1;
        }

}

float max30102_getBPM()
{
    return AvgBPM;
}

int32_t max30102_getSpo2()
{
    return SPO2_Reading;
}



// #include "MAX30102Module.h"
// #include <Wire.h>
// #include "MAX30105.h"
// #include "heartRate.h"
// #include "config.h"
// #include "spo2_algorithm.h"


// #define BUFFER_SIZE 112
// #define NO_FINGER_THRESHOLD 5000
// #define NO_FINGER_CONFIRM   10

// MAX30105 maxSensor;

// //volatile bool fifoReady = false;
// static long lastBeat = -1;
// static volatile long  SampleIndex=0;
// static float AvgBPM = 0.0f;
// static int32_t SPO2_Reading=0;
// static int c=0;

// static float bpmSum = 0;
// static int beatCount = 0;
// static int noFingerCount = 0;

// uint32_t IR_Buffer[BUFFER_SIZE];
// uint32_t RED_Buffer[BUFFER_SIZE];

// static int ReadingIndex=0;

// // void IRAM_ATTR MaxISR()
// // {
// //     fifoReady = true;
// // }

// void max30102_init()
// {
//     if (!maxSensor.begin(Wire))
//     {
//         Serial.println("MAX30102 not found");
//         while (1);
//     }

//     maxSensor.setup(LED_BRIGHTNESS, SAMPLE_AVERAGE, LED_MODE,
//                  SAMPLE_RATE, PULSE_WIDTH, ADC_RANGE);

//     // maxSensor.enableAFULL();
//     // maxSensor.setFIFOAlmostFull(16);

//    // pinMode(MAX_INT, INPUT);
//    //Make it pullup as the interrupt line is open drain
//       //pinMode(MAX_INT, INPUT_PULLUP);
//     //attachInterrupt(digitalPinToInterrupt(MAX_INT), MaxISR, FALLING);

//     //Added Delay
//         delay(1000);

// }

// void max30102_update()
// {
//     //Threshold reached?
//     // if (!fifoReady) 
//     // return;

//     // fifoReady = false;

//     //SparkFun library requires checking to pull data from FIFO 
//     maxSensor.check();
//     while (maxSensor.available())
//     {
//         uint32_t  irValue = maxSensor.getIR();
//         uint32_t  redValue = maxSensor.getRed();
//         maxSensor.nextSample();

//         // DEBUG: print every raw IR so we can see current values with new brightness
//         // Serial.print("RAW IR: ");
//         // Serial.println(irValue);

//     //  Skin detection check
//         if (irValue < NO_FINGER_THRESHOLD)
//             {
//                 noFingerCount++;
//                 if (noFingerCount >= NO_FINGER_CONFIRM)
//                 {
//                     lastBeat = -1;
//                     SampleIndex = 0;
//                     ReadingIndex = 0;
//                     bpmSum = 0;
//                     beatCount = 0;
//                     AvgBPM = 0;
//                     SPO2_Reading = 0;
//                     Serial.println("Adjust the band");
//                     c++;
//                     if( c == 10 ){
//                         Serial.println("Driver is not wearing the band correctly");
//                     }
//                 }
//                 continue;
//             }

//         // good sample — reset the no-finger counter
//         noFingerCount = 0;

//         //Bound Check before writing to avoid overflow
//         if (ReadingIndex < BUFFER_SIZE)
//         {
//         RED_Buffer[ReadingIndex]=redValue;
//         IR_Buffer[ReadingIndex]=irValue;
//         ReadingIndex++;
//         }
//         if (checkForBeat(irValue))
//         {
//             if(lastBeat==-1)
//             {
//              lastBeat=SampleIndex;   
//             }
//             else
//             {
//                 //delta between peaks
//                // float delta = (SampleIndex - lastBeat) * (1.0 /SAMPLE_RATE);   
               
//                //Less floating point noise
//                long diff = SampleIndex - lastBeat;
//                 if(diff>0)
//                 {
//                 float instantBPM = (60.0 * SAMPLE_RATE) / (diff);
//                 lastBeat=SampleIndex;

//                     if (instantBPM > 30 && instantBPM < 220)
//                     {
//                     bpmSum+=instantBPM;
//                     beatCount++;
//                     AvgBPM = bpmSum / beatCount;
//                     }
//                 }
//             }
//         }
//         SampleIndex++;
//     }

//     if(ReadingIndex>=BUFFER_SIZE)
//     {
//         int32_t spo2, heartrateCalc;
//     int8_t validSPO2, validHeartRate;

//     maxim_heart_rate_and_oxygen_saturation(
//         IR_Buffer, BUFFER_SIZE , RED_Buffer,
//         &spo2, &validSPO2, &heartrateCalc, &validHeartRate
//     );

//     if (validSPO2)
//     {
//       Serial.print("SpO2: "), Serial.println(spo2);
//     SPO2_Reading=spo2;
//     }
//     else
//       Serial.println("SpO2 invalid");

//     // if (validHeartRate)
//     //   Serial.print("BPM (from SpO2 calc): "), Serial.println(heartrateCalc);

//     ReadingIndex = 0;

// // //Reset
// //     SampleIndex=-1;
// //     lastBeat=-1;
//         }

// }

// float max30102_getBPM()
// {
//     return AvgBPM;
// }

// int32_t max30102_getSpo2()
// {
//     return SPO2_Reading;
// }