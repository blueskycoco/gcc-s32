/* MAIN.C file
 *
 * Copyright (c) 2002-2005 STMicroelectronics
 */
#include "board.h"
#include "si446x.h"
#include "Oled.h"

INT8U  buffer[64] = {'0','0','0','0','0',0};
int main( void )
{
    volatile INT8U i, j, length, error;
    INT16U rx_conter = 0, itmp;

    SPI_Initial( );
    GPIO_Initial( );

    LCD_Init( );            //OLED初始化
    LCD_Dis_Logo( );

    LCD_Dis_Str( 2,24,"STM8 board" );
    LCD_Dis_Str( 4,0,"yhmcu.taobao.com" );
    LCD_Dis_Str( 6,0,"SI446X:RX " );
    LCD_Dis_Str( 6, 80, (char*)buffer );

    SI446X_RESET( );        //SI446X 模块复位
    SI446X_CONFIG_INIT( );  //SI446X 模块初始化配置函数
    SI446X_START_RX( 0, 0, PACKET_LENGTH,0,0,3 );

    while( 1 )
    {
        SI446X_INT_STATUS( buffer );
        if( buffer[3] & ( 1<<4 ) )
        {
            //收到一个数据包，翻转LED
            length = SI446X_READ_PACKET( buffer );
            LED_Toggle( );


            //收到的数据是0-9，比较是否收到正确。
            for( i = 0, error = 0; i < 10; i ++ )
            {
                if( buffer[i] != i )
                {
                    error = 1;      //错误标记
                    break;
                }
            }
            //数据长度和内容都正确，进入IF
            if( length == 10 && error == 0 )
            {
                rx_conter ++;
                itmp = rx_conter;

                //Display the received count
                buffer[0] = ( itmp / 10000 ) + '0';
                itmp %= 10000;
                buffer[1] = ( itmp / 1000 ) + '0';
                itmp %= 1000;
                buffer[2] = ( itmp / 100 ) + '0';
                itmp %= 100;
                buffer[3] = ( itmp / 10 ) + '0';
                itmp %= 10;
                buffer[4] = itmp + '0';
                buffer[5] = 0;
                LCD_Dis_Str( 6, 80, (char*)buffer );

                //应答数据为10-19
                for( i = 0; i < 10; i ++ )
                {
                    buffer[i] = i + 10;
                }
                //产生应答信号,延时，等待发射方成功进入接收模式
                for( itmp = 0; itmp < 10000; itmp ++ );
                //变长或定长模式，在SI446X.H中配置
#if PACKET_LENGTH == 0   //变长模式，最大发射15字节,此处发10字节
                SI446X_SEND_PACKET( buffer, 10, 0, 0 );
#else                    //定长模式，长度取决于PACKET_LENGTH,最大64
                SI446X_SEND_PACKET( buffer, PACKET_LENGTH, 0, 0 );
#endif

                do{              //等待发射完成（中断产生）
                    SI446X_INT_STATUS( buffer );
                }while( !( buffer[3] & ( 1<<5 ) ) );

            }
            //回到接收模式，继续等待信号
            SI446X_START_RX( 0, 0, PACKET_LENGTH,0,0,3 );
        }
    }
}