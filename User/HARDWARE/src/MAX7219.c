#include "stm32f10x.h"
#include "MAX7219.h"
#include "delay.h"

/*****************************************************
*��������  ����MAX7219��Ӧ��GPIO�ڽ��г�ʼ������
*������    ��MAX7219_GPIO_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            SCL   PA5   SPI1����
*            CS    PA6   ��ͨ����
*            SDA   PA7   SPI1����
********************************************************/
void MAX7219_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;  //GPIOx���ýṹ��
	SPI_InitTypeDef   SPI_InitStruct;      //SPIx���ýṹ��
	
	/*GPIOx����*/
  //GPIOxʱ��ʹ��  GPIOA
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

	//PA6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				     //PA6 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					       //�����趨������ʼ��PA6
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	
	/*GPIOx��ʼ������*/
  //GPIOx�˿�����
  //PA5  PA7
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5|GPIO_Pin_7;		//PA5��PA7 �˿�����
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; 	  	    //����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		      //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);                    //�����趨������ʼ��PA5��PA7
	
	/*SPI2�Ĵ�����ʼ������*/
	SPI_InitStruct.SPI_Direction         = SPI_Direction_1Line_Tx;   //SPI����Ϊ˫��˫��ȫ˫��,��NSS�ϵĵ�ƽΪ�ߵ�ƽ(SPI_CR1�Ĵ����ĵ�10��15λ)
	SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;                   //����Ϊ��SPI(SPI_CR1�Ĵ����ĵ�2��8λ)
	SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;                   //ʹ��8λ����֡��ʽ���з���/����(SPI_CR1�Ĵ����ĵ�11λ)
	SPI_InitStruct.SPI_CPOL              = SPI_CPOL_Low;                      //����״̬ʱ�� SCK���ֵ͵�ƽ(SPI_CR1�Ĵ����ĵ�1λ)
	SPI_InitStruct.SPI_CPHA              = SPI_CPHA_1Edge;                    //���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ(SPI_CR1�Ĵ����ĵ�0λ)
	SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;                      //����������豸����NSS�����ϵĵ�ƽ��SSIλ��ֵ����(SPI_CR1�Ĵ����ĵ�9λ)
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;           //������Ԥ��ƵֵΪ8  (SPI_CR1�Ĵ����ĵ�3-5λ)
	SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;                  //���ݴ����MSBλ��ʼ(�����׼:�����)(SPI_CR1�Ĵ����ĵ�7λ)
	SPI_InitStruct.SPI_CRCPolynomial     = 7;                                 //CRCֵ����Ķ���ʽ:0x7�Ǹ�λֵ(SPI_CRCPR�Ĵ���)
	SPI_Init(SPI1, &SPI_InitStruct);    
  SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����(SPI_CR1�Ĵ����ĵ�6λ)	//��ʼ��SPI1
}

/******************************************************************
*��������  ��SPI1����һ���ֽ����ݣ�8λ��
*������    ��SPI1_TX_Byte
*��������  ��u8 data
*��������ֵ��
*����      ��
*******************************************************************/
void SPI1_TX_Byte(u8 data)
{
	//��������
	SPI_I2S_SendData(SPI1,data);
	//�жϷ��ͻ������Ƿ�Ϊ��(SPI->SR�Ĵ����ĵ�1λ)
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){};
}

//Ƭѡ��
#define T_CS_H    GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define T_CS_L    GPIO_ResetBits(GPIOA,GPIO_Pin_6)

/******************************************************************
*��������  ����MAX7219��ĳ���Ĵ�������һ��8bit����
*������    ��MAX7219_Data
*��������  ��u8 address, u8 data
*��������ֵ��void
*����      ��
*******************************************************************/
void MAX7219_Data(u8 address,u8 data)
{
	T_CS_L;
	SPI1_TX_Byte(address);     //���͵�ַ
	SPI1_TX_Byte(data);        //��������
	T_CS_H;
}

/******************************************************************
*��������  ����MAX7219д����ʾ����
*������    ��MAX7219_Display
*��������  ��u8 address, u8 data
*��������ֵ��void
*����      ��
*******************************************************************/
void MAX7219_Display(u8 address,u8 data)
{
	u8 addr = address + 1;
	T_CS_L;
	SPI1_TX_Byte(addr);     //���͵�ַ
	SPI1_TX_Byte(data);        //��������
	T_CS_H;
}


/*****************************************************
*��������  ����MAX7219���г�ʼ������
*������    ��MAX7219_Init
*��������  ��void
*��������ֵ��void
*����      ��
*            SCL   PA5   SPI1����
*            CS    PA6   ��ͨ����
*            SDA   PA7   SPI1����
********************************************************/
void MAX7219_Init(void)
{
	u8 i=0;

	MAX7219_GPIO_Init();
	
	MAX7219_Data(MAX7219_REG_SHUTDOWN, 0x01);     //MAX7219�ϵ�
	MAX7219_Data(MAX7219_REG_TEST, 0x00);         //�ر�MAX7219��ʾ����ģʽ
	MAX7219_Data(MAX7219_REG_DECODE,0x00);        //MAX7219������BCD����
	MAX7219_Data(MAX7219_REG_INTENSITY, 0xFF);    //MAX7219���ȵ������
	MAX7219_Data(MAX7219_REG_SCANLIMIT, 0x07);    //MAX7219��̬ɨ��7��λ
	for(i=0; i<8; i++)
	{
		MAX7219_Display(i, 0x0);
	}
	
	for(i = 0; i < 8; i++)
	{
			MAX7219_Display(i,0XFF);
			delay_ms(90);
	}
}




