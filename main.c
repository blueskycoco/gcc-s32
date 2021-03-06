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

void delay(int times)
{
	volatile long i,j;
	int m;
	for(m=0;m<times;m++)
	for(i=0;i<100000;i++)
		j=0;
}
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

	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	/* Enable USART */
	/* enable interrupt */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);
	USART_Cmd(USART1, ENABLE);
	return 0;
}
int uart_send(unsigned char byte)
{
	while (!(USART1->ISR & USART_FLAG_TXE));
	USART1->TDR = byte;
}
char uart_recv(char *dat)
{
	char ch;
	ch = -1;
	if (USART1->ISR & USART_FLAG_RXNE)
	{
		*dat = USART1->RDR & 0xff;
		
	}
	return ch;
}
void USART1_IRQHandler(void)
{
	int ch=-1;
	char dat;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		while (1)
		{
			ch = uart_recv(&dat);
			if (ch == 0xff)
			{
				uart_send(dat);
				break;
			}
		}
		/* clear interrupt */
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		/* clear interrupt */
		USART_ClearITPendingBit(USART1, USART_IT_TC);
	}
}
void led_init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    /* Enable the GPIO_LED Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF|RCC_AHBPeriph_GPIOA, ENABLE);
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
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
	#if 0
	ind_cnn_init();	
	config conf;
	conf.local_ip[0]=192;
	conf.local_ip[1]=168;
	conf.local_ip[2]=1;
	conf.local_ip[3]=13;
	conf.local_port[0]=11;
	conf.local_port[1]=34;
	conf.sub_msk[0]=255;
	conf.sub_msk[1]=255;
	conf.sub_msk[2]=255;
	conf.sub_msk[3]=0;
	conf.gw[0]=192;
	conf.gw[1]=168;
	conf.gw[2]=1;
	conf.gw[3]=1;
	conf.mac[0]=0xe3;
	conf.mac[1]=0x90;
	conf.mac[2]=0x12;
	conf.mac[3]=0x2d;
	conf.mac[4]=0xef;
	conf.mac[5]=0x33;
	conf.remote_ip[0]=192;
	conf.remote_ip[1]=168;
	conf.remote_ip[2]=1;
	conf.remote_ip[3]=122;
	conf.remote_port[0]=0xef;
	conf.remote_port[1]=33;
	conf.protol=NET_PROTOL_TCP;
	conf.server_mode=CLIENT_MODE;
	conf.uart_baud=BAUD_115200;
	ind_out(1);
	delay(5);
	config_param(CONFIG_LOCAL_IP,&conf);
	config_param(CONFIG_LOCAL_PORT,&conf);
	config_param(CONFIG_GW,&conf);
	config_param(CONFIG_SUB_MSK,&conf);
	config_param(CONFIG_MAC,&conf);
	config_param(CONFIG_REMOTE_IP,&conf);
	config_param(CONFIG_REMOTE_PORT,&conf);
	config_param(CONFIG_PROTOL,&conf);
	config_param(CONFIG_SERVER_MODE,&conf);
	config_param(CONFIG_UART_BAUD,&conf);
	//read_param();
	delay(5);
	ind_out(0);
	uart_send('A');
	#endif
	GPIO_SetBits(GPIOF, GPIO_Pin_0);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	delay_ms(100);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	delay_ms(100);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	delay_ms(100);
	E2promWriteByte(0x40,0x04,0x8a);
	E2promWriteByte(0x40,0x04,0x01);
    //
    // Loop forever echoing data through the UART.
    //

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
