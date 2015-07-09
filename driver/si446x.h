

#ifndef _SI446X_H_
#define _SI446X_H_
#include "SI446X_defs.h"

/*
=================================================================================
-----------------------------External IMPORT functions---------------------------
=================================================================================
*/

//#include "board.h"   //BSP里面包含了Si446X所用的相关函数。
//#define SI_CSN_LOW( )   GPIO_ResetBits( GPIOB, GPIO_Pin_4 );
//#define SI_CSN_HIGH( )  GPIO_SetBits( GPIOB, GPIO_Pin_4 );

//#define SI_SDN_LOW( )   GPIO_ResetBits( GPIOC, GPIO_Pin_4 );
//#define SI_SDN_HIGH( )  GPIO_SetBits( GPIOC, GPIO_Pin_4 );

/*
=================================================================================
------------------------------INTERNAL EXPORT APIs-------------------------------
=================================================================================
*/

#define  PACKET_LENGTH      0 //0-64, if = 0: variable mode, else: fixed mode

void SI_CSN_LOW();
void SI_CSN_HIGH();
void SI_SDN_LOW();
void SI_SDN_HIGH();
unsigned char SPI_ExchangeByte( unsigned char input );
void SPI_Initial( void );
void GPIO_Initial( void );


/*
=================================================================================
------------------------------INTERNAL EXPORT APIs-------------------------------
=================================================================================
*/

/*Read the PART_INFO of the device, 8 bytes needed*/
void SI446X_PART_INFO( unsigned char *buffer );

/*Read the FUNC_INFO of the device, 7 bytes needed*/
void SI446X_FUNC_INFO( unsigned char *buffer );

/*Send a command to the device*/
void SI446X_CMD( unsigned char *cmd, unsigned char cmdsize );

/*Read the INT status of the device, 9 bytes needed*/
void SI446X_INT_STATUS( unsigned char *buffer );

/*Read the PROPERTY of the device*/
void SI446X_GET_PROPERTY_X( SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, unsigned char *buffer  );

/*configuration the device*/
void SI446X_CONFIG_INIT( void );

/*reset the SI446x device*/
void SI446X_RESET( void );

/*write data to TX fifo*/
void SI446X_W_TX_FIFO( unsigned char *txbuffer, unsigned char size );

/*start TX command*/
void SI446X_START_TX( unsigned char channel, unsigned char condition, unsigned int tx_len );

/*read RX fifo*/
unsigned char SI446X_READ_PACKET( unsigned char *buffer );

/*start RX state*/
void SI446X_START_RX( unsigned char channel, unsigned char condition, unsigned int rx_len,
                      unsigned char n_state1, unsigned char n_state2, unsigned char n_state3 );

/*read packet information*/
void SI446X_PKT_INFO( unsigned char *buffer, unsigned char FIELD, unsigned int length, unsigned int diff_len );

/*read fifo information*/
void SI446X_FIFO_INFO( unsigned char *buffer );

/*Power up the device*/
void SI446X_POWER_UP( unsigned long f_xtal );

/*send a packet*/
void SI446X_SEND_PACKET( unsigned char *txbuffer, unsigned char size, unsigned char channel, unsigned char condition );

/*Set the PROPERTY of the device*/
void SI446X_SET_PROPERTY_X( SI446X_PROPERTY GROUP_NUM, unsigned char NUM_PROPS, unsigned char *PAR_BUFF );

/*config the CRC, PROPERTY 0x1200*/
void SI446X_CRC_CONFIG( unsigned char PKT_CRC_CONFIG );

/*Get the PROPERTY of the device, only 1 byte*/
unsigned char SI446X_GET_PROPERTY_1( SI446X_PROPERTY GROUP_NUM );

/*Set the PROPERTY of the device, only 1 byte*/
void SI446X_SET_PROPERTY_1( SI446X_PROPERTY GROUP_NUM, unsigned char proirity );

/*config the GPIOs, IRQ, SDO*/
void SI446X_GPIO_CONFIG( unsigned char G0, unsigned char G1, unsigned char G2, unsigned char G3,
                         unsigned char IRQ, unsigned char SDO, unsigned char GEN_CONFIG );

/*reset the RX FIFO of the device*/
void SI446X_RX_FIFO_RESET( void );

/*reset the TX FIFO of the device*/
void SI446X_TX_FIFO_RESET( void );
void si446x_rw(unsigned char *buf,int len,unsigned char *out,int *out_len);
void si446x_rw(unsigned char *buf,int len,unsigned char *out,int *out_len);

/*
=================================================================================
----------------------------PROPERTY fast setting macros-------------------------
=================================================================================
*/
// GOLBAL(0x00)
#define GLOBAL_XO_TUNE( x )                 SI446X_SET_PROPERTY_1( 0x0000, x )
#define GLOBAL_CLK_CFG( x )                 SI446X_SET_PROPERTY_1( 0x0001, x )
#define GLOBAL_LOW_BATT_THRESH( x )         SI446X_SET_PROPERTY_1( 0x0002, x )
#define GLOBAL_CONFIG( x )                  SI446X_SET_PROPERTY_1( 0x0003, x )
#define GLOBAL_WUT_CONFIG( x )              SI446X_SET_PROPERTY_1( 0x0004, x )
#define GLOBAL_WUT_M_15_8( x )              SI446X_SET_PROPERTY_1( 0x0005, x )
#define GLOBAL_WUT_M_7_0( x )               SI446X_SET_PROPERTY_1( 0x0006, x )
#define GLOBAL_WUT_R( x )                   SI446X_SET_PROPERTY_1( 0x0007, x )
#define GLOBAL_WUT_LDC( x )                 SI446X_SET_PROPERTY_1( 0x0008, x )
#define GLOBAL_WUT_CAL( x )                 SI446X_SET_PROPERTY_1( 0x0009, x )

// INT_CTL(0x01)
#define INT_CTL_ENABLE( x )                 SI446X_SET_PROPERTY_1( 0x0100, x )
#define INT_CTL_PH_ENABLE( x )              SI446X_SET_PROPERTY_1( 0x0101, x )
#define INT_CTL_MODEM_ENABLE( x )           SI446X_SET_PROPERTY_1( 0x0102, x )
#define INT_CTL_CHIP_ENABLE( x )            SI446X_SET_PROPERTY_1( 0x0103, x )

//group 0x02, FRR_CTL
#define FRR_CTL_A_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0200, x )
#define FRR_CTL_B_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0201, x )
#define FRR_CTL_C_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0202, x )
#define FRR_CTL_D_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0203, x )

// PREAMBLE (0x10)









#endif //_SI446X_H_

/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
