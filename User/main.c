#include "stm32f10x.h"
#include "MAX7219.h"
#include "adc.h"
#include "delay.h"
#include "time.h"
#include "adc.h"
#include "dma.h"
#include "FFT.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	SysTick_Init();
	DMA1_Channels1_Init(adcValues, NPT);
	ADC1_Init_In0();
	TIM3_Init(39, 71);
	MAX7219_Init();
	
	while(1)
	{
		if(ADCC == 1)
		{
			//在进入DAM接收完成中断中会关闭定时器3
			//关掉定时器3是为了避免进行FFT转换未完成时还在进行ADC转换
			//转换完成标志清零
			ADCC =0;
			
			Spectrum_Bounce();
			
			//重新开回定时器
			TIM_Cmd(TIM3, ENABLE);
		}
	}
}


