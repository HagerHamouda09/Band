#include "MAX30102Module.h"
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "config.h"
#include "spo2_algorithm.h"

#define BUFFER_SIZE 112

MAX30105 maxSensor;
bool maxIsReady= false;
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
static bool consequentiveIR=false;
static bool previous = true;/////false ma3naha en el reading valid


void max30102_init()
{
    if (!maxSensor.begin(Wire))
    {
        Serial.println("MAX30102 not found");
    }
    maxIsReady=true;
    maxSensor.setup(LED_BRIGHTNESS, SAMPLE_AVERAGE, LED_MODE,
                 SAMPLE_RATE, PULSE_WIDTH, ADC_RANGE);

    maxSensor.setPulseAmplitudeRed(0x3F);
    maxSensor.setPulseAmplitudeIR(0x3F);
    delay(1000);

}

void max30102_update()
{ 
    maxSensor.check();
    int samplesThisCall = 0;  

    while (maxSensor.available())
    {
        uint32_t  irValue = maxSensor.getIR();
        uint32_t  redValue = maxSensor.getRed();
        maxSensor.nextSample();

        samplesThisCall++;  
        

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

                // if(c>= 2){
                //     consequentiveIR=true;
                // }
                if( c == 10 ){
                    Serial.println("Driver is not wearing the band correctly");
                }
                continue;
            }
        c=0;
        if (ReadingIndex < BUFFER_SIZE)
        {
        RED_Buffer[ReadingIndex]=redValue;
        IR_Buffer[ReadingIndex]=irValue;
        ReadingIndex++;
        }
        if (checkForBeat(irValue))
        {
            if(lastBeat==-1)
            {
             lastBeat=SampleIndex;   
            }
            else
            {
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
                    }
                }
            }
        }
        SampleIndex++;
    }

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
    ReadingIndex = 0;

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

bool maxReady(){
    return maxIsReady;
}

bool exceededTrials(){
    return c>=10 ? true : false;
}
bool maximumTrials(){
    return c>=50 ? true : false;
   
}
