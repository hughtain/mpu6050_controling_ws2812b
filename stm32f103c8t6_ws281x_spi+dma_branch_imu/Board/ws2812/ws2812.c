#include "../BOARD/ws2812/ws2812.h"
#include "bsp_usart1.h"
#include "delay.h"
#include "math.h"

uint8_t pixelBuffer[PIXEL_NUM][24] ;
 

void ws281x_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	

  /* PA7  SPI1_MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //SPI-PA-7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI1, &SPI_InitStructure);  
 
	SPI_Cmd(SPI1, ENABLE); 
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
  
	DMA_DeInit(DMA1_Channel3);   
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(SPI1 -> DR); 
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pixelBuffer; //cmar
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  
	DMA_InitStructure.DMA_BufferSize = PIXEL_NUM * 24; //cndtr;  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel3, &DMA_InitStructure); 
  
  ws281x_closeAll();  //SHUT DOWN ALL LEDS
  delay_ms(100); //SHUT DOWN DELAY
}


//shut down all the led
void ws281x_closeAll(void)
{
  uint16_t i;
  uint8_t j;
  
  for(i = 0; i < PIXEL_NUM; ++i)
  {
    for(j = 0; j < 24; ++j)
    {
      pixelBuffer[i][j] = WS_LOW;
    }
  }
  ws281x_show(); 
}

//
//将三原色三个八位数据合并为24位数据
uint32_t ws281x_color(uint8_t red, uint8_t green, uint8_t blue)
{
  return green << 16 | red << 8 | blue;
}

//设置第N个灯珠的颜色
void ws281x_setPixelColor(uint16_t n ,uint32_t GRBcolor)
{
  uint8_t i;
  if(n < PIXEL_NUM)
  {
    for(i = 0; i < 24; ++i)
    {
      pixelBuffer[n][i] = (((GRBcolor << i) & 0X800000) ? WS_HIGH : WS_LOW);
    }
  }
}

//设置第N个灯珠的颜色
void ws281x_setPixelRGB(uint16_t n ,uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t i;
  
  if(n < PIXEL_NUM)
  {
    for(i = 0; i < 24; ++i)
    {
      pixelBuffer[n][i] = (((ws281x_color(red,green,blue) << i) & 0X800000) ? WS_HIGH : WS_LOW);
    }
  }
}


//重新更新颜色显示，因为每次重新设定后的颜色是被存入缓存的，所以要执行这个函数刷新
void ws281x_show(void)
{
    DMA_Cmd(DMA1_Channel3, DISABLE );  //shut down USART1 TX DMA1 
    DMA_ClearFlag(DMA1_FLAG_TC3);    
 	  DMA_SetCurrDataCounter(DMA1_Channel3,24 * PIXEL_NUM );//buffsize of dma channel
 	  DMA_Cmd(DMA1_Channel3, ENABLE);  //enable USART1 TX DMA1 
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t ws281x_wheel(uint8_t wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return ws281x_color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    return ws281x_color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return ws281x_color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// Fill the dots one after the other with a color
void ws281x_colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<PIXEL_NUM; i++) {
    ws281x_setPixelColor(i, c);
    ws281x_show();
    delay_ms(wait);
  }
}


//roll the opposite way 
//doesnt enable the last led
void ws281x_colorWipe_rollback(uint32_t c, uint8_t wait){
  for(int i = PIXEL_NUM ; i > (-1); --i) {

    ws281x_setPixelColor(i, c);
	
    ws281x_show();
    delay_ms(wait);
  }
}



void ws281x_rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<PIXEL_NUM; i++) {
      ws281x_setPixelColor(i, ws281x_wheel((i+j) & 255));
    }
    ws281x_show();
    delay_ms(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void ws281x_rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< PIXEL_NUM; i++) {
      ws281x_setPixelColor(i,ws281x_wheel(((i * 256 / PIXEL_NUM) + j) & 255));
    }
    ws281x_show();
    delay_ms(wait);
  }
}

//Theatre-style crawling lights.
void ws281x_theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, c);    //turn every third pixel on
      }
      ws281x_show();

      delay_ms(wait);

      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void ws281x_theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, ws281x_wheel( (i+j) % 255));    //turn every third pixel on
      }
      ws281x_show();

      delay_ms(wait);

      for (uint16_t i=0; i < PIXEL_NUM; i=i+3) {
        ws281x_setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


//light up all the led
//点亮所有的灯
//c 三色设置
//wait 颜色存留时间
void LightUpALL(uint32_t c,uint8_t wait)
{
  //全部点亮
      for(uint8_t i = 0; i < PIXEL_NUM; i ++)
      {
        ws281x_setPixelColor(i, c);
      }
    ws281x_show();
    delay_ms(wait);
}



//红蓝交替
void Alternate_RedNBlue(void)
{
  	 
	for(uint8_t i = 0; i < 50; i ++)
	{
		LightUpALL(ws281x_color(i,0,0),5);
	}
	for(uint8_t i = 50; i > 0; i --)
	{
		LightUpALL(ws281x_color(i,0,0),5);
	}
	for(uint8_t i = 0; i < 50; i ++)
	{
		LightUpALL(ws281x_color(0,0,i),5);
	}
	for(uint8_t i = 50; i > 0; i --)
	{
		LightUpALL(ws281x_color(0,0,i),5);
	}
	ws281x_closeAll();
}



//num of led shows power quantity 也就是电量显示
void Show_PowerQuantity(uint8_t n)
{
	ws281x_closeAll();//必须关闭所有灯，否则有传输错误
	delay_us(800);
	for(uint8_t i = 0; i < n; i ++)
    {
        ws281x_setPixelColor(i, ws281x_color(10, 0, 5) );
    }
  ws281x_show();
	delay_us(800);
}

//超声测距颜色渐变
//假设超声测距最远距离2000
void LightUp_Distance(uint32_t distance)
{

	uint32_t s;
	for(uint8_t i = 0; i < PIXEL_NUM; i ++)
	{
		if(distance == 0)
		{
			ws281x_setPixelColor(i, ws281x_color(50, 0, 0) );
		}
		if(distance > 19890)
		{
			ws281x_setPixelColor(i, ws281x_color(0, 0,50) );
		}
//		if(distance > 13260 && distance < 19891)
//		{
//			ws281x_setPixelColor(i, ws281x_color(45, 5, 0) );
//		}
//		if(distance > 6630 && distance < 13261)
//		{
//			ws281x_setPixelColor(i, ws281x_color(0, 5, 45) );
//		}
		
		if(distance < 19891 && distance > 0)
		{
			s = (19890 - distance)/255;
			ws281x_setPixelColor(i, ws281x_color(s , (s / 2), 78 - s) );
		}

	}
	ws281x_show();
	delay_ms(12); 
      
	}	
  
    
