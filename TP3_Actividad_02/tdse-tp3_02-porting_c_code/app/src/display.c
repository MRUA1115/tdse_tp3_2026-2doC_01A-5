//=====[Libraries]=============================================================
//#include "mbed.h"
//#include "arm_book_lib.h"

#include "display.h"
#include "main.h"
#include <stdbool.h>

/* ============================================================
 * PCF8574 - LCD I2C
 * ============================================================ */

#define DISPLAY_I2C_ADDRESS    (0x27 << 1)

/*
 * Mapeo típico del backpack:
 *
 * PCF8574 P0 -> LCD RS
 * PCF8574 P1 -> LCD RW
 * PCF8574 P2 -> LCD EN
 * PCF8574 P3 -> Backlight
 * PCF8574 P4 -> LCD D4
 * PCF8574 P5 -> LCD D5
 * PCF8574 P6 -> LCD D6
 * PCF8574 P7 -> LCD D7
 */

#define DISPLAY_I2C_RS          0x01
#define DISPLAY_I2C_RW          0x02
#define DISPLAY_I2C_EN          0x04
#define DISPLAY_I2C_BACKLIGHT   0x08
#define DISPLAY_I2C_D4          0x10
#define DISPLAY_I2C_D5          0x20
#define DISPLAY_I2C_D6          0x40
#define DISPLAY_I2C_D7          0x80

/* Demo includes */
#include "logger.h"
#include "dwt.h"
#include "systick.h"

/********************** arm_book Defines *******************************/
//#include "arm_book_lib.h"
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

#define DISPLAY_16x2_LINE1_FIRST_CHARACTER_ADDRESS  0
#define DISPLAY_16x2_LINE2_FIRST_CHARACTER_ADDRESS  64

#define DISPLAY_RS_INSTRUCTION 0
#define DISPLAY_RS_DATA        1

#define DISPLAY_RW_WRITE 0
#define DISPLAY_RW_READ  1

#define DISPLAY_PIN_RS  4
#define DISPLAY_PIN_RW  5
#define DISPLAY_PIN_EN  6
#define DISPLAY_PIN_D0  7
#define DISPLAY_PIN_D1  8
#define DISPLAY_PIN_D2  9
#define DISPLAY_PIN_D3 10
#define DISPLAY_PIN_D4 11
#define DISPLAY_PIN_D5 12
#define DISPLAY_PIN_D6 13
#define DISPLAY_PIN_D7 14

#define DISPLAY_DEL_37US	37ul
#define DISPLAY_DEL_01US	01ul

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

/*
DigitalOut displayD0( D0 );
DigitalOut displayD1( D1 );
DigitalOut displayD2( D2 );
DigitalOut displayD3( D3 );
*/

// uso del .ioc

//=====[Declaration of external public global variables]=======================
extern I2C_HandleTypeDef hi2c1;

//=====[Declaration and initialization of private global variables]============
static display_t display;
static bool initial8BitCommunicationIsCompleted;
static uint8_t displayI2CState = DISPLAY_I2C_BACKLIGHT;

//=====[Declarations (prototypes) of private functions]========================
static void displayPinWrite( uint8_t pinName, int value );
static void displayDataBusWrite( uint8_t dataByte );
static void displayCodeWrite( bool type, uint8_t dataBus );

static void displayI2CWrite(uint8_t data);

//=====[Implementations of public functions]===================================

static void displayI2CWrite(uint8_t data)
{
    HAL_I2C_Master_Transmit(
        &hi2c1,
        DISPLAY_I2C_ADDRESS,
        &data,
        1,
        HAL_MAX_DELAY
    );
}

void displayInit( displayConnection_t connection )
{
    display.connection = connection;

    initial8BitCommunicationIsCompleted = false;

    HAL_Delay(50);

    displayCodeWrite( DISPLAY_RS_INSTRUCTION,
                      DISPLAY_IR_FUNCTION_SET |
                      DISPLAY_IR_FUNCTION_SET_8BITS );
    HAL_Delay(5);

    displayCodeWrite( DISPLAY_RS_INSTRUCTION,
                      DISPLAY_IR_FUNCTION_SET |
                      DISPLAY_IR_FUNCTION_SET_8BITS );
    HAL_Delay( 1 );

    displayCodeWrite( DISPLAY_RS_INSTRUCTION,
                      DISPLAY_IR_FUNCTION_SET |
                      DISPLAY_IR_FUNCTION_SET_8BITS );
    HAL_Delay(1);

    switch( display.connection ) {
        case DISPLAY_CONNECTION_GPIO_8BITS:
            displayCodeWrite( DISPLAY_RS_INSTRUCTION,
                              DISPLAY_IR_FUNCTION_SET |
                              DISPLAY_IR_FUNCTION_SET_8BITS |
                              DISPLAY_IR_FUNCTION_SET_2LINES |
                              DISPLAY_IR_FUNCTION_SET_5x8DOTS );
            HAL_Delay( 1 );
        break;

        case DISPLAY_CONNECTION_GPIO_4BITS:
            displayCodeWrite( DISPLAY_RS_INSTRUCTION,
                              DISPLAY_IR_FUNCTION_SET |
                              DISPLAY_IR_FUNCTION_SET_4BITS );
            HAL_Delay( 1 );

            initial8BitCommunicationIsCompleted = true;

            displayCodeWrite( DISPLAY_RS_INSTRUCTION,
                              DISPLAY_IR_FUNCTION_SET |
                              DISPLAY_IR_FUNCTION_SET_4BITS |
                              DISPLAY_IR_FUNCTION_SET_2LINES |
                              DISPLAY_IR_FUNCTION_SET_5x8DOTS );
            HAL_Delay( 1 );
        break;
    }

    displayCodeWrite( DISPLAY_RS_INSTRUCTION,
                      DISPLAY_IR_DISPLAY_CONTROL |
                      DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_OFF |
                      DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF |
                      DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF );
    HAL_Delay( 1 );

    displayCodeWrite( DISPLAY_RS_INSTRUCTION,
                      DISPLAY_IR_CLEAR_DISPLAY );
    HAL_Delay( 1 );

    displayCodeWrite( DISPLAY_RS_INSTRUCTION,
                      DISPLAY_IR_ENTRY_MODE_SET |
                      DISPLAY_IR_ENTRY_MODE_SET_INCREMENT |
                      DISPLAY_IR_ENTRY_MODE_SET_NO_SHIFT );
    HAL_Delay( 1 );

    displayCodeWrite( DISPLAY_RS_INSTRUCTION,
                      DISPLAY_IR_DISPLAY_CONTROL |
                      DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_ON |
                      DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF |
                      DISPLAY_IR_DISPLAY_CONTROL_BLINK_OFF );
    HAL_Delay( 1 );
}

void displayCharPositionWrite(uint8_t x, uint8_t y)
{
    switch (y)
    {
        case 0:

            displayCodeWrite(
                DISPLAY_RS_INSTRUCTION,
                DISPLAY_IR_SET_DDRAM_ADDR |
                DISPLAY_16x2_LINE1_FIRST_CHARACTER_ADDRESS |
                x
            );

            break;


        case 1:

            displayCodeWrite(
                DISPLAY_RS_INSTRUCTION,
                DISPLAY_IR_SET_DDRAM_ADDR |
                DISPLAY_16x2_LINE2_FIRST_CHARACTER_ADDRESS |
                x
            );

            break;


        default:
            break;
    }

    systick_delay_us(DISPLAY_DEL_37US);
}

void displayStringWrite( const char * str )
{
    while (*str) {
        displayCodeWrite(DISPLAY_RS_DATA, *str++);
    }
}

void displayDataWrite( char data )
{
    displayCodeWrite( DISPLAY_RS_DATA, (uint8_t)data );
}

//=====[Implementations of private functions]==================================
static void displayCodeWrite( bool type, uint8_t dataBus )
{
    if ( type == DISPLAY_RS_INSTRUCTION )
        displayPinWrite( DISPLAY_PIN_RS, DISPLAY_RS_INSTRUCTION );
	else
        displayPinWrite( DISPLAY_PIN_RS, DISPLAY_RS_DATA );
    displayPinWrite( DISPLAY_PIN_RW, DISPLAY_RW_WRITE );
    displayDataBusWrite( dataBus );
}

static void displayPinWrite(uint8_t pinName, int value)
{
    switch (pinName)
    {
        case DISPLAY_PIN_RS:

            if (value)
            {
                displayI2CState |= DISPLAY_I2C_RS;
            }
            else
            {
                displayI2CState &= (uint8_t)~DISPLAY_I2C_RS;
            }

            break;


        case DISPLAY_PIN_RW:

            if (value)
            {
                displayI2CState |= DISPLAY_I2C_RW;
            }
            else
            {
                displayI2CState &= (uint8_t)~DISPLAY_I2C_RW;
            }

            break;


        case DISPLAY_PIN_EN:

            if (value)
            {
                displayI2CState |= DISPLAY_I2C_EN;
            }
            else
            {
                displayI2CState &= (uint8_t)~DISPLAY_I2C_EN;
            }

            break;


        case DISPLAY_PIN_D4:

            if (value)
            {
                displayI2CState |= DISPLAY_I2C_D4;
            }
            else
            {
                displayI2CState &= (uint8_t)~DISPLAY_I2C_D4;
            }

            break;


        case DISPLAY_PIN_D5:

            if (value)
            {
                displayI2CState |= DISPLAY_I2C_D5;
            }
            else
            {
                displayI2CState &= (uint8_t)~DISPLAY_I2C_D5;
            }

            break;


        case DISPLAY_PIN_D6:

            if (value)
            {
                displayI2CState |= DISPLAY_I2C_D6;
            }
            else
            {
                displayI2CState &= (uint8_t)~DISPLAY_I2C_D6;
            }

            break;


        case DISPLAY_PIN_D7:

            if (value)
            {
                displayI2CState |= DISPLAY_I2C_D7;
            }
            else
            {
                displayI2CState &= (uint8_t)~DISPLAY_I2C_D7;
            }

            break;


        default:
            break;
    }
}

static void displayDataBusWrite(uint8_t dataBus)
{
    uint8_t nibble;


    /* --------------------------------------------------------
     * EN = 0
     * -------------------------------------------------------- */

    displayI2CState &= (uint8_t)~DISPLAY_I2C_EN;

    displayI2CWrite(displayI2CState);


    /* --------------------------------------------------------
     * Nibble alto
     *
     * dataBus:
     *
     * D7 D6 D5 D4
     *
     * ya están en los bits 7..4.
     * -------------------------------------------------------- */

    nibble = dataBus & 0xF0;

    displayI2CState &= 0x0F;
    displayI2CState |= nibble;


    /* --------------------------------------------------------
     * Pulso EN
     * -------------------------------------------------------- */

    displayI2CState |= DISPLAY_I2C_EN;

    displayI2CWrite(displayI2CState);

    systick_delay_us(DISPLAY_DEL_37US);

    displayI2CState &= (uint8_t)~DISPLAY_I2C_EN;

    displayI2CWrite(displayI2CState);


    /* --------------------------------------------------------
     * Durante la inicialización de 4 bits solamente
     * enviamos el nibble alto.
     *
     * Esto es MUY importante.
     * -------------------------------------------------------- */

    if (initial8BitCommunicationIsCompleted == false)
    {
        return;
    }


    /* --------------------------------------------------------
     * Nibble bajo
     *
     * Los bits 3..0 de dataBus se desplazan a 7..4.
     * -------------------------------------------------------- */

    nibble = (uint8_t)((dataBus << 4) & 0xF0);

    displayI2CState &= 0x0F;
    displayI2CState |= nibble;


    /* --------------------------------------------------------
     * Pulso EN
     * -------------------------------------------------------- */

    displayI2CState |= DISPLAY_I2C_EN;

    displayI2CWrite(displayI2CState);

    systick_delay_us(DISPLAY_DEL_37US);

    displayI2CState &= (uint8_t)~DISPLAY_I2C_EN;

    displayI2CWrite(displayI2CState);

    systick_delay_us(DISPLAY_DEL_37US);
}
