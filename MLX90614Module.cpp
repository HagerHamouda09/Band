
#include "MLX90614Module.h"
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float temp=0;
float lastTempRead=0;
bool mlxIsReady = false;
int calibrated_Temp=0;

void mlx90614_init() {
    Wire.begin();  
    if (!mlx.begin()) {
        Serial.println("MLX90614 not detected!");
    }
mlxIsReady=true;

    mlx.writeEmissivity(0.98);
    Serial.println("MLX90614 Ready");
}

float mlx90614_readObjectTemp() {
    float sum = 0;
    for(int i = 0; i < 5; i++) {
        sum += mlx.readObjectTempC();
        delay(50);
    }
    return sum / 5.0;
}

void mlx90614_update() {
    if (millis() - lastTempRead >= 1000) 
    { 
        lastTempRead = millis();
        temp = mlx90614_readObjectTemp() ;
        calibrated_Temp= temp + 5 ;
       
    }
}
float mlx90614_getTemp()
{   
    return calibrated_Temp ;
}


bool mlxReady(){
    return mlxIsReady;
}