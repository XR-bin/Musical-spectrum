#include "stm32f10x.h"

void DMA1_Channels1_Init(u32 *data, u16 ndtr)
{
	DMA_InitTypeDef   DMA_InitStruct;    //DMA���ýṹ��
	NVIC_InitTypeDef  NVIC_InitStruct;    //NVIC(�ж�)���ýṹ��
	
	//ʱ��ʹ��  DMA1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMA1ʱ��ʹ��
	
	//��ʼ��ͨ��������ΪĬ������
	DMA_DeInit(DMA1_Channel1);
	
	/*DMA����*/
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&ADC1->DR);           //DMA�����ַ��ADC1(DMA_CPARx�Ĵ���)
  DMA_InitStruct.DMA_MemoryBaseAddr = (u32)data;                      //DMA�洢��0��ַ������һ�������ַ(DMA_CMARx�Ĵ���)
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;                     //���赽�洢��ģʽ�����ݴ�������洢��������(DMA_CCRx�Ĵ����ĵ�4λ)
  DMA_InitStruct.DMA_BufferSize = ndtr;                               //���ݴ�����(DMA_CNDTRx�Ĵ���)
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;       //���������ģʽ�������ַָ��̶�(DMA_CCRx�Ĵ����ĵ�6λ)
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                //�洢������ģʽ��ÿ�����ݴ���󣬴洢����ַָ�����(DMA_CCRx�Ĵ����ĵ�7λ)
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݳ���16bit��ÿ�δ��������16bit����(DMA_CCRx�Ĵ����ĵ�8-9λ)
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;        //�洢�����ݳ���16bit��ÿ�δӴ洢������16bit����(DMA_CCRx�Ĵ����ĵ�10-11λ)
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                            //ѭ��ģʽѡ��ѭ��ģʽ(DMA_CCRx�Ĵ����ĵ�5λ)
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;                  //�����ȼ����ߵ����ȼ�(DMA_CCRx�Ĵ����ĵ�12-13λ)
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                           //�Ǵ洢�����洢��ģʽ(DMA_CCRx�Ĵ����ĵ�14λ)
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);//��ʼ��DMA
	
	NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE; // ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;      // ��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;      // �����ȼ�
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_Init(&NVIC_InitStruct);     // Ƕ�������жϿ�������ʼ��
	
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);     //������������ж�
	
	//������ʹ��
	DMA_Cmd(DMA1_Channel1, ENABLE);      //(DMA_SxCR�Ĵ����ĵ�0λ)
}




u8 ADCC = 0;   //ADCת����ɱ�־
//DMA1ͨ��1�����ж�
void DMA1_Channel1_IRQHandler(void)		
{
	//DMA��������ж�
	if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
	{
		TIM_Cmd(TIM3, DISABLE);
		ADCC = 1;
		DMA_ClearITPendingBit(DMA1_IT_TC1);   // �����������жϱ�־λ
	}
}







