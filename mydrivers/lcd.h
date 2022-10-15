#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
/* LCD Data bits mode configuration */
#define DATA_BITS_MODE 4
#define UPPER_PORT_PINS

/* LCD HW Pins */
#define RS 0
#define E 1
/**/
#define LCD_CLOCK_GATING SYSCTL_RCGCGPIO_R
/*LCD PORTS*/

#define LCD_PORT GPIO_PORTB_DATA_R
#define LCD_PORT_DIR GPIO_PORTB_DIR_R
#define LCD_DIGITAL_ENABLE GPIO_PORTB_DEN_R
#define LCD_CR GPIO_PORTB_CR_R
#define LCD_PORT_NUM 1

/* LCD Commands */
#define CLEAR_COMMAND 0x01
#define FOUR_BITS_DATA_MODE 0x02
#define TWO_LINE_LCD_Four_BIT_MODE 0x28
#define TWO_LINE_LCD_Eight_BIT_MODE 0x38
#define CURSOR_OFF 0x0C
#define CURSOR_ON 0x0E
#define SET_CURSOR_LOCATION 0x80

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void LCD_init(void);
void LCD_Command(uint8 command);
void LCD_printChar(uint8 data);
void LCD_displayString(const char *Str);
void LCD_clearScreen(void);
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char *Str);
void LCD_setCursor(uint8 row, uint8 col);
void LCD_intgerToString(int data);
void LCD_Print(char *s1, char *s2); //Print 2 lines
void LCD_PrintLn(char l, char *string);

#endif /* LCD_H_ */
