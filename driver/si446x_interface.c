/*
================================================================================
Function : Operation for SI446x
网址：http://shop57165217.taobao.com
作者：李勇  原  亿和电子工作室  现 亿佰特电子科技有限公司
TEL：18615799380, e-mail: yihe_liyong@126.com
================================================================================
*/
#include <stdint.h>
#include <stdbool.h>

#include <stm32f0xx.h>
#include "si446x.h"

/*
=================================================================================
SPI_ExchangeByte( );
Function : Exchange a byte via the SPI bus
INTPUT   : input, The input byte
OUTPUT   : The output byte from SPI bus
=================================================================================
*/
unsigned char SPI_ExchangeByte( unsigned char input )
{
#if 0
    //wait for last transmitt finishing
	while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
	SPI_SendData( input );
	//wait for receiving a byte
	while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
	return SPI_ReceiveData( );
#endif
	while(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) == RESET);
	SPI_SendData8(SPI1, input);
	while(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == RESET);
	return SPI_ReceiveData8(SPI1);
}
void SI_CSN_LOW()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}
void SI_CSN_HIGH()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}
void SI_SDN_HIGH()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}
void SI_SDN_LOW()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}
/*
=================================================================================
SPI_Initial( );
Function : Initialize the SPI bus
INTPUT   : None
OUTPUT   : None
=================================================================================
*/

void SPI_Initial( void )
{
#if 0
    //enable clock for SPI bus
	CLK_PeripheralClockConfig(CLK_Peripheral_SPI, ENABLE);
	//Set as default
	SPI_DeInit( );
	//Set the priority of the SPI
	SPI_Init( SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2,
            SPI_Mode_Master, SPI_CPOL_Low, SPI_CPHA_1Edge,
            SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft );
    //Enable SPi
	SPI_Cmd( ENABLE );
    //Config the GPIOs for SPI bus
	GPIO_Init( GPIOB, GPIO_Pin_7,GPIO_Mode_In_PU_No_IT );
	GPIO_Init( GPIOB, GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_Out_PP_High_Slow );
#endif
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the SPI periph */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	/* Enable SCK, MOSI, MISO and NSS GPIO clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOF, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);//sck
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);//miso
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);//mosi
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_0);//cs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

	/* SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI  MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	/* SPI NSS pin configuration */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* SPI configuration -------------------------------------------------------*/
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	/* Configure the SPI interrupt priority */
	//NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);

	/* Initializes the SPI communication */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Initialize the FIFO threshold */
	SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);

	/* Enable the Rx buffer not empty interrupt */
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	/* Enable the SPI Error interrupt */
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_ERR, ENABLE);
	/* Data transfer is performed in the SPI interrupt routine */
	SPI_SSOutputCmd(SPI1,ENABLE);
	//SPI_NSSPulseModeCmd(SPI1,ENABLE);
	/* Enable the SPI peripheral */
	SPI_Cmd(SPI1, ENABLE);

}
/*
=================================================================================
GPIO_Initial( );
Function : Initialize the other GPIOs of the board
INTPUT   : None
OUTPUT   : None
=================================================================================
*/
void GPIO_Initial( void )
{
#if 0
    //config the LED and CSN for RF module, CSN = PB4,LED = PB0
    GPIO_Init( GPIOB, GPIO_Pin_4|GPIO_Pin_0, GPIO_Mode_Out_PP_High_Slow );
    GPIO_SetBits( GPIOB, GPIO_Pin_4 | GPIO_Pin_0 );

    //config Pin: SDN for SI446X module, PC4
    GPIO_Init( GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast );

    //config the PC0-PC3, connected to GPIO0-GPIO3 of the SI446X module
    GPIO_Init( GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3,GPIO_Mode_In_PU_No_IT );

    //config the CS and RESET pin of the OLED module, PB2, PB3
    GPIO_Init( GPIOB, GPIO_Pin_2|GPIO_Pin_3,GPIO_Mode_Out_PP_High_Slow );
    GPIO_SetBits( GPIOB, GPIO_Pin_3);

    //Config the D/C pin of the OLED module, PD0
    GPIO_Init( GPIOD, GPIO_Pin_0,GPIO_Mode_Out_PP_High_Slow );
#endif
	GPIO_InitTypeDef  GPIO_InitStructure;
    /* Enable the GPIO_LED Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;


	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line3);

}
/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
