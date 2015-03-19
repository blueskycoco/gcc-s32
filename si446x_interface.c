/*
================================================================================
Function : Operation for SI446x
网址：http://shop57165217.taobao.com
作者：李勇  原  亿和电子工作室  现 亿佰特电子科技有限公司
TEL：18615799380, e-mail: yihe_liyong@126.com
================================================================================
*/
#include "board.h"

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
    //wait for last transmitt finishing
	while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
	SPI_SendData( input );
	//wait for receiving a byte
	while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
	return SPI_ReceiveData( );
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

}
/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
