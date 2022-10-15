#ifndef MYUART_H_
#define MYUART_H_
//our baud rate is 9600, so to get Clock divisor:
//  16MHZ/(16*9600) = 104.166666666667
//So IntBaud is 104
#define IntBaud 104
//#define IntBaud 325
// fractionBaud :
// round((16667*64)+0.5) = 11
//#define FractionBaud 34
#define FractionBaud 11
/////////////////////////////////////
//funcition defs
void init_UART(void);
void UART1_Init(void);
void UART4_Init(void);

char UART0_CharGet(void);
void UART0_SendChar(char c);

char UART1_GetChar(void);
void UART1_SendChar(char c);

char UART4_GetChar(void);
void UART4_SendChar(char c);

void InitLEDs(void);
void UART0_sendString(char *string);

#endif
