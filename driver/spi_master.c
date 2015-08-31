#include <stm32f0xx.h>
#include <stdio.h>
SPI_InitTypeDef  SPI_InitStructure;
unsigned char send_data=0;
void init_spi()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable the SPI periph */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	/* Enable SCK, MOSI, MISO and NSS GPIO clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);//sck
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);//miso
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);//mosi
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_0);//cs
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

	/* SPI SCK pin configuration */
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI  MOSI pin configuration */
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	//GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* SPI NSS pin configuration */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* SPI configuration -------------------------------------------------------*/
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;//SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	/* Configure the SPI interrupt priority */
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

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
unsigned char write_spi(unsigned char *data,int len)
{	
    int i=0;
	  GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	  for(i=0;i<len;i++)
	 {
		send_data=data[i];
		/* Enable the Tx buffer empty interrupt */
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);

		/* Waiting until TX FIFO is empty */
		while (SPI_GetTransmissionFIFOStatus(SPI1) != SPI_TransmissionFIFOStatus_Empty)
		{}
	}
	  GPIO_SetBits(GPIOA,GPIO_Pin_4);
}
void SPI1_IRQHandler(void)
{
unsigned char recv_data;
  /* SPI in Master Tramitter mode--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) == SET)
  {
      SPI_SendData8(SPI1, send_data);
      SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE);
     
  }
  
  /* SPI in Master Receiver mode--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET)
  {
    recv_data=SPI_ReceiveData8(SPI1);
  }
  
  /* SPI Error interrupt--------------------------------------- */
  if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_OVR) == SET)
  {
    SPI_ReceiveData8(SPI1);
    SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_OVR);
  }
}

