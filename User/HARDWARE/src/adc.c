#include "stm32f10x.h"

/************************************
*   F407的ADC时钟频率官方建议: 36Mhz
*   F103的ADC时钟频率官方建议: 14Mhz
*************************************/

/******************************************
*函数名    ：ADC1_Init_In0
*函数功能  ：ADC1的第0个通道初始化配置
*函数参数  ：void
*函数返回值：void 
*函数描述  ：
*          PA0----------ADC1_IN0   模拟模式
*********************************************/
void ADC1_Init_In0(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx配置结构体
	ADC_InitTypeDef   ADC_InitStruct;      //ADCx一般配置结构体
	
	//时钟使能  GPIOA  ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	/*GPIOx初始化设置*/
  //GPIOx端口配置                         
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;          //PA0端口配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;		  //模拟输入引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStruct);					    //根据设定参数初始化PA0
	
	ADC_DeInit(ADC1);                    //ADC1接口复位,将外设ADC1的全部寄存器重设为缺省值(RCC_APB2RSTR寄存器的第15位)
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);    //对总线进行6分频12MHZ
	
	/*ADCx初始化设置*/
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                    //独立模式(ADC_CR1寄存器的第16-19位)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                         //非扫描(ADC_CR1寄存器的第8位)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                   //单次转换模式(ADC_CR2寄存器的第1位)
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;   //触发方式：TIM3的TRGO事件(ADC_CR2寄存器的第17-19位)
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                //数据对齐：右对齐(ADC_CR2寄存器的第11位)
  ADC_InitStruct.ADC_NbrOfChannel = 1;                               //有几个转换(ADC_SQR1寄存器的第20-23位)
  ADC_Init(ADC1, &ADC_InitStruct);                                   //ADC1初始化
	
	//ADC规则通道序列偏序及采样时间配置函数
  //ADC1的第一个转换的是通道0，采样时间1.5周期    (采样：ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5); 
	
	ADC_DMACmd(ADC1,ENABLE);    //使能adc的dma传输
	
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);   //使能adc外部触发启动
	
	//ADC1使能函数
  ADC_Cmd(ADC1, ENABLE);
	
	/*校准部分(这是必须的)*/ 
  //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。
  ADC_ResetCalibration(ADC1);                 //使能复位校准
  while(ADC_GetResetCalibrationStatus(ADC1)); //等待校准结束
  //该位由软件设置以开始校准，并在校准结束时由硬件清除
  ADC_StartCalibration(ADC1);                 //开启ADC校准
  while(ADC_GetCalibrationStatus(ADC1));      //等待校准结束
}








