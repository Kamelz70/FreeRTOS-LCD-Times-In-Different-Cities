
#include "stdint.h"

#include "stdbool.h"
#include "lcd.h"
#include "uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "TM4C123GH6PM.h"

#include "citytimes.h"
//////////////code for testing uart
/*
int main(void)
{ 

int i=0;
	LCD_init(); 
    
	// LCD_Command(CURSOR_ON);
	// LCD_Command(CLEAR_COMMAND); 
InitLEDs();
	while(1)
{i++;
        SysCtlDelay(5000000);
    GPIO_PORTF_DATA_R^=0x02;
	
	 if(i%2==0&&i>0)
	 {	
	LCD_Clear();
	 LCD_Print("MATHAR","");
	 	i=0;
	 }
	 else{
	LCD_Clear();
LCD_Print("","FAKKAR");
	 }
}
*/


QueueHandle_t xQueue1;
QueueHandle_t xQueue2;

int main()
	{
  xQueue1 = xQueueCreate(1,sizeof(long));
	xQueue2 = xQueueCreate(1,sizeof(long));
	
	xTaskCreate(Task1,"Time Controller", 80,NULL,10,NULL);
	xTaskCreate(Task2, "LCD Controller",80, NULL, 10, NULL);
	xTaskCreate(Task3, "UART Controller", 80, NULL,10,NULL);
	xTaskCreate(InitTask, "init", 80, NULL, 11, NULL);
	
	
	vTaskStartScheduler();
	
	
	return 0;
	
}


