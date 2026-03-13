#ifndef MLX90614MODULE_H
#define MLX90614MODULE_H

#include <Wire.h>
#include <Adafruit_MLX90614.h>
extern Adafruit_MLX90614 mlx;

void mlx90614_init();
float mlx90614_readObjectTemp();
void mlx90614_update();
float mlx90614_getTemp();

#endif