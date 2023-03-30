#include "stm32f10x.h"

void DMA1_Channels1_Init(u32 *data, u16 ndtr)
{
	DMA_InitTypeDef   DMA_InitStruct;    //DMA配置结构体
	NVIC_InitTypeDef  NVIC_InitStruct;    //NVIC(中断)配置结构体
	
	//时钟使能  DMA1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMA1时钟使能
	
	//初始化通道，设置为默认配置
	DMA_DeInit(DMA1_Channel1);
	
	/*DMA配置*/
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&ADC1->DR);           //DMA外设地址：ADC1(DMA_CPARx寄存器)
  DMA_InitStruct.DMA_MemoryBaseAddr = (u32)data;                      //DMA存储器0地址：定义一个数组地址(DMA_CMARx寄存器)
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;                     //外设到存储器模式：数据从外设向存储器方向传输(DMA_CCRx寄存器的第4位)
  DMA_InitStruct.DMA_BufferSize = ndtr;                               //数据传输量(DMA_CNDTRx寄存器)
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;       //外设非增量模式：外设地址指针固定(DMA_CCRx寄存器的第6位)
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                //存储器增量模式：每次数据传输后，存储器地址指针递增(DMA_CCRx寄存器的第7位)
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据长度16bit：每次从外设搬运16bit数据(DMA_CCRx寄存器的第8-9位)
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;        //存储器数据长度16bit：每次从存储器运输16bit数据(DMA_CCRx寄存器的第10-11位)
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                            //循环模式选择：循环模式(DMA_CCRx寄存器的第5位)
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;                  //高优先级：高等优先级(DMA_CCRx寄存器的第12-13位)
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                           //非存储器到存储器模式(DMA_CCRx寄存器的第14位)
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);//初始化DMA
	
	NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE; // 使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;      // 抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;      // 子优先级
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_Init(&NVIC_InitStruct);     // 嵌套向量中断控制器初始化
	
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);     //启动传输完成中断
	
	//数据流使能
	DMA_Cmd(DMA1_Channel1, ENABLE);      //(DMA_SxCR寄存器的第0位)
}




u8 ADCC = 0;   //ADC转换完成标志
//DMA1通道1接收中断
void DMA1_Channel1_IRQHandler(void)		
{
	//DMA接收完成中断
	if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
	{
		TIM_Cmd(TIM3, DISABLE);
		ADCC = 1;
		DMA_ClearITPendingBit(DMA1_IT_TC1);   // 清除传输完成中断标志位
	}
}







