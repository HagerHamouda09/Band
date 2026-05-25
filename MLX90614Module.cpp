// #include "MLX90614Module.h"
// Adafruit_MLX90614 mlx = Adafruit_MLX90614();
// float temp=0;
// float lastTempRead=0;
// void mlx90614_init() {
//     Wire.begin();  // Initialize I2C
//     if (!mlx.begin()) {
//         Serial.println("MLX90614 not detected!");
//         while (1);
//     }
//     // Set human skin emissivity
//     mlx.writeEmissivity(0.98);
//     Serial.println("MLX90614 Ready");
// }

// /// Read body temperature with averaging
// float mlx90614_readObjectTemp() {
//     float sum = 0;
//     for(int i = 0; i < 5; i++) {
//         sum += mlx.readObjectTempC();
//         delay(50); // shorter delay is fine for smoothing
//     }
//     return sum / 5.0;
// }
#include "MLX90614Module.h"
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float temp=0;
float lastTempRead=0;
bool mlxIsReady = false;


void mlx90614_init() {
    Wire.begin();  // Initialize I2C
    if (!mlx.begin()) {
        Serial.println("MLX90614 not detected!");
       // while (1);
    }
mlxIsReady=true;

    // Set human skin emissivity
    mlx.writeEmissivity(0.98);
    Serial.println("MLX90614 Ready");
}

/// Read body temperature with averaging
float mlx90614_readObjectTemp() {
    float sum = 0;
    for(int i = 0; i < 5; i++) {
        sum += mlx.readObjectTempC();
        delay(50); // shorter delay is fine for smoothing
    }
    return sum / 5.0;
}

// Update function to call in main loop
void mlx90614_update() {
    if (millis() - lastTempRead >= 1000) { // read every 1 second
        lastTempRead = millis();
        temp = mlx90614_readObjectTemp();
       
    }
    else temp=0;
}
float mlx90614_getTemp()
{
    return temp;
}


bool mlxReady(){
    return mlxIsReady;
}

//float bodyTemp = temp + 2.0;

// // Update function to call in main loop
// void mlx90614_update() {
//     if (millis() - lastTempRead >= 1000) { // read every 1 second
//         lastTempRead = millis();
//         temp = mlx90614_readObjectTemp();
       
//     }
//     else temp=0;
// }
// float mlx90614_getTemp()
// {
//     return temp;
// }

// //float bodyTemp = temp + 2.0;