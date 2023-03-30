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
			//�ڽ���DAM��������ж��л�رն�ʱ��3
			//�ص���ʱ��3��Ϊ�˱������FFTת��δ���ʱ���ڽ���ADCת��
			//ת����ɱ�־����
			ADCC =0;
			
			Spectrum_Bounce();
			
			//���¿��ض�ʱ��
			TIM_Cmd(TIM3, ENABLE);
		}
	}
}


