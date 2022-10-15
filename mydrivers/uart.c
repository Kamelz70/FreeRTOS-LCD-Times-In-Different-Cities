#include "stdint.h"
#include "stdbool.h"
#include "tm4c123gh6pm.h"
#include "uart.h"
///////////////////////////////////////////////////////////////////////////
//Defines

/*******************************************************************************
 *                                               
 * FUNCTION TO INIT LEDS (TESTING PURPOSES)
 * */

void InitLEDs(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20U; //enable portF clock
    GPIO_PORTF_DIR_R |= 0X0EU;  //Enable LED outputs
    GPIO_PORTF_DEN_R |= 0X0EU;  //Enable LED Pins
}

/////////////////////////////////////

//funcition defs
/*******************************************************************************
 *                                               
 * FUNCTION TO INIT UART0
 * */
void init_UART(void)
{
    SYSCTL_RCGCUART_R |= 0x01;                                         //enable clock for uart0
    SYSCTL_RCGCGPIO_R |= 0x01;                                         //enable clock for port A
    GPIO_PORTA_DEN_R |= 0x03;                                          //digital enable PortA0-1
    GPIO_PORTA_AFSEL_R |= 0x03;                                        //enable alternate func. for portA0-1
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) + 0x00000011; //enable UART0 PA0 as a reciever, PA1 as a transmitter
    GPIO_PORTA_AMSEL_R &= ~(0x03);                                     //disable analog for portA0-1

    UART0_CTL_R &= ~(0x01); //disable UART0

    //Configure baud rate (MORE INFO IN UART.H)
    UART0_IBRD_R = IntBaud;
    UART0_FBRD_R = FractionBaud;

    UART0_LCRH_R = 0x70;  //8-bit word length,enable FIFO
    UART0_CC_R = 0x00;    //use default clock source
    UART0_CTL_R = 0x0301; //enable RX,TX,UART
}
/*******************************************************************************
 *                                               
 * FUNCTION TO RECIECVE CHAR
 * */
char UART0_CharGet(void)
{
    char c;
    while ((UART0_FR_R & 0X10) != 0)
        ;
    //Wait until recieve fifo is not empty
    c = (char)(UART0_DR_R & 0xFF);
    return c;
}
/*******************************************************************************
 *                                               
 * FUNCTION TO SEND CHAR
 * */
void UART0_SendChar(char c)
{
    while ((UART0_FR_R & 0X20) != 0)
        ;
    //Wait until send fifo is not full
    UART0_DR_R = c;
}
/*******************************************************************************
 *                                               
 * FUNCTION TO SEND STRING
 * */
void UART0_sendString(char *string)
{
    while (*string)
    {
        UART0_SendChar(*(string++));
    }
}
