#ifndef UART_H
#define UART_H
#include "common_macros.h"
#include "stdint.h"
#include "inc/tm4c123gh6pm.h"
typedef  float float32;

//#include "std_types.h"
#define SYS_CLK		16000000


void UART_INIT(uint8_t length, uint16_t baud_rate);

void UART_SEND_CHARACTER(uint8_t character);
void UART_SEND_STR(const char *Str);

uint8_t UART_RECIEVE_CHARACTER();
void UART_RECIEVE_STR(uint8_t *Str);


#endif
