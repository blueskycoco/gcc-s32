#include <stm32f0xx.h>
#define E2PROM_SDA GPIO_Pin_10
#define E2PROM_SCL GPIO_Pin_9
void DelayMs(int delay)
{
	volatile long i,j;
	for(i=0;i<delay*1000;i++)
		j=1;
}
unsigned char I2CReadByte( void )
{
  unsigned char i;
  unsigned char ReadValue = 0;
  GPIO_InitTypeDef GPIO_InitStruct;
  unsigned char bit;

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;  /*这里一定要设成输入上拉，否则不能读出数据*/
  GPIO_InitStruct.GPIO_Pin = E2PROM_SDA;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( GPIOA, &GPIO_InitStruct );
  for( i=0; i<8; i++ )
  {
    GPIO_SetBits( GPIOA, E2PROM_SCL );
    DelayMs(1);
    if( SET == GPIO_ReadInputDataBit( GPIOA, E2PROM_SDA ) )
      bit = 0X01;
    else
      bit = 0x00;
      
    ReadValue = (ReadValue<<1)|bit;
    GPIO_ResetBits( GPIOA, E2PROM_SCL );
    DelayMs(1);
  }

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_Init( GPIOA, &GPIO_InitStruct );
  return ReadValue;
}
void I2CWriteByte( unsigned char byte )
{
  unsigned char i;

  for( i=0; i<8; i++ )
  {
    if( 0X80 & byte )
      GPIO_SetBits( GPIOA, E2PROM_SDA );
    else
      GPIO_ResetBits( GPIOA, E2PROM_SDA );
    byte <<= 1;
    DelayMs(1);

    GPIO_SetBits( GPIOA, E2PROM_SCL );
    DelayMs(1);
    GPIO_ResetBits( GPIOA, E2PROM_SCL );
    DelayMs(1);
  }
}
unsigned char I2CSlaveAck( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  unsigned int TimeOut;
  unsigned char RetValue;

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;  /*这里一定要设成输入上拉，否则不能读出数据*/
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Pin = E2PROM_SDA;
  GPIO_Init( GPIOA, &GPIO_InitStruct );

  GPIO_SetBits( GPIOA, E2PROM_SCL );
  TimeOut = 10000;
  while( TimeOut-- > 0 )
  {
    if( SET == GPIO_ReadInputDataBit( GPIOA, E2PROM_SDA ) )
    {
      RetValue = RESET;
      break;
    }
    else
    {
      RetValue = SET;
    }
  }
  GPIO_ResetBits( GPIOA, E2PROM_SCL );
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_Init( GPIOA, &GPIO_InitStruct );
  return RetValue;
}
void I2CStop( void )
{
  GPIO_ResetBits( GPIOA, E2PROM_SDA );
  DelayMs(1);
  GPIO_SetBits( GPIOA, E2PROM_SCL );
  DelayMs(1);
  GPIO_SetBits( GPIOA, E2PROM_SDA );
  DelayMs(1);

  GPIO_ResetBits( GPIOA, E2PROM_SCL );
}
void I2CStart( void )
{
  GPIO_SetBits( GPIOA, E2PROM_SDA );
  DelayMs(1);
  GPIO_SetBits( GPIOA, E2PROM_SCL );
  DelayMs(1);
  GPIO_ResetBits( GPIOA, E2PROM_SDA );
  DelayMs(1);

  GPIO_ResetBits( GPIOA, E2PROM_SCL );
}
unsigned char E2promWriteByte( unsigned char addr, unsigned char data,unsigned char data1 )
{
  //asm("CPSID I");  //关中断

  I2CStart();

  I2CWriteByte( addr );
  if( RESET == I2CSlaveAck() )
  {
    return RESET;
  }
  I2CWriteByte( data );
  if( RESET == I2CSlaveAck() )
  {
    return RESET;
  }
  I2CWriteByte( data1 );
  if( RESET == I2CSlaveAck() )
  {
    return RESET;
  }
  I2CStop();
  //asm("CPSIE I");  //关中断

  return SET;
}
