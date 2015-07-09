#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stm32f0xx.h>
extern void cmx865a_init(void);
extern void delay_ms(unsigned short nms);
extern void delay_init(unsigned char SYSCLK);
extern void i2c_write (unsigned char addr, unsigned char* buf, int len) ;
#define rt_hw_led1_on()   GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define rt_hw_led1_off()  GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define UART1_GPIO_TX			GPIO_Pin_9
#define UART1_GPIO_TX_SOURCE	GPIO_PinSource9
#define UART1_GPIO_RX			GPIO_Pin_10
#define UART1_GPIO_RX_SOURCE	GPIO_PinSource10
#define UART1_GPIO_AF			GPIO_AF_1
#define UART1_GPIO				GPIOA
int pressed=0;
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
#if 1
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
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);
	USART_Cmd(USART1, ENABLE);
	return 0;
}
int uart_send(unsigned char byte)
{
	while (!(USART1->ISR & USART_FLAG_TXE));
	USART1->TDR = byte;
	return 0;
}
int uart_read()
{
	int ch = -1;
	if (USART1->ISR & USART_FLAG_RXNE)
	{
		ch = USART1->RDR & 0xff;
	}
	
	return ch;
}
void uart_wait_rcv()
{
	while(!(USART1->ISR & USART_FLAG_RXNE))
		delay_ms(1);
}
void led_init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    /* Enable the GPIO_LED Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF|RCC_AHBPeriph_GPIOA, ENABLE);
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    //GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_2);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	   	
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		
	GPIO_SetBits(GPIOA, GPIO_Pin_3);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOF, GPIO_Pin_1);
	GPIO_ResetBits(GPIOF, GPIO_Pin_0);
	delay_ms(10);
	GPIO_SetBits(GPIOF, GPIO_Pin_0);
}
void button_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;


	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line1);
}
void button_irq()
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==Bit_SET)
		pressed=1;
	else
		pressed=0;
}
int
main(void)
{
	unsigned char aic12k[10]={0x04,0x8a,0x04,0x01,0x05,0x30,0x70,0x06,0x02};	
	delay_init(48);
	uart_config();
	led_init(); 
	i2c_write(0x40,&aic12k[0],2);
	i2c_write(0x40,&aic12k[2],2);
	i2c_write(0x40,&aic12k[4],3);
	i2c_write(0x40,&aic12k[7],2);
	cmx865a_init();	
	//printf("system init\n");
	while(1)
    {
		rt_hw_led1_off();
		delay_ms(100);
		rt_hw_led1_on();
		delay_ms(100);
	}
}
