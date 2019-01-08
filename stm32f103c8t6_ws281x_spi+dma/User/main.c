#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp_usart1.h"
#include "delay.h"
#include "bsp_i2c.h"
#include "../BOARD/ws2812/ws2812.h"
#include "../BOARD/mpu6050/mpu6050.h"

//#define TASK_ENABLE 0
//extern unsigned int Task_Delay[NumOfTask];


const char s[5];
int8_t i;



int main(void)
{
//mpu6050 data buff
	short Acel[3];
//	short Gyro[3];
//	short Temp;

	USART1_Config();
	NVIC_Configuration();
	delay_init();
	ws281x_init();
	i2c_GPIO_Config();
	MPU6050_Init();

	ws281x_closeAll();
//ws281x_rainbow(200);

//	while(1)
//	{
//		//  ws281x_colorWipe(ws281x_color(0, 255, 0), 50); // Green
//		ws281x_colorWipe(ws281x_color(0, 0, 10), 50); // Blue
//		ws281x_colorWipe_rollback(ws281x_color(10, 0, 0), 50);
////colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
//  // Send a theater pixel chase in...
////		ws281x_theaterChase(ws281x_color(127, 127, 127), 50); // White
////		ws281x_theaterChase(ws281x_color(127, 0, 0), 50); // Red
//		ws281x_theaterChase(ws281x_color(0, 0, 20), 50); // Blue

//		ws281x_rainbow(20);
////		ws281x_rainbowCycle(20);
////		ws281x_theaterChaseRainbow(200);
//	}
  while(1)
  {



			MPU6050ReadAcc(Acel);
			//中间
			if(Acel[0] >= (-1000) && Acel[0] < 1000)
			{	
				ws281x_colorWipe(ws281x_color(10, 10, 10), 15);
//				printf("accdata%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
			}
			//右边
			else if(Acel[0] > (-15000) && Acel[0] < (-1001)) 
			{
					ws281x_colorWipe(ws281x_color(10, 0, 0), 15); // Red
//					printf("accdata%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
			}
			//右边多一点
			else if(Acel[0] > (-30000) && Acel[0] < (-15000)) 
			{
					ws281x_colorWipe(ws281x_color(10, 10, 0), 15); // Red
//					printf("accdata%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
			}
			//左边
			else if(Acel[0] < 15000 && Acel[0] > 1001 )
			{
					ws281x_colorWipe_rollback(ws281x_color(0, 10, 10), 15);
//					printf("accdata%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
			}
			
			else if(Acel[0] < 30000 && Acel[0] > 15001 )
			{
					ws281x_colorWipe_rollback(ws281x_color(0, 0, 10), 15);
//					printf("accdata%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
			}
		

//			MPU6050ReadAcc(Acel);
//			printf("accdata%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
//			MPU6050ReadGyro(Gyro);
//			printf("	gyrodata%8d%8d%8d",Gyro[0],Gyro[1],Gyro[2]);
//			MPU6050_ReturnTemp(&Temp);
//			printf("	tempdata:%d\r\n",Temp);

		}

	
		


}
