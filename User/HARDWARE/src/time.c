#include "stm32f10x.h"

/*****************************************************
*��������  ���Զ�ʱ��3���г�ʼ������
*������    ��TIM3_Init
*��������  ��void
*��������ֵ��u16 arr,u16 psc  
*����      ��
********************************************************/
void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;  //TIMx���ýṹ��
	
	//TIM3ʱ��ʹ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/*TIMx��ʼ������*/
  TIM_TimeBaseInitStruct.TIM_Period        = arr;                 //����ֵ(TIMx_ARR�Ĵ���)
  TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                 //��Ƶ(TIMx_PSC�Ĵ���)
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //TDTS = Tck_tim(TIMx_CR1�Ĵ����ĵ�9-8λ)(������ʱ��û��)
  TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;  //���ϼ���(TIMx_CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ����)
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);                 //��ʼ��TIM3
	
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);     //TIM3����ֵ���½�����TRGO�¼�
	
	//ʹ����װ��Ӱ�ӼĴ�������
	TIM_ARRPreloadConfig(TIM3, ENABLE);			// ʹ��TIM6���ؼĴ���ARR
	
	//ʹ��TIMx
  TIM_Cmd(TIM3, ENABLE);
}





