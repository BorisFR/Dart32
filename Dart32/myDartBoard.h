#pragma once
#ifndef MYDARTBOARD_H
#define MYDARTBOARD_H

#define ROWS 7
#define COLUMNS 10
#define DEBOUNCE_DELAY 500
/*
				 3V3		GND
				  EN		23 SPI MISO
				I 36		22 SPI SCK/Clock           I: Input without internal pull-up or pull-down
				I 39		 1 Output (TX)
				I 34		 3 Input (RX)
				I 35		21  2
		1		  32		GND
		1		  33		19  2
		1		  25		18  2
		1		  26		 5 SPI CS/SS/SDA
SPI MOSI  27		17  2
		1		  14		16  2
		1		  12		 4  2          (12: pb boot si pull HIGH)
				 GND		 0
		2		  13		 2 LED
				*  9		15  2
				* 10		 8 * SPI Flash
				  xx		 7 *
				  5V		 6 *
*/
#include <PCF8574.h>
#include <Wire.h>

#include "configuration.h"

/* in platformio.ini
#define I2C_SDA 33
#define I2C_SCL 32*/
PCF8574 pcfOUT(0x20);
PCF8574 pcfIN0(0x21);
PCF8574 pcfIN1(0x22);
/*
PCF8574 and PCF8574A I2C-Bus Slave Address Map
INPUTS PCF8574 I2C-Bus PCF8574A I2C-Bus
A2 A1 A0 Slave Address Slave Address
L L L 20 (hexadecimal) 38 (hexadecimal)
L L H 21 (hexadecimal) 39 (hexadecimal)
L H L 22 (hexadecimal) 3A (hexadecimal)
L H H 23 (hexadecimal) 3B (hexadecimal)
H L L 24 (hexadecimal) 3C (hexadecimal)
H L H 25 (hexadecimal) 3D (hexadecimal)
H H L 26 (hexadecimal) 3E (hexadecimal)
H H H 27 (hexadecimal) 3F (hexadecimal)
*/
#define MATRIX ROWS *COLUMNS
unsigned long press[ROWS * COLUMNS + 3];
bool oneTouch = false;
bool displayTooLong = false;
char keymap[ROWS][COLUMNS] = {{'0', '1', '2', '4', '5', '6', '7', '8', '9', '*'},
                              {'a', 'c', 'd', 'e', 'f', 'g', 'h', 'i', '?', '!'},
                              {'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', '&'},
                              {'t', 'u', 'v', 'w', 'x', 'y', 'z', 'A', '@', '#'},
                              {'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', '}', '('},
                              {'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', ')', '='},
                              {'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '+', '-', ']'}};

// for sending info to web browser
void (*_callWS)(String from);

#define PIN_BUTTON_TOP 6
#define PIN_BUTTON_MIDDLE 7
#define PIN_BUTTON_BOTTOM 5
#define BUTTON_TOP 0
#define BUTTON_MIDDLE 1
#define BUTTON_BOTTOM 2
void (*_pressTop)();
void (*_pressMiddle)();
void (*_pressBottom)();

// ************************************************************************************

void setupDart(void (*callWS)(String), void (*pressTop)(), void (*pressMiddle)(), void (*pressBottom)())
{
    _callWS = callWS;
    _pressTop = pressTop;
    _pressMiddle = pressMiddle;
    _pressBottom = pressBottom;
    pcfOUT.begin(I2C_SDA, I2C_SCL);
    pcfOUT.write8(B11111111);
    pcfIN0.begin(I2C_SDA, I2C_SCL);
    pcfIN1.begin(I2C_SDA, I2C_SCL);
    pcfIN0.write8(B11111111);
    pcfIN1.write8(B11100011);
}

// ************************************************************************************

void testButton(uint8_t button, uint8_t bit)
{
    uint8_t index = MATRIX + button;
    if (bit == 0)
    {
        if (press[index] == 0)
        {
            press[index] = millis();
            switch ((button))
            {
            case BUTTON_TOP:
                _pressTop();
                break;
            case BUTTON_MIDDLE:
                _pressMiddle();
                break;
            case BUTTON_BOTTOM:
                _pressBottom();
                break;
            }
        }
    }
    else
    {
        if (press[index] > 0)
        {
            if (millis() - press[index] > DEBOUNCE_DELAY)
            {
                press[index] = 0;
            }
        }
    }
}

// ************************************************************************************

void testPin(uint8_t o, uint8_t i, uint8_t bit)
{
    uint8_t index = o * COLUMNS + i;
    if (bit == 0)
    {
        if (press[index] == 0)
        {
            press[index] = millis();
            MY_DEBUG(String(o) + "/" + String(i) + "=>" + keymap[o][i] + " ");
            _callWS("O" + String(keymap[o][i]));
            //ws.textAll("O" + String(keymap[o][i]));
            oneTouch = true;
        }
        else
        {
            // debouce
            if (!displayTooLong)
            {
                if (millis() - press[index] > DEBOUNCE_DELAY)
                {
                    MY_DEBUG_LN("*** TOO LONG ***");
                    displayTooLong = true;
                    _callWS("E" + String(keymap[o][i]));
                    //ws.textAll("E" + String(keymap[o][i]));
                }
            }
        }
    }
    else // bitRead
    {
        if (press[index] > 0)
        {
            if (millis() - press[index] > DEBOUNCE_DELAY)
            {
                press[index] = 0;
                oneTouch = false;
                MY_DEBUG_LN(" OFF");
                displayTooLong = false;
                _callWS("X" + String(keymap[o][i]));
                //ws.textAll("X" + String(keymap[o][i]));
            }
        }
    } // else bitRead
}

// ************************************************************************************

void readDart()
{
    for (byte o = 0; o < ROWS; o++)
    {
        pcfOUT.write(o, LOW);
        uint16_t value = pcfIN1.read8();
        value = value << 8;
        value += pcfIN0.read8();
        pcfOUT.write(o, HIGH);
        for (byte i = 0; i < COLUMNS; i++)
        {
            uint8_t b = bitRead(value, i);
            testPin(o, i, b);
        }
    } // for ROWS

    // 3 buttons
    pcfOUT.write(7, LOW);
    uint8_t value = pcfIN1.read8();
    pcfOUT.write(7, HIGH);
    uint8_t b = bitRead(value, PIN_BUTTON_BOTTOM);
    testButton(BUTTON_BOTTOM, b);
    b = bitRead(value, PIN_BUTTON_MIDDLE);
    testButton(BUTTON_MIDDLE, b);
    b = bitRead(value, PIN_BUTTON_TOP);
    testButton(BUTTON_TOP, b);
}

#endif