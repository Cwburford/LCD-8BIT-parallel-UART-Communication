/****************************************************/
/*													*/
/* Project name:	LCD_Test_8bit_parallel			*/
/* Displays a two line test message:				*/
/*													*/
/*		Test message: LCD is OK						*/
/*		in 8bit parallel mode						*/
/*   												*/
/* on the 2x24 dot matrix LCD at a specific 		*/
/* position using the ATMEL ATmega2560				*/
/*													*/
/* PORTB for LCD data:							 	*/
/* 													*/
/* B.0  LCD D0			U301 PIN 22					*/
/* B.1  LCD D1 			U301 PIN 23					*/
/* All the way to B.7	etc.						*/
/* B.7  LCD D7			U301 PIN 29					*/
/*													*/
/* PORTA for LCD control:							*/
/*													*/
/*	A.7	LCD RS			U301 PIN 16					*/
/*	A.6	LCD E			U301 PIN 17					*/
/*	A.5	LCD R/!W		U301 PIN 18					*/
/*													*/
/* 	The code is written as a single .c module		*/
/*													*/
/*	Author:		Cameron Burford						*/
/*	Date:		3/30/15								*/
/*	Ver:		1.0									*/
/*	Filename:	LCD_8bit_2_line_Test_Message.c		*/
/*													*/
/****************************************************/

// #defines

#define F_CPU 8000000UL             // F_CPU must be defined BEFORE the #include <util/delay.h>

/*includes*/
#include <avr/io.h>
#include <util/delay.h>

#define INSTR_WR	0
#define DATA_WR		1

// declare function prototypes

void init_ports (void);
void init_LCD (void);
void LCD_write (unsigned char RS, unsigned char data);
void LCD_write_str_data(char *data_ptr);

int main(void)
{
	char T_mess[] = {"Test message: "};
	char B_mess1[] = {"LCD is OK!           "};
	char B_mess2[] = {"In 8bit parallel mode"};
	
	

	init_ports();                           // initialize PORTs BEFORE initializing LCD
    init_LCD();                             // initialize LCD

	LCD_write(INSTR_WR, 0x01);
	_delay_ms(2);

	LCD_write(INSTR_WR, 0x02);
	_delay_ms(2);


	LCD_write_str_data (T_mess);


	LCD_write(INSTR_WR, 0xC0);
	_delay_us(50);


	while(1) // start of super loop
	{
		
		LCD_write(INSTR_WR, 0xC0);              // Set DDRAM address to bottom row, left character
		_delay_us(50);
		LCD_write_str_data (B_mess1);
		_delay_ms(1000);
		LCD_write(INSTR_WR, 0xC0);              // Set DDRAM address to bottom row, left character
		_delay_us(50);
		LCD_write_str_data (B_mess2);
		_delay_ms(1000);
		
		}  //end while(1) loop

		return(0);

		}	// end main

		void init_ports (void)
		{
			DDRB = 0xFF;                        // PORTB all outputs
			PORTB = 0x00;                       // Set all outputs to 0 initially
			DDRA = 0xE0;                        // Set A.7 through A.5 to outputs for LCD control
		}

		// Perform LCD initialization
		void init_LCD (void)
		{
			_delay_ms(35);                      // wait for more than 30mS after VDD rises to 4.5V
			LCD_write(INSTR_WR,0x38);           // function set 8bits, 2line, display off
			_delay_us(50);                      // wait for more than 39microS
			LCD_write(INSTR_WR,0x0C);           // display on, cursor off, blink off
			_delay_us(50);                      // wait for more than 39microS
			LCD_write(INSTR_WR,0x01);           // display clear
			_delay_ms(2);                       // wait for more than 1.53mS
			LCD_write(INSTR_WR,0x06);           // entry mode set, increment mode
		}
		// End of LCD initialization

		void LCD_write (unsigned char RS, unsigned char data)
		{
			if(RS==DATA_WR) PORTA = 0b10000000;	// write data: RS = 1 E = 0, R/!W=0 (write)
			else			PORTA = 0b00000000;	// Write instruction: RS = 0 E = 0, R/!W=0 (write)


			PORTA = PORTA | 0x40;               // Take E HIGH (logic 1)
			PORTB = data;
			_delay_us(50);                      // needs to be at least 30uS or no display - use 50
			PORTA = PORTA & 0x80;               // Take E LOW (logic 0)
			_delay_us(50);                      // Delay REQUIRED
		}

		void LCD_write_str_data(char *str_ptr)
		{
			PORTA = 0b10000000;                 // write data: RS = 1 E = 0, R/!W=0 (write)

			while(*str_ptr != '\0')
			{
				PORTA = PORTA | 0x40;           // Take E HIGH (logic 1)
				PORTB = *str_ptr++;
				_delay_us(50);                  //needs to be at least 30uS or no display - use 50
				PORTA = PORTA & 0x80;           // Take E LOW (logic 0)
				_delay_us(50);                  // Delay REQUIRED
			}
		}

