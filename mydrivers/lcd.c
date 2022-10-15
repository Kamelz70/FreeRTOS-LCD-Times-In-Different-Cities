#include "lcd.h"
#include "TM4C123GH6PM.h"
#include "stdint.h"
#include "stdlib.h"
#include <stdio.h>
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

char *itoa(int num, char *str);

char *itoa(int num, char *str)
{
	if (str == NULL)
	{
		return NULL;
	}
	sprintf(str, "%d", num);
	return str;
}
/*******************************************************************************
THIS FUNCTION INTIALIZES LCD AND ITS CORRESPONDING PORTS
*/
void LCD_init(void)
{
	LCD_CLOCK_GATING |= (1 << LCD_PORT_NUM); /*ENABLE CLOCK FOR LCD PORT*/
	LCD_PORT_DIR |= 0xFF;					 /* Configure the control pins(E,RS,RW) as output pins */
	LCD_DIGITAL_ENABLE |= 0xFF;				 /*ENABLE DIGITAL PORTS FOR LCD PORT*/
	LCD_CR |= 0xFF;
	SysCtlDelay(16000);

	CLEAR_BIT(LCD_PORT, RS); /*CLEAR RS BIT IN LCD FOR COMMAND MODE*/

	LCD_PORT = (LCD_PORT & 0X0F) | 0X30;
	SET_BIT(LCD_PORT, E);
	SysCtlDelay(10);
	CLEAR_BIT(LCD_PORT, E);
	SysCtlDelay(16000);

	LCD_PORT = (LCD_PORT & 0X0F) | 0X30;

	SET_BIT(LCD_PORT, E);
	SysCtlDelay(10);
	CLEAR_BIT(LCD_PORT, E);

	SysCtlDelay(16000);

	LCD_PORT = (LCD_PORT & 0X0F) | 0X30;

	SET_BIT(LCD_PORT, E);
	SysCtlDelay(10);
	CLEAR_BIT(LCD_PORT, E);

	SysCtlDelay(16000);

	LCD_PORT = (LCD_PORT & 0X0F) | 0X20;

	SET_BIT(LCD_PORT, E);
	SysCtlDelay(10);
	CLEAR_BIT(LCD_PORT, E);

	SysCtlDelay(16000);

	LCD_Command(0x0f);			/* TURN ON DISPLAY WITH THIS COMMAND*/
	LCD_Command(CLEAR_COMMAND); /* CLEAR DISPLAY */
}
/*******************************************************************************
 *                                               
 * FUNCTION SENDS COMMENDS TO LCD
 * */
void LCD_Command(uint8 command)
{

	LCD_PORT = (LCD_PORT & 0X0F) | (command & 0XF0);		/* out the required command to the data bus D4 --> D7 */
	CLEAR_BIT(LCD_PORT, RS);								/* Instruction Mode RS=0 */
	SET_BIT(LCD_PORT, E);									/* Enable LCD E=1 */
	SysCtlDelay(16000);										/* delay for processing Tas = 50ns */
	CLEAR_BIT(LCD_PORT, E);									/* disable LCD E=0 */
	SysCtlDelay(16000);										/* delay for processinG */
	LCD_PORT = (LCD_PORT & 0X0F) | ((command & 0X0F) << 4); /* out the REST OF required command to the data bus D4 --> D7 */
	CLEAR_BIT(LCD_PORT, RS);								/* Instruction Mode RS=0 */
	SET_BIT(LCD_PORT, E);									/* Enable LCD E=1 */
	SysCtlDelay(10);
	CLEAR_BIT(LCD_PORT, E); /* Instruction Mode RS=0 */
	SysCtlDelay(16000);		/* delay for processing*/
}

/*******************************************************************************
 *                                               
 * FUNCTION CLEARS LCD BY SENDING CLEAR COMMAND
 * */

void LCD_printChar(uint8 data)
{

	LCD_PORT = (LCD_PORT & 0X0F) | (data & 0XF0); /* out the required DATA to the data bus D4 --> D7 */
	SET_BIT(LCD_PORT, RS);						  /* DATA Mode RS=1 */
	SET_BIT(LCD_PORT, E);						  /* Enable LCD E=1 */
	SysCtlDelay(20);							  /* delay for processing */
	CLEAR_BIT(LCD_PORT, E);						  /* disable LCD E=0 */

	SysCtlDelay(16000);									 /* delay for processing Tpw  */
	LCD_PORT = (LCD_PORT & 0X0F) | ((data & 0X0F) << 4); /* out the REST OF required DATA to the data bus D4 --> D7 */
	SET_BIT(LCD_PORT, RS);								 /* DATA Mode RS=1 */
	SET_BIT(LCD_PORT, E);								 /* Enable LCD E=1 */
	SysCtlDelay(10);
	CLEAR_BIT(LCD_PORT, E); /* DATA Mode RS=1 */
	SysCtlDelay(16000);		/* delay for processing */
}

/*******************************************************************************
 *                                               
 * FUNCTION TO DIPLAY A STRING INPUT
 * */
void LCD_displayString(const char *Str)
{
	uint8 i = 0;
	while (Str[i] != '\0')
	{
		LCD_printChar(Str[i]); //SHOW CHARACTER UNTILL STRING[I] IS NULL
		i++;
	}
}
/*******************************************************************************
 *                                               
 * FUNCTION TO DIPLAY 2 STRINGS, 1 EACH LINE
 * */
void LCD_Print(char *s1, char *s2)
{
	int C;
	for (C = 0; C < 16; C++)
	{
		if (C < strlen(s1))
		{
			LCD_setCursor(0, C);
			LCD_printChar(s1[C]);
		}
		if (C < strlen(s2))
		{
			LCD_setCursor(1, C);
			LCD_printChar(s2[C]);
		}
	}
	LCD_Command(0xC0 + 16); //Hide cursor
}
/*******************************************************************************
 *                                               
 * FUNCTION TO DIPLAY A STRING INPUT IN A SPECIFIED LINE
 * */
void LCD_PrintLn(char l, char *string)
{
	LCD_setCursor(l, 0);
	for (l = 0; l < strlen(string); l++)
	{
		LCD_printChar(string[l]);
	}
	LCD_Command(0xC0 + 16); //Hide cursor
}

/*******************************************************************************
 *                                               
 * FUNCTION TO SET CURSOR AT A POSITION
 * */
void LCD_setCursor(uint8 row, uint8 col)
{

	if (row == 0)
	{
		LCD_Command(0x80 + (col % 16));
		return;
	}
	LCD_Command(0xC0 + (col % 16));

	/* to write to a specific address in the LCD 
	 * we need to apply the corresponding command 0b10000000+Address */
}

/*******************************************************************************
 *                                               
 * FUNCTION TO DIPLAY A STRING INPUT IN A ROW/COLUMN POSITION
 * */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *Str)
{
	LCD_setCursor(row, col);	/* go to to the required LCD position */
	LCD_displayString(Str); /* display the string */
}

/*******************************************************************************
 *                                               
 * FUNCTION TO CONVERT INT TO STRING
 * */
void LCD_intgerToString(int data)
{
	char buff[16];	  /* String to hold the ascii result */
	itoa(data, buff); /* 10 for decimal */
	LCD_displayString(buff);
}
/*******************************************************************************
 *                                               
 * FUNCTION TO CLEAR SCREEN
 * */
void LCD_clearScreen(void)
{
	LCD_Command(CLEAR_COMMAND); //clear display
}
