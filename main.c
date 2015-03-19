#include <stdint.h>
#include <stdbool.h>
#include <stm32f0xx.h>
#include "socket.h"
#define rt_hw_led1_on()   GPIO_SetBits(GPIOF, GPIO_Pin_0)
#define rt_hw_led1_off()  GPIO_ResetBits(GPIOF, GPIO_Pin_0)
#define UART1_GPIO_TX			GPIO_Pin_2
#define UART1_GPIO_TX_SOURCE	GPIO_PinSource2
#define UART1_GPIO_RX			GPIO_Pin_3
#define UART1_GPIO_RX_SOURCE	GPIO_PinSource3
#define UART1_GPIO_AF			GPIO_AF_1
#define UART1_GPIO				GPIOA

int uart_config()
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* Connect PXx to USARTx_Tx */
	GPIO_PinAFConfig(UART1_GPIO, UART1_GPIO_TX_SOURCE, UART1_GPIO_AF);

	/* Connect PXx to USARTx_Rx */
	GPIO_PinAFConfig(UART1_GPIO, UART1_GPIO_RX_SOURCE, UART1_GPIO_AF);

	/* Configure USART Tx, Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = UART1_GPIO_TX | UART1_GPIO_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(UART1_GPIO, &GPIO_InitStructure);
#if 0
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	/* Enable USART */
	/* enable interrupt */
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//NVIC_EnableIRQ(USART1_IRQn);
	USART_Cmd(USART1, ENABLE);
	return 0;
}
int uart_send(unsigned char byte)
{
	while (!(USART1->ISR & USART_FLAG_TXE));
	USART1->TDR = byte;
}
void led_init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    /* Enable the GPIO_LED Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF|RCC_AHBPeriph_GPIOA, ENABLE);
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits( GPIOA, GPIO_Pin_9 );
	GPIO_SetBits( GPIOA, GPIO_Pin_10 );
}
//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
int
main(void)
{
	delay_init(48);
	uart_config();
	led_init();

    while(1)
    {
		rt_hw_led1_on();
		//uart_send('A');
		//ind_out(0);
		delay_ms(50);
		rt_hw_led1_off();
		//uart_send('B');
		//ind_out(1);
		delay_ms(50);
    }
}
