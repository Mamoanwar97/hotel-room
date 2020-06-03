#include "GPIO.h"
#include "KeyPad.h"
#include "LCD.h"
#include "UART.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>

void SystemInit(){}

struct Room
{
    int status;
    int pin;
} rooms[100];


int rooms_num, y, j, key = -1;
unsigned char str[100];
char x, pin_checker[5];

int main()
{
	 Data_Port_Init();
   Control_Pins_Init();
	 UART_INIT(8, 9600);
	 keypad_init();
	 LCD_Init();
	
	pin_checker[4] = '\0';

	UART_SEND_STR("Enter your rooms number:");
	UART_RECIEVE_STR(str);
  rooms_num = atoi(str);
	
	for(j=0; j< rooms_num; j++)
    {
        rooms[j].status = 0;
        rooms[j].pin = 0;
    }
	
	pinMode(PortF , P2 ,Output);
	digitalWrite(PortF , P2 , low);
	
	while(1)
	{
		UART_SEND_STR("Choose your mood | '1' for reserve | '2' for unlock | '3' for clean | '4' for lock");
    UART_RECIEVE_STR(str);
		x = atoi(str);

        if( x == '1')
        {
						UART_SEND_STR("choose your room");
						UART_RECIEVE_STR(str);
            y = atoi(str);
            if( y <= rooms_num)
            {
                UART_SEND_STR("enter new 4 digit password");
								UART_RECIEVE_STR(str);
                rooms[y-1].pin = atoi(str)%10000;
                rooms[y-1].status = 1;
								digitalWrite(PortF , P2 , low);
            }
            else
                UART_SEND_STR("this room doesnot exist");
        }

        else if( x == '2')
        {
						int z;
            Display_String("choose your room");
						y = keypad_get_char();
            if( y <= rooms_num && rooms[y-1].status == 1)
            {
                int flag = 0;
                Display_String("enter your 4 digit password");
					
								for(j = 0; j < 4; j++)
                {
                    key = keypad_get_char();
										pin_checker[j] = key + '0';
                }
								
								z = atoi(pin_checker);
							
                if(rooms[y-1].pin != z)
									flag = 1;
                if(flag == 0)
									digitalWrite(PortF , P2 , high);
                else
									Display_String("Wrong password");

            }
            else if(y <= rooms_num && rooms[y-1].status == 0)
                Display_String("this room is locked");
            else
                Display_String("this room doesnot exist");
        }
        else if( x == '3')
        {
            UART_SEND_STR("choose your room");
						UART_RECIEVE_STR(str);
            y = atoi(str);
            if( y <= rooms_num)
						{
							digitalWrite(PortF , P2 , high);
              rooms[y-1].status = 2;
						}
            else
                UART_SEND_STR("this room doesnot exist");
        }
        else if( x == '4')
        {
            UART_SEND_STR("choose your room");
						UART_RECIEVE_STR(str);
            y = atoi(str);
            if( y <= rooms_num)
						{
							digitalWrite(PortF , P2 , low);
              rooms[y-1].status = 0;
						}
            else
                UART_SEND_STR("this room doesnot exist");
        }
	}
} 
