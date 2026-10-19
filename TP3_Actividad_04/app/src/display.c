/*
 * Copyright (c) 2026 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 */

//=====[Libraries]=============================================================
#include "display.h"
#include "main.h"
#include <stdbool.h>

/* Demo includes */
#include "logger.h"
#include "dwt.h"
#include "systick.h"

/********************** arm_book Defines *******************************/
// Functional states
#ifndef OFF
#define OFF    0
#endif
#ifndef ON
#define ON     ( !OFF )
#endif

// Electrical states
#ifndef LOW
#define LOW    0
#endif
#ifndef HIGH
#define HIGH   ( !LOW )
#endif

//=====[Declaration of private defines]========================================
#define DISPLAY_IR_CLEAR_DISPLAY   0b00000001
#define DISPLAY_IR_ENTRY_MODE_SET  0b00000100
#define DISPLAY_IR_DISPLAY_CONTROL 0b00001000
#define DISPLAY_IR_FUNCTION_SET    0b00100000
#define DISPLAY_IR_SET_DDRAM_ADDR  0b10000000

#define DISPLAY_IR_ENTRY_MODE_SET_INCREMENT 0b00000010
#define DISPLAY_IR_ENTRY_MODE_SET_DECREMENT 0b00000000
#define DISPLAY_IR_ENTRY_MODE_SET_SHIFT     0b00000001
#define DISPLAY_IR_ENTRY_MODE_SET_NO_SHIFT  0b00000000

#define DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_ON  0b00000100
#define DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_OFF 0b00000000
#define DISPLAY_IR_DISPLAY_CONTROL_CURSOR_ON   0b00000010
#define DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF  0b00000000
#define DISPLAY_IR_DISPLAY_CONTROL_BLINK_ON    0b00000001
#define DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF   0b00000000

#define DISPLAY_IR_FUNCTION_SET_8BITS    0b00010000
#define DISPLAY_IR_FUNCTION_SET_4BITS    0b00000000
#define DISPLAY_IR_FUNCTION_SET_2LINES   0b00001000
#define DISPLAY_IR_FUNCTION_SET_1LINE    0b00000000
#define DISPLAY_IR_FUNCTION_SET_5x10DOTS 0b00000100
#define DISPLAY_IR_FUNCTION_SET_5x8DOTS  0b00000000

#define DISPLAY_20x4_LINE1_FIRST_CHARACTER_ADDRESS 0
#define DISPLAY_20x4_LINE2_FIRST_CHARACTER_ADDRESS 64
#define DISPLAY_20x4_LINE3_FIRST_CHARACTER_ADDRESS 20
#define DISPLAY_20x4_LINE4_FIRST_CHARACTER_ADDRESS 84

#define DISPLAY_RS_INSTRUCTION 0
#define DISPLAY_RS_DATA        1

#define DISPLAY_RW_WRITE 0
#define DISPLAY_RW_READ  1

#define DISPLAY_DEL_37US	37ul
#define DISPLAY_DEL_01US	01ul

/* ---------- Mapeo del backpack PCF8574 (módulo I2C estándar tipo
   "LiquidCrystal_I2C") ------------------------------------------------- */
#define LCD_I2C_ADDR_7BITS   0x27            /* Dirección típica: 0x27 o 0x3F */
#define LCD_I2C_ADDR         (LCD_I2C_ADDR_7BITS << 1)

#define LCD_PCF_RS           0x01  /* P0 */
#define LCD_PCF_RW           0x02  /* P1 */
#define LCD_PCF_EN           0x04  /* P2 */
#define LCD_PCF_BACKLIGHT    0x08  /* P3 */
/* P4-P7 = D4-D7 */

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

/* Handle I2C definido en main.c (MX_I2C1_Init) */
extern I2C_HandleTypeDef hi2c1;

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of private global variables]============
static display_t display;
static uint8_t backlightState = LCD_PCF_BACKLIGHT;

//=====[Declarations (prototypes) of private functions]========================
static void lcd_i2c_write_byte(uint8_t data);
static void lcd_pulse_enable(uint8_t data);
static void lcd_write_nibble(uint8_t nibble, bool isData);
static void displayCodeWrite(bool type, uint8_t dataBus);

//=====[Implementations of public functions]===================================
void displayInit(displayConnection_t connection)
{
    display.connection = connection;

    HAL_Delay(50);

    /* Secuencia de "despertar" del HD44780 en modo 4 bits (ver datasheet).
       Se hace con nibbles crudos, sin pasar por displayCodeWrite. */
    lcd_write_nibble(0x30, false);
    HAL_Delay(5);
    lcd_write_nibble(0x30, false);
    HAL_Delay(1);
    lcd_write_nibble(0x30, false);
    HAL_Delay(1);
    lcd_write_nibble(0x20, false); /* conmuta definitivamente a modo 4 bits */
    HAL_Delay(1);

    displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                      DISPLAY_IR_FUNCTION_SET |
                      DISPLAY_IR_FUNCTION_SET_4BITS |
                      DISPLAY_IR_FUNCTION_SET_2LINES |
                      DISPLAY_IR_FUNCTION_SET_5x8DOTS);
    HAL_Delay(1);

    displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                      DISPLAY_IR_DISPLAY_CONTROL |
                      DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_OFF |
                      DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF |
                      DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF);
    HAL_Delay(1);

    displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                      DISPLAY_IR_CLEAR_DISPLAY);
    HAL_Delay(2); /* el clear display necesita más tiempo (~1.6ms típico) */

    displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                      DISPLAY_IR_ENTRY_MODE_SET |
                      DISPLAY_IR_ENTRY_MODE_SET_INCREMENT |
                      DISPLAY_IR_ENTRY_MODE_SET_NO_SHIFT);
    HAL_Delay(1);

    displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                      DISPLAY_IR_DISPLAY_CONTROL |
                      DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_ON |
                      DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF |
                      DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF);
    HAL_Delay(1);
}

void displayCharPositionWrite(uint8_t charPositionX, uint8_t charPositionY)
{
    switch (charPositionY)
    {
        case 0:
            displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                              DISPLAY_IR_SET_DDRAM_ADDR |
                              (DISPLAY_20x4_LINE1_FIRST_CHARACTER_ADDRESS +
                               charPositionX));
            systick_delay_us(DISPLAY_DEL_01US);
        break;

        case 1:
            displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                              DISPLAY_IR_SET_DDRAM_ADDR |
                              (DISPLAY_20x4_LINE2_FIRST_CHARACTER_ADDRESS +
                               charPositionX));
            systick_delay_us(DISPLAY_DEL_01US);
        break;

        case 2:
            displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                              DISPLAY_IR_SET_DDRAM_ADDR |
                              (DISPLAY_20x4_LINE3_FIRST_CHARACTER_ADDRESS +
                               charPositionX));
            systick_delay_us(DISPLAY_DEL_01US);
        break;

        case 3:
            displayCodeWrite(DISPLAY_RS_INSTRUCTION,
                              DISPLAY_IR_SET_DDRAM_ADDR |
                              (DISPLAY_20x4_LINE4_FIRST_CHARACTER_ADDRESS +
                               charPositionX));
            systick_delay_us(DISPLAY_DEL_01US);
        break;
    }
}

void displayStringWrite(const char *str)
{
    while (*str)
    {
        displayCodeWrite(DISPLAY_RS_DATA, *str++);
    }
}

void displayDataWrite(const char data)
{
    displayCodeWrite(DISPLAY_RS_DATA, data);
}

//=====[Implementations of private functions]==================================

/* Manda un byte crudo al PCF8574 por I2C */
static void lcd_i2c_write_byte(uint8_t data)
{
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR, &data, 1, HAL_MAX_DELAY);
}

/* Genera el pulso de Enable necesario para que el HD44780 "lea" el nibble
   ya presente en las líneas D4-D7 del expansor */
static void lcd_pulse_enable(uint8_t data)
{
    lcd_i2c_write_byte(data | LCD_PCF_EN);
    systick_delay_us(1);                 /* ancho mínimo del pulso E (>450ns) */
    lcd_i2c_write_byte(data & ~LCD_PCF_EN);
    systick_delay_us(50);                /* tiempo de settle tras bajar E */
}

/* Escribe un nibble (4 bits altos de 'nibble') al LCD, agregando
   RS/backlight según corresponda. RW se deja siempre en 0 (escritura). */
static void lcd_write_nibble(uint8_t nibble, bool isData)
{
    uint8_t data = (nibble & 0xF0) | backlightState;

    if (isData)
        data |= LCD_PCF_RS;

    lcd_i2c_write_byte(data);
    lcd_pulse_enable(data);
}

/* Reemplaza a la antigua displayCodeWrite(GPIO): arma los dos nibbles
   (alto y bajo) de dataBus y los manda por I2C */
static void displayCodeWrite(bool type, uint8_t dataBus)
{
    bool isData = (type == DISPLAY_RS_DATA);

    lcd_write_nibble(dataBus & 0xF0, isData);         /* nibble alto */
    lcd_write_nibble((uint8_t)(dataBus << 4) & 0xF0, isData); /* nibble bajo */

    systick_delay_us(DISPLAY_DEL_37US);
}

/********************** end of file ******************************************/
