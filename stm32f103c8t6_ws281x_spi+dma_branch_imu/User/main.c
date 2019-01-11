#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp_usart1.h"
#include "delay.h"
#include "bsp_i2c.h"
#include "../BOARD/ws2812/ws2812.h"
#include "../BOARD/mpu6050/mpu6050.h"
#include "../BOARD/mpu6050/IMU.h"

float  Angle_ax=0.0,Angle_ay=0.0,Angle_az=0.0;  //由加速度计算的加速度(弧度制)
float  Angle_gy=0.0,Angle_gx=0.0,Angle_gz=0.0;  //由角速度计算的角速率(角度制)

short Acel[3];
short Gyro[3];

int main(void)
{
//mpu6050 data buff

//	short Temp;

	USART1_Config();
	NVIC_Configuration();
	delay_init();
	ws281x_init();
	i2c_GPIO_Config();
	MPU6050_Init();
//	ws281x_closeAll();

	while(1)
	{
	//  ws281x_colorWipe(ws281x_color(0, 2, 0), 40);
//		Alternate_RedNBlue();
	
		//Show_PowerQuantity(23);
		LightUp_Distance(19000);

	}

}


//	MPU6050ReadAcc(Acel);
//	MPU6050ReadGyro(Gyro);
//	  
//	Angle_ax = Acel[0] / 8192.0;
//	Angle_ay = Acel[1] / 8192.0;
//	Angle_az = Acel[2] / 8192.0;
//	Angle_gx = Gyro[0] / 65.5*0.0174533;
//	Angle_gy = Gyro[1] / 65.5*0.0174533;
//	Angle_gz = Gyro[2] / 65.5*0.0174533;

//	Ang_Dat Ang= IMUupdate(Angle_gx,Angle_gy,Angle_gz,Angle_ax,Angle_ay,Angle_az);
//	  
//	
//	
//	  printf("	longtitude :%f\r\n",Ang.Angle_dat);
//	  printf("	latitude :%f\r\n",Ang.Angley_dat);





//ws281x_rainbow(200);

//	while(1)
//	{
//	  //ws281x_colorWipe(ws281x_color(0, 255, 0), 50); // Green
//		ws281x_colorWipe(ws281x_color(0, 0, 10), 50); // Blue
//		ws281x_colorWipe_rollback(ws281x_color(10, 0, 0), 50);
////	colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW

//		ws281x_rainbow(20);
////		ws281x_rainbowCycle(20);
////		ws281x_theaterChaseRainbow(200);
//	}



//	ws281x_theaterChase(ws281x_color(20, 25, 30), 50); // White
//	ws281x_theaterChase(ws281x_color(20, 0, 30), 50); 
//	ws281x_theaterChase(ws281x_color(10, 55, 35), 50); // Blue

