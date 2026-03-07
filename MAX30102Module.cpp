#include "MAX30102Module.h"
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "config.h"
#include "spo2_algorithm.h"


#define BUFFER_SIZE 112
MAX30105 maxSensor;

volatile bool fifoReady = false;
long lastBeat = -1;
volatile long  SampleIndex=-1;
float AvgBPM = 0;
int32_t SPO2_Reading=0;

uint32_t IR_Buffer[BUFFER_SIZE];
uint32_t RED_Buffer[BUFFER_SIZE];


int ReadingIndex=0;


void IRAM_ATTR MaxISR()
{
    fifoReady = true;
}

void max30102_init()
{
    if (!maxSensor.begin(Wire))
    {
        Serial.println("MAX30102 not found");
        while (1);
    }

    maxSensor.setup(LED_BRIGHTNESS, SAMPLE_AVERAGE, LED_MODE,
                 SAMPLE_RATE, PULSE_WIDTH, ADC_RANGE);

    maxSensor.enableAFULL();
    maxSensor.setFIFOAlmostFull(16);

    pinMode(MAX_INT, INPUT);
    attachInterrupt(digitalPinToInterrupt(MAX_INT), MaxISR, FALLING);
}

void max30102_update()
{
    //Threshold reached?
    if (!fifoReady) 
    return;

    fifoReady = false;

//Moved to Global but will check why later
    //int index=-1;
    float sum=0;
    int beatCount=0;
    while (maxSensor.available())
    {
        // index+=1;

        uint32_t  irValue = maxSensor.getIR();
        uint32_t  redValue = maxSensor.getRed();
        maxSensor.nextSample();


        RED_Buffer[ReadingIndex]=redValue;
        IR_Buffer[ReadingIndex]=irValue;
        ReadingIndex++;

        if (checkForBeat(irValue))
        {
            if(lastBeat==-1)
            {
             lastBeat=SampleIndex;   
            }
            else
            {
                //delta between peaks
                float delta = (SampleIndex - lastBeat) * (1.0 /SAMPLE_RATE);
                lastBeat=SampleIndex;
                float instantBPM = 60.0 / (delta);
                sum+=instantBPM;
                beatCount++;
            }
        }
        SampleIndex++;
    }

        if (beatCount > 0)
        {
        AvgBPM=sum/beatCount;
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

    // if (validHeartRate)
    //   Serial.print("BPM (from SpO2 calc): "), Serial.println(heartrateCalc);

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