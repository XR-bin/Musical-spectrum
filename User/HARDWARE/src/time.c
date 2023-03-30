#include "stm32f10x.h"

/*****************************************************
*函数功能  ：对定时器3进行初始化设置
*函数名    ：TIM3_Init
*函数参数  ：void
*函数返回值：u16 arr,u16 psc  
*描述      ：
********************************************************/
void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;  //TIMx配置结构体
	
	//TIM3时钟使能
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/*TIMx初始化设置*/
  TIM_TimeBaseInitStruct.TIM_Period        = arr;                 //重载值(TIMx_ARR寄存器)
  TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                 //分频(TIMx_PSC寄存器)
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //TDTS = Tck_tim(TIMx_CR1寄存器的第9-8位)(基本定时器没有)
  TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;  //向上计数(TIMx_CR1寄存器的第4位)(基本定时器只递增)
  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);                 //初始化TIM3
	
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);     //TIM3计数值更新将触发TRGO事件
	
	//使能重装载影子寄存器函数
	TIM_ARRPreloadConfig(TIM3, ENABLE);			// 使能TIM6重载寄存器ARR
	
	//使能TIMx
  TIM_Cmd(TIM3, ENABLE);
}





