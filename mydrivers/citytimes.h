
#include <stdint.h>
#include <stdbool.h> 
#include <stdlib.h>

#include <FreeRTOS.h>
#include "FreeRTOSConfig.h"
#include "tm4c123gh6pm.h"
#include <task.h>
#include "queue.h"
#include "semphr.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"


#include "driverlib/pin_map.h"
#include "tm4c123gh6pm.h"


extern QueueHandle_t xQueue1;
extern QueueHandle_t xQueue2;

void InitTask(void *pvparameter);
void Task1(void *pvparameter);
void Task2(void *pvparameter);
void Task3(void *pvparameter);
void printStringUART(char* string);
unsigned int Read_From_Keyboard(void);
void Read_Time(char buf[]);
void init_UART(void);
char *Ltrim(char *str);
