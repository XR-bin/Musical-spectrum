#include "stm32f10x.h"
#include "MAX7219.h"
#include "delay.h"

/*****************************************************
*函数功能  ：对MAX7219对应的GPIO口进行初始化设置
*函数名    ：MAX7219_GPIO_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            SCL   PA5   SPI1引脚
*            CS    PA6   普通引脚
*            SDA   PA7   SPI1引脚
********************************************************/
void MAX7219_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;  //GPIOx配置结构体
	SPI_InitTypeDef   SPI_InitStruct;      //SPIx配置结构体
	
	/*GPIOx设置*/
  //GPIOx时钟使能  GPIOA
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

	//PA6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				     //PA6 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					       //根据设定参数初始化PA6
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	
	/*GPIOx初始化设置*/
  //GPIOx端口配置
  //PA5  PA7
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5|GPIO_Pin_7;		//PA5、PA7 端口配置
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; 	  	    //复用推挽输
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		      //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);                    //根据设定参数初始化PA5、PA7
	
	/*SPI2寄存器初始化设置*/
	SPI_InitStruct.SPI_Direction         = SPI_Direction_1Line_Tx;   //SPI设置为双线双向全双工,且NSS上的电平为高电平(SPI_CR1寄存器的第10、15位)
	SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;                   //设置为主SPI(SPI_CR1寄存器的第2、8位)
	SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;                   //使用8位数据帧格式进行发送/接收(SPI_CR1寄存器的第11位)
	SPI_InitStruct.SPI_CPOL              = SPI_CPOL_Low;                      //空闲状态时， SCK保持低电平(SPI_CR1寄存器的第1位)
	SPI_InitStruct.SPI_CPHA              = SPI_CPHA_1Edge;                    //数据采样从第一个时钟边沿开始(SPI_CR1寄存器的第0位)
	SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;                      //启用软件从设备管理，NSS引脚上的电平由SSI位的值决定(SPI_CR1寄存器的第9位)
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;           //波特率预分频值为8  (SPI_CR1寄存器的第3-5位)
	SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;                  //数据传输从MSB位开始(对齐标准:左对齐)(SPI_CR1寄存器的第7位)
	SPI_InitStruct.SPI_CRCPolynomial     = 7;                                 //CRC值计算的多项式:0x7是复位值(SPI_CRCPR寄存器)
	SPI_Init(SPI1, &SPI_InitStruct);    
  SPI_Cmd(SPI1, ENABLE); //使能SPI外设(SPI_CR1寄存器的第6位)	//初始化SPI1
}

/******************************************************************
*函数功能  ：SPI1发送一个字节数据（8位）
*函数名    ：SPI1_TX_Byte
*函数参数  ：u8 data
*函数返回值：
*描述      ：
*******************************************************************/
void SPI1_TX_Byte(u8 data)
{
	//发送数据
	SPI_I2S_SendData(SPI1,data);
	//判断发送缓存区是否为空(SPI->SR寄存器的第1位)
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){};
}

//片选线
#define T_CS_H    GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define T_CS_L    GPIO_ResetBits(GPIOA,GPIO_Pin_6)

/******************************************************************
*函数功能  ：向MAX7219内某个寄存器发送一个8bit数据
*函数名    ：MAX7219_Data
*函数参数  ：u8 address, u8 data
*函数返回值：void
*描述      ：
*******************************************************************/
void MAX7219_Data(u8 address,u8 data)
{
	T_CS_L;
	SPI1_TX_Byte(address);     //发送地址
	SPI1_TX_Byte(data);        //发送数据
	T_CS_H;
}

/******************************************************************
*函数功能  ：向MAX7219写入显示数据
*函数名    ：MAX7219_Display
*函数参数  ：u8 address, u8 data
*函数返回值：void
*描述      ：
*******************************************************************/
void MAX7219_Display(u8 address,u8 data)
{
	u8 addr = address + 1;
	T_CS_L;
	SPI1_TX_Byte(addr);     //发送地址
	SPI1_TX_Byte(data);        //发送数据
	T_CS_H;
}


/*****************************************************
*函数功能  ：对MAX7219进行初始化设置
*函数名    ：MAX7219_Init
*函数参数  ：void
*函数返回值：void
*描述      ：
*            SCL   PA5   SPI1引脚
*            CS    PA6   普通引脚
*            SDA   PA7   SPI1引脚
********************************************************/
void MAX7219_Init(void)
{
	u8 i=0;

	MAX7219_GPIO_Init();
	
	MAX7219_Data(MAX7219_REG_SHUTDOWN, 0x01);     //MAX7219上电
	MAX7219_Data(MAX7219_REG_TEST, 0x00);         //关闭MAX7219显示测试模式
	MAX7219_Data(MAX7219_REG_DECODE,0x00);        //MAX7219不进行BCD译码
	MAX7219_Data(MAX7219_REG_INTENSITY, 0xFF);    //MAX7219亮度调到最高
	MAX7219_Data(MAX7219_REG_SCANLIMIT, 0x07);    //MAX7219动态扫描7个位
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




