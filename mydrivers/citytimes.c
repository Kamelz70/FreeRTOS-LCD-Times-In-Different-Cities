#include "citytimes.h"
#include "lcd.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "uart.h"
unsigned int selection;
/*
CITY NAMES ARRAY
*/
char cities[][10] = {"London", "Paris", "Medrid", "Rome",
					 "Athens", "Ankara", "Istanbul", "Cairo", "Moscow", "Tehran"};
/*
TIME DIFFERENCE FOR EACH CITY IN ARRAY
*/
char timediff[] = {0, 1, 1, 1, 2, 2, 2, 2, 3, 4};
/*
MESSAGE WHERE WE WILL PUT TIME IN
*/
typedef struct TIMESTRUCT
{
	unsigned char hh;
	unsigned char mm;
	unsigned char ss;

} TIMEMSG;

/*
TASK TO INIT UART
*/
void InitTask(void *pvparameter)
{
	init_UART();
	vTaskSuspend(NULL);
}

/*
TASK TO READ INPUT FROM PC
*/
unsigned int Read_From_Keyboard()
{
	unsigned int Total;
	unsigned char N;
	Total = 0;
	while (1)
	{
		N = UART0_CharGet(); //Read Number
		UART0_SendChar(N);	 //Echo the Number
		if (N == '\r')
			break;				//if Enter
		N = N - '0';			//pure number
		Total = 10 * Total + N; //Total number
	}
	return Total; //return the number
}
/*
READ TIE FROM PC
*/
void Read_Time(char buf[])
{
	unsigned char c, k = 0;
	while (1)
	{
		c = UART0_CharGet(); //Read a char
		UART0_SendChar(c);	 // echo the char
		if (c == '\r')
			break;	//If enter
		buf[k] = c; //save char
		k++;		//increment pointer
	}
	buf[k] = '\0'; //NULL terminator
}
//Time Controller
/*
INCREMENT TIME IN QUEUE EACH SECOND AND SEND IT BACK TO THE QUEUE 
*/
void Task1(void *pvparameter)
{
	TIMEMSG Tim;
	xQueueReceive(xQueue2, &Tim, portMAX_DELAY); //Get current time from Task 3
	while (1)
	{

		vTaskDelay(pdMS_TO_TICKS(1000)); //Wait one second
		Tim.ss++;
		if (Tim.ss > 59) //If 60
		{
			Tim.ss -= 60;		  //Reset to 0
			Tim.mm++;		  //Increment minutes
			if (Tim.mm > 59) //If 60
			{
				Tim.mm -= 60;		  //Reset to 0
				Tim.hh++;		  //Increment hours
				if (Tim.hh > 23) //If 24
				{
					Tim.hh -=24 ; //Reset to 0
				}
			}
		}
		xQueueSend(xQueue1, &Tim, 0); //send to task 2
	}
}

/*
INIT LC AND DISPLAY TIME AND CITY ON IT
*/
void Task2(void *pvparameter)
{
	char Txt[7];
	TIMEMSG Tim;
	LCD_init();
	LCD_Command(CLEAR_COMMAND);
	selection = 0;

	while (1)
	{

		xQueueReceive(xQueue1, &Tim, portMAX_DELAY);
		LCD_Command(CLEAR_COMMAND);
		LCD_PrintLn(0, cities[selection]);

		Tim.hh = Tim.hh + timediff[selection];
		if (Tim.hh > 23)
			Tim.hh = Tim.hh - 24;
		sprintf(Txt, "%d", Tim.hh); //GET STRING
		Ltrim(Txt);					//REMOEV SPACES
		if (Tim.hh < 10)
		{
			Txt[1] = Txt[0]; //INSERT FIRST ZERO
			Txt[0] = '0';
			Txt[2] = '\0';
		}
		LCD_PrintLn(1, Txt);
		LCD_setCursor(1, 2);
		LCD_printChar(':');
		sprintf(Txt, "%d", Tim.mm);
		Ltrim(Txt);
		if (Tim.mm < 10)
		{
			Txt[1] = Txt[0];
			Txt[0] = '0';
			Txt[2] = '\0';
		}
		LCD_printChar(Txt[0]);
		LCD_printChar(Txt[1]);
		LCD_printChar(':');
		sprintf(Txt, "%d", Tim.ss);
		Ltrim(Txt);
		if (Tim.ss < 10)
		{
			Txt[1] = Txt[0];
			Txt[0] = '0';
			Txt[2] = '\0';
		}
		LCD_printChar(Txt[0]); //display seconds
		LCD_printChar(Txt[1]);
	}
}

char *Ltrim(char *str)
{
	char *end;

	// remove starting spaces
	while (isspace((unsigned char)*str))
		str++;

	if (*str == 0) // if all are spaces
		return str;

	// Trim ending spaces
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end))
		end--;

	// insert null at end
	end[1] = '\0';

	return str;
}

/*
USE UART TO DISPLAY CITIES AND OPTIONS
*/
void Task3(void *pvparameter)
{

	char k, Buffer[10];
	TIMEMSG Tim;

	UART0_sendString("\n\rTime in Different Countries:");
	//UART0_sendString("Time in Different Countries:");
	UART0_sendString("\n\r============================");
	UART0_sendString("\n\rEnter the time in London (hh:mm:ss):");
	Read_Time(Buffer);
	Tim.hh = 10 * (Buffer[0] - '0') + Buffer[1] - '0';
	Tim.mm = 10 * (Buffer[3] - '0') + Buffer[4] - '0';
	Tim.ss = 10 * (Buffer[6] - '0') + Buffer[7] - '0';

	xQueueSend(xQueue2, &Tim, 0);
	while (1)
	{
		UART0_sendString("\n\r\n\rSelect a City");
		for (k = 0; k < 10; k++)
		{
			UART0_sendString("\n\r");
			UART0_SendChar((char)(k + '0'));
			UART0_sendString(". ");
			UART0_sendString(cities[k]);
		}

		UART0_sendString("\n\rSelection");
		selection = Read_From_Keyboard();
	}
}
