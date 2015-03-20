/*
================================================================================
Function : Operation for SI446x
http://shop57165217.taobao.com
================================================================================
*/
#include "si446x.h"
#include "radio_config_Si4461.h"
const static unsigned char config_table[] = RADIO_CONFIGURATION_DATA_ARRAY;

/*read a array of command response*/
void SI446X_READ_RESPONSE( unsigned char *buffer, unsigned char size );

/*wait the device ready to response a command*/
void SI446X_WAIT_CTS( void );

/*write data to TX fifo*/
void SI446X_W_TX_FIFO( unsigned char *txbuffer, unsigned char size );


/*
=================================================================================
SI446X_CMD();
Function : Send a command to the device
INTPUT   : cmd, the buffer stores the command array
           cmdsize, the size of the command array
OUTPUT   : NONE
=================================================================================
*/
void SI446X_CMD( unsigned char *cmd, unsigned char cmdsize )
{
    SI446X_WAIT_CTS();
    SI_CSN_LOW();
    while( cmdsize -- )
    {
        SPI_ExchangeByte( *cmd++ );
    }
    SI_CSN_HIGH();
}
/*
=================================================================================
SI446X_POWER_UP();
Function : Power up the device
INTPUT   : f_xtal, the frequency of the external high-speed crystal
OUTPUT   : NONE
=================================================================================
*/
void SI446X_POWER_UP( unsigned long f_xtal )
{
    unsigned char cmd[7];
    cmd[0] = POWER_UP;
    cmd[1] = 0x01;
    cmd[2] = 0x00;
    cmd[3] = f_xtal>>24;
    cmd[4] = f_xtal>>16;
    cmd[5] = f_xtal>>8;
    cmd[6] = f_xtal;
    SI446X_CMD( cmd, 7 );
}
/*
=================================================================================
SI446X_READ_RESPONSE();
Function : read a array of command response
INTPUT   : buffer,  a buffer, stores the data responsed
           size,    How many bytes should be read
OUTPUT   : NONE
=================================================================================
*/
void SI446X_READ_RESPONSE( unsigned char *buffer, unsigned char size )
{
    SI446X_WAIT_CTS();
    SI_CSN_LOW();
	SPI_ExchangeByte( READ_CMD_BUFF );
	while( size -- )
    {
        *buffer++ = SPI_ExchangeByte( 0xFF );
    }
    SI_CSN_HIGH();

}
/*
=================================================================================
SI446X_WAIT_CTS();
Function : wait the device ready to response a command
INTPUT   : NONE
OUTPUT   : NONE
=================================================================================
*/
void SI446X_WAIT_CTS( void )
{
    unsigned char cts;
    do
    {
        SI_CSN_LOW();
        SPI_ExchangeByte( READ_CMD_BUFF );
        cts = SPI_ExchangeByte( 0xFF );
        SI_CSN_HIGH();
    }while( cts != 0xFF );
}
/*
=================================================================================
SI446X_NOP();
Function : NO Operation command
INTPUT   : NONE
OUTPUT   : NONE
=================================================================================
*/
unsigned char SI446X_NOP( void )
{
    unsigned char cts;
    SI_CSN_LOW();
    cts = SPI_ExchangeByte( NOP );
    SI_CSN_HIGH();
	return cts;
}

/*
=================================================================================
SI446X_PART_INFO();
Function : Read the PART_INFO of the device, 8 bytes needed
INTPUT   : buffer, the buffer stores the part information
OUTPUT   : NONE
=================================================================================
*/
void SI446X_PART_INFO( unsigned char *buffer )
{
    unsigned char cmd = PART_INFO;

    SI446X_CMD( &cmd, 1 );
    SI446X_READ_RESPONSE( buffer, 8 );

}
/*
=================================================================================
SI446X_FUNC_INFO();
Function : Read the FUNC_INFO of the device, 7 bytes needed
INTPUT   : buffer, the buffer stores the FUNC information
OUTPUT   : NONE
=================================================================================
*/
void SI446X_FUNC_INFO( unsigned char *buffer )
{
    unsigned char cmd = FUNC_INFO;

    SI446X_CMD( &cmd, 1 );
    SI446X_READ_RESPONSE( buffer, 7 );
}
/*
=================================================================================
SI446X_INT_STATUS();
Function : Read the INT status of the device, 9 bytes needed
INTPUT   : buffer, the buffer stores the int status
OUTPUT   : NONE
=================================================================================
*/
void SI446X_INT_STATUS( unsigned char *buffer )
{
    unsigned char cmd[4];
    cmd[0] = GET_INT_STATUS;
    cmd[1] = 0;
    cmd[2] = 0;
    cmd[3] = 0;

    SI446X_CMD( cmd, 4 );
    SI446X_READ_RESPONSE( buffer, 9 );

}
/*
=================================================================================
SI446X_GET_PROPERTY();
Function : Read the PROPERTY of the device
INTPUT   : buffer, the buffer stores the PROPERTY value
           GROUP_NUM, the group and number of the parameter
           NUM_GROUP, number of the group
OUTPUT   : NONE
=================================================================================
*/
void SI446X_GET_PROPERTY_X( SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, unsigned char *buffer  )
{
    unsigned char cmd[4];

    cmd[0] = GET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = NUM_PROPS;
    cmd[3] = GROUP_NUM;

    SI446X_CMD( cmd, 4 );
    SI446X_READ_RESPONSE( buffer, NUM_PROPS + 1 );
}
/*
=================================================================================
SI446X_SET_PROPERTY_X();
Function : Set the PROPERTY of the device
INTPUT   : GROUP_NUM, the group and the number of the parameter
           NUM_GROUP, number of the group
           PAR_BUFF, buffer stores parameters
OUTPUT   : NONE
=================================================================================
*/
void SI446X_SET_PROPERTY_X( SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, unsigned char *PAR_BUFF )
{
    unsigned char cmd[20], i = 0;
    if( NUM_PROPS >= 16 )   { return; }
    cmd[i++] = SET_PROPERTY;
    cmd[i++] = GROUP_NUM>>8;
    cmd[i++] = NUM_PROPS;
    cmd[i++] = GROUP_NUM;
    while( NUM_PROPS-- )
    {
        cmd[i++] = *PAR_BUFF++;
    }
    SI446X_CMD( cmd, i );
}
/*
=================================================================================
SI446X_SET_PROPERTY_1();
Function : Set the PROPERTY of the device, only 1 byte
INTPUT   : GROUP_NUM, the group and number index
           prioriry,  the value to be set
OUTPUT   : NONE
=================================================================================
*/
void SI446X_SET_PROPERTY_1( SI446X_PROPERTY GROUP_NUM, unsigned char proirity )
{
    unsigned char cmd[5];

    cmd[0] = SET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    cmd[4] = proirity;
    SI446X_CMD( cmd, 5 );
}
/*
=================================================================================
SI446X_GET_PROPERTY_1();
Function : Get the PROPERTY of the device, only 1 byte
INTPUT   : GROUP_NUM, the group and number index
OUTPUT   : the PROPERTY value read from device
=================================================================================
*/
unsigned char SI446X_GET_PROPERTY_1( SI446X_PROPERTY GROUP_NUM )
{
    unsigned char cmd[4];

    cmd[0] = GET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    SI446X_CMD( cmd, 4 );
    SI446X_READ_RESPONSE( cmd, 2 );
    return cmd[1];
}
/*
=================================================================================
SI446X_RESET();
Function : reset the SI446x device
INTPUT   : NONE
OUTPUT   : NONE
=================================================================================
*/
void SI446X_RESET( void )
{
    unsigned int x = 255;	
	GPIO_Initial();
    SI_SDN_HIGH();
    while( x-- );
    SI_SDN_LOW();
    SI_CSN_HIGH();
}
/*
=================================================================================
SI446X_CONFIG_INIT();
Function : configuration the device
INTPUT   : NONE
OUTPUT   : NONE
=================================================================================
*/
void SI446X_CONFIG_INIT( void )
{
    unsigned char i;
    unsigned int j = 0;
	SPI_Initial();
    while( ( i = config_table[j] ) != 0 )
    {
        j += 1;
        SI446X_CMD( config_table + j, i );
        j += i;
    }
#if PACKET_LENGTH > 0           //fixed packet length
    SI446X_SET_PROPERTY_1( PKT_FIELD_1_LENGTH_7_0, PACKET_LENGTH );
#else                           //variable packet length
    SI446X_SET_PROPERTY_1( PKT_CONFIG1, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_CRC_CONFIG, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_LEN_FIELD_SOURCE, 0x01 );
    SI446X_SET_PROPERTY_1( PKT_LEN, 0x2A );
    SI446X_SET_PROPERTY_1( PKT_LEN_ADJUST, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_1_LENGTH_12_8, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_1_LENGTH_7_0, 0x01 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_1_CONFIG, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_1_CRC_CONFIG, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_2_LENGTH_12_8, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_2_LENGTH_7_0, 0x10 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_2_CONFIG, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_2_CRC_CONFIG, 0x00 );
#endif //PACKET_LENGTH

    //重要： 4463的GDO2，GDO3控制射频开关，  33 ,32 
    //发射时必须： GDO2=0，GDO3=1
    //接收时必须： GDO2=1，GDO3=0
    //所以，以下语句对于4463是必须的，4460或4461不需要射频开关，则可以去掉此语句
    SI446X_GPIO_CONFIG( 0, 0, 33|0x40, 32|0x40, 0, 0, 0 );    //重要
    //SI446X_GPIO_CONFIG( 0, 0, 0x43, 0x42, 0, 0, 0 );

}
/*
=================================================================================
SI446X_W_TX_FIFO();
Function : write data to TX fifo
INTPUT   : txbuffer, a buffer stores TX array
           size,  how many bytes should be written
OUTPUT   : NONE
=================================================================================
*/
void SI446X_W_TX_FIFO( unsigned char *txbuffer, unsigned char size )
{
    SI_CSN_LOW();
    SPI_ExchangeByte( WRITE_TX_FIFO );
    while( size -- )    { SPI_ExchangeByte( *txbuffer++ ); }
    SI_CSN_HIGH();
}
/*
=================================================================================
SI446X_SEND_PACKET();
Function : send a packet
INTPUT   : txbuffer, a buffer stores TX array
           size,  how many bytes should be written
           channel, tx channel
           condition, tx condition
OUTPUT   : NONE
=================================================================================
*/
void SI446X_SEND_PACKET( unsigned char *txbuffer, unsigned char size, unsigned char channel, unsigned char condition )
{
    unsigned char cmd[5];
    unsigned char tx_len = size;

    SI446X_TX_FIFO_RESET();

    SI_CSN_LOW();
    SPI_ExchangeByte( WRITE_TX_FIFO );
#if PACKET_LENGTH == 0
    tx_len ++;
    SPI_ExchangeByte( size );
#endif
    while( size -- )    { SPI_ExchangeByte( *txbuffer++ ); }
    SI_CSN_HIGH();
    cmd[0] = START_TX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = 0;
    cmd[4] = tx_len;
    SI446X_CMD( cmd, 5 );
}
/*
=================================================================================
SI446X_START_TX();
Function : start TX command
INTPUT   : channel, tx channel
           condition, tx condition
           tx_len, how many bytes to be sent
OUTPUT   : NONE
=================================================================================
*/
void SI446X_START_TX( unsigned char channel, unsigned char condition, unsigned int tx_len )
{
    unsigned char cmd[5];

    cmd[0] = START_TX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = tx_len>>8;
    cmd[4] = tx_len;
    SI446X_CMD( cmd, 5 );
}
/*
=================================================================================
SI446X_READ_PACKET();
Function : read RX fifo
INTPUT   : buffer, a buffer to store data read
OUTPUT   : received bytes
=================================================================================
*/
unsigned char SI446X_READ_PACKET( unsigned char *buffer )
{
    unsigned char length, i;
    SI446X_WAIT_CTS();
    SI_CSN_LOW();

    SPI_ExchangeByte( READ_RX_FIFO );
#if PACKET_LENGTH == 0
    length = SPI_ExchangeByte( 0xFF );
#else
    length = PACKET_LENGTH;
#endif
    i = length;

    while( length -- )
    {
        *buffer++ = SPI_ExchangeByte( 0xFF );
    }
    SI_CSN_HIGH();
    return i;
}
/*
=================================================================================
SI446X_START_RX();
Function : start RX state
INTPUT   : channel, receive channel
           condition, receive condition
           rx_len, how many bytes should be read
           n_state1, next state 1
           n_state2, next state 2
           n_state3, next state 3
OUTPUT   : NONE
=================================================================================
*/
void SI446X_START_RX( unsigned char channel, unsigned char condition, unsigned int rx_len,
                      unsigned char n_state1, unsigned char n_state2, unsigned char n_state3 )
{
    unsigned char cmd[8];
    SI446X_RX_FIFO_RESET();
    SI446X_TX_FIFO_RESET();
    cmd[0] = START_RX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = rx_len>>8;
    cmd[4] = rx_len;
    cmd[5] = n_state1;
    cmd[6] = n_state2;
    cmd[7] = n_state3;
    SI446X_CMD( cmd, 8 );
}
/*
=================================================================================
SI446X_RX_FIFO_RESET();
Function : reset the RX FIFO of the device
INTPUT   : None
OUTPUT   : NONE
=================================================================================
*/
void SI446X_RX_FIFO_RESET( void )
{
    unsigned char cmd[2];

    cmd[0] = FIFO_INFO;
    cmd[1] = 0x02;
    SI446X_CMD( cmd, 2 );
}
/*
=================================================================================
SI446X_TX_FIFO_RESET();
Function : reset the TX FIFO of the device
INTPUT   : None
OUTPUT   : NONE
=================================================================================
*/
void SI446X_TX_FIFO_RESET( void )
{
    unsigned char cmd[2];

    cmd[0] = FIFO_INFO;
    cmd[1] = 0x01;
    SI446X_CMD( cmd, 2 );
}
/*
=================================================================================
SI446X_PKT_INFO();
Function : read packet information
INTPUT   : buffer, stores the read information
           FIELD, feild mask
           length, the packet length
           diff_len, diffrence packet length
OUTPUT   : NONE
=================================================================================
*/
void SI446X_PKT_INFO( unsigned char *buffer, unsigned char FIELD, unsigned int length, unsigned int diff_len )
{
    unsigned char cmd[6];
    cmd[0] = PACKET_INFO;
    cmd[1] = FIELD;
    cmd[2] = length >> 8;
    cmd[3] = length;
    cmd[4] = diff_len >> 8;
    cmd[5] = diff_len;

    SI446X_CMD( cmd, 6 );
    SI446X_READ_RESPONSE( buffer, 3 );
}
/*
=================================================================================
SI446X_FIFO_INFO();
Function : read fifo information
INTPUT   : buffer, stores the read information
OUTPUT   : NONE
=================================================================================
*/
void SI446X_FIFO_INFO( unsigned char *buffer )
{
    unsigned char cmd[2];
    cmd[0] = FIFO_INFO;
    cmd[1] = 0x03;

    SI446X_CMD( cmd, 2 );
    SI446X_READ_RESPONSE( buffer, 3);
}
/*
=================================================================================
SI446X_GPIO_CONFIG();
Function : config the GPIOs, IRQ, SDO
INTPUT   :
OUTPUT   : NONE
=================================================================================
*/
void SI446X_GPIO_CONFIG( unsigned char G0, unsigned char G1, unsigned char G2, unsigned char G3,
                         unsigned char IRQ, unsigned char SDO, unsigned char GEN_CONFIG )
{
    unsigned char cmd[10];
    cmd[0] = GPIO_PIN_CFG;
    cmd[1] = G0;
    cmd[2] = G1;
    cmd[3] = G2;
    cmd[4] = G3;
    cmd[5] = IRQ;
    cmd[6] = SDO;
    cmd[7] = GEN_CONFIG;
    SI446X_CMD( cmd, 8 );
    SI446X_READ_RESPONSE( cmd, 8 );
}
/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
