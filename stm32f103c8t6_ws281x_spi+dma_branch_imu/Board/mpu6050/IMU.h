#ifndef __IMU_H
#define __IMU_H
#include  <math.h>
#include "stm32f10x.h"

typedef struct 
{
    float Angle_dat;
    float Angley_dat;
}Ang_Dat;


Ang_Dat IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
extern float Angle,Angley;


#endif




