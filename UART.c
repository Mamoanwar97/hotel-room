#include "UART.h"

void UART_INIT(uint8_t length, uint16_t baud_rate)
{

    uint16_t clk_div=(int)(SYS_CLK/(baud_rate*16));
    float32 fraction=(float)(SYS_CLK/(baud_rate*16))-clk_div;

    SET_BIT(SYSCTL_RCGCUART_R,0);																        //ENABLE CLK FOR UART0
    SET_BIT(SYSCTL_RCGCGPIO_R,0);																				//ENABLE CLK FOR PORTA

    CLEAR_BIT(UART0_CTL_R,0);																		        //DISABLE UART0

    UART0_IBRD_R = clk_div;																							//SET BR
    UART0_FBRD_R = (int)(fraction*64+0.5);															//SET BR
    UART0_CC_R = 0x0;																										//USE SYSTEM CLOCK
    UART0_LCRH_R = 0;
	  UART0_LCRH_R = (1 << 4) ;																						//set Buffer on
    UART0_LCRH_R = UART0_LCRH_R || ((length-5)<<5);											//SET DATA SIZE

    SET_BIT(UART0_CTL_R,8);																							//ENABLE RX
    SET_BIT(UART0_CTL_R,9);																							//ENABLE TX
    SET_BIT(UART0_CTL_R,0);																							//ENABLE UART0
    SET_BIT(GPIO_PORTA_AFSEL_R,0);
    SET_BIT(GPIO_PORTA_AFSEL_R,1);																			//ENABLE ALT FUN FOR PA0 & PA1

    GPIO_PORTA_PCTL_R = 0x11;																						//DEFINE ALT FUN CODE PA0 & PA1

    SET_BIT(GPIO_PORTA_DEN_R,0);
    SET_BIT(GPIO_PORTA_DEN_R,1);																				//ENABLE DIGITAL FOR PA0 & PA1

    CLEAR_BIT(GPIO_PORTA_AMSEL_R,0);
    CLEAR_BIT(GPIO_PORTA_AMSEL_R,1);																		//DISABLE ANALOG FOR PA0 & PA1
}


void UART_SEND_CHARACTER(uint8_t character)
{
    while(BIT_IS_SET(UART0_FR_R, 5)) {};
    UART0_DR_R = character;
}

uint8_t UART_RECIEVE_CHARACTER()
{
    uint8_t data;
    while(BIT_IS_SET(UART0_FR_R, 4)) {};
    data = UART0_DR_R;
    return data;
}

void UART_SEND_STR(const char *Message)
{
    uint8_t j = 0;
    while(Message[j] != '\0')
    {
        UART_SEND_CHARACTER(Message[j]);
        j++;
    }
}

void UART_RECIEVE_STR(uint8_t *Message)
{
    uint8_t j = 0;
    Message[j] = UART_RECIEVE_CHARACTER();
    while(Message[j] != '#')
    {
        j++;
        Message[j] = UART_RECIEVE_CHARACTER();
    }
    Message[j] = '\0';
}
