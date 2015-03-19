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

    LCD_Init( );            //OLED��ʼ��
    LCD_Dis_Logo( );

    LCD_Dis_Str( 2,24,"STM8 board" );
    LCD_Dis_Str( 4,0,"yhmcu.taobao.com" );
    LCD_Dis_Str( 6,0,"SI446X:RX " );
    LCD_Dis_Str( 6, 80, (char*)buffer );

    SI446X_RESET( );        //SI446X ģ�鸴λ
    SI446X_CONFIG_INIT( );  //SI446X ģ���ʼ�����ú���
    SI446X_START_RX( 0, 0, PACKET_LENGTH,0,0,3 );

    while( 1 )
    {
        SI446X_INT_STATUS( buffer );
        if( buffer[3] & ( 1<<4 ) )
        {
            //�յ�һ�����ݰ�����תLED
            length = SI446X_READ_PACKET( buffer );
            LED_Toggle( );


            //�յ���������0-9���Ƚ��Ƿ��յ���ȷ��
            for( i = 0, error = 0; i < 10; i ++ )
            {
                if( buffer[i] != i )
                {
                    error = 1;      //������
                    break;
                }
            }
            //���ݳ��Ⱥ����ݶ���ȷ������IF
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

                //Ӧ������Ϊ10-19
                for( i = 0; i < 10; i ++ )
                {
                    buffer[i] = i + 10;
                }
                //����Ӧ���ź�,��ʱ���ȴ����䷽�ɹ��������ģʽ
                for( itmp = 0; itmp < 10000; itmp ++ );
                //�䳤�򶨳�ģʽ����SI446X.H������
#if PACKET_LENGTH == 0   //�䳤ģʽ�������15�ֽ�,�˴���10�ֽ�
                SI446X_SEND_PACKET( buffer, 10, 0, 0 );
#else                    //����ģʽ������ȡ����PACKET_LENGTH,���64
                SI446X_SEND_PACKET( buffer, PACKET_LENGTH, 0, 0 );
#endif

                do{              //�ȴ�������ɣ��жϲ�����
                    SI446X_INT_STATUS( buffer );
                }while( !( buffer[3] & ( 1<<5 ) ) );

            }
            //�ص�����ģʽ�������ȴ��ź�
            SI446X_START_RX( 0, 0, PACKET_LENGTH,0,0,3 );
        }
    }
}