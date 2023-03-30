#include "stm32f10x.h"

/************************************
*   F407��ADCʱ��Ƶ�ʹٷ�����: 36Mhz
*   F103��ADCʱ��Ƶ�ʹٷ�����: 14Mhz
*************************************/

/******************************************
*������    ��ADC1_Init_In0
*��������  ��ADC1�ĵ�0��ͨ����ʼ������
*��������  ��void
*��������ֵ��void 
*��������  ��
*          PA0----------ADC1_IN0   ģ��ģʽ
*********************************************/
void ADC1_Init_In0(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;     //GPIOx���ýṹ��
	ADC_InitTypeDef   ADC_InitStruct;      //ADCxһ�����ýṹ��
	
	//ʱ��ʹ��  GPIOA  ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	/*GPIOx��ʼ������*/
  //GPIOx�˿�����                         
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;          //PA0�˿�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;		  //ģ����������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStruct);					    //�����趨������ʼ��PA0
	
	ADC_DeInit(ADC1);                    //ADC1�ӿڸ�λ,������ADC1��ȫ���Ĵ�������Ϊȱʡֵ(RCC_APB2RSTR�Ĵ����ĵ�15λ)
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);    //�����߽���6��Ƶ12MHZ
	
	/*ADCx��ʼ������*/
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                    //����ģʽ(ADC_CR1�Ĵ����ĵ�16-19λ)
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;                         //��ɨ��(ADC_CR1�Ĵ����ĵ�8λ)
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                   //����ת��ģʽ(ADC_CR2�Ĵ����ĵ�1λ)
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;   //������ʽ��TIM3��TRGO�¼�(ADC_CR2�Ĵ����ĵ�17-19λ)
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                //���ݶ��룺�Ҷ���(ADC_CR2�Ĵ����ĵ�11λ)
  ADC_InitStruct.ADC_NbrOfChannel = 1;                               //�м���ת��(ADC_SQR1�Ĵ����ĵ�20-23λ)
  ADC_Init(ADC1, &ADC_InitStruct);                                   //ADC1��ʼ��
	
	//ADC����ͨ������ƫ�򼰲���ʱ�����ú���
  //ADC1�ĵ�һ��ת������ͨ��0������ʱ��1.5����    (������ADCx->SMPRx)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5); 
	
	ADC_DMACmd(ADC1,ENABLE);    //ʹ��adc��dma����
	
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);   //ʹ��adc�ⲿ��������
	
	//ADC1ʹ�ܺ���
  ADC_Cmd(ADC1, ENABLE);
	
	/*У׼����(���Ǳ����)*/ 
  //��λ��������ò���Ӳ���������У׼�Ĵ�������ʼ�����λ���������
  ADC_ResetCalibration(ADC1);                 //ʹ�ܸ�λУ׼
  while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ�У׼����
  //��λ����������Կ�ʼУ׼������У׼����ʱ��Ӳ�����
  ADC_StartCalibration(ADC1);                 //����ADCУ׼
  while(ADC_GetCalibrationStatus(ADC1));      //�ȴ�У׼����
}








