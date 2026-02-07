/*

Avalible #define-s:
* DISABLE_SPECIAL_CHARACTERS
* USE_CLASSICAL_SPECIAL_CHARACTERS
* DISABLE_PRINT
* DISABLE_4_BIT_MODE
* DISABLE_DISPLAY_AND_CURSOR_CONTROL
* DISABLE_CLEAR_FUNCTION
* DISABLE_RETURN_HOME_FUNCTION
* DISABLE_CUSTOM_CHARACTERS
* DISABLE_AUTO_LINE_BREAK
* DISABLE_4_LINES_SUPPORT
* DISABLE_SET_POSITION_FUNCTION
* DISABLE_PIN_MODE_EXECUTION
* DISABLE_PRE_INIT_DELAY
* DISABLE_PIN_MODE
* REMOVE_COLS_AND_ROWS
* FULL_ACCESS
* NO_MEMORY
* NO_LIBRARY

*/

#pragma once
#include <Arduino.h>

#include <inttypes.h>

#ifdef NO_LIBRARY
#define NO_MEMORY
#define FULL_ACCESS
#define DISABLE_PIN_MODE
#endif // NO_LIBRARY

#ifdef NO_MEMORY
#define DISABLE_SPECIAL_CHARACTERS
#define DISABLE_PRINT
#define DISABLE_4_BIT_MODE
#define DISABLE_DISPLAY_AND_CURSOR_CONTROL
#define DISABLE_CLEAR_FUNCTION
#define DISABLE_RETURN_HOME_FUNCTION
#define DISABLE_CUSTOM_CHARACTERS
#define DISABLE_AUTO_LINE_BREAK
#define DISABLE_4_LINES_SUPPORT
#define DISABLE_SET_POSITION_FUNCTION
#define DISABLE_PRE_INIT_DELAY
#define REMOVE_COLS_AND_ROWS
#endif // NO_MEMORY

#ifdef DISABLE_SET_POSITION_FUNCTION
#define DISABLE_AUTO_LINE_BREAK
#define DISABLE_SPECIAL_CHARACTERS
#define DISABLE_CUSTOM_CHARACTERS
#endif // DISABLE_SET_POSITION_FUNCTION

#ifdef REMOVE_COLS_AND_ROWS
#define DISABLE_AUTO_LINE_BREAK
#define DISABLE_4_LINES_SUPPORT
#endif // REMOVE_COLS_AND_ROWS

#ifdef DISABLE_AUTO_LINE_BREAK
#define DISABLE_SPECIAL_CHARACTERS
#endif // DISABLE_AUTO_LINE_BREAK

#define LCD_CMD_CLEAR_DISPLAY 0x1
#define LCD_CMD_RETURN_HOME 0x2
#define LCD_CMD_ENTRY_MODE_SET 0x4
#define LCD_CMD_DISPLAY_CONTROL 0x8
#define LCD_CMD_FUNCTION_SET 0x20
#define LCD_CMD_SET_CGRAM_ADDRESS 0x40
#define LCD_CMD_SET_DDRAM_ADDRESS 0x80

#define LCD_FONT_5X8 0
#define LCD_FONT_5X10 1

#ifndef DISABLE_PRINT
class Lcd2004 : public Print
#else
class Lcd2004
#endif // DISABLE_PRINT
{

#ifndef FULL_ACCESS
protected:
#else
public:
#endif // FULL_ACCESS

  uint8_t RS, E, D0, D1, D2, D3, D4, D5, D6, D7; // Pins.

#ifndef DISABLE_4_BIT_MODE
  bool four_bit_mode = false;
#endif // DISABLE_4_BIT_MODE

#ifndef DISABLE_DISPLAY_AND_CURSOR_CONTROL
  bool display_en = true, cursor_en = false, cursor_blink = false;
#endif // DISABLE_DISPLAY_AND_CURSOR_CONTROL

#ifndef REMOVE_COLS_AND_ROWS
  uint8_t cols, rows; // Resolution.
#endif                // REMOVE_COLS_AND_ROWS

#ifndef DISABLE_AUTO_LINE_BREAK
  uint8_t cur_x, cur_y; // Position.
#endif                  // DISABLE_AUTO_LINE_BREAK

  void sendByte(uint8_t byte, bool is_data = false)
  {
    digitalWrite(RS, is_data);

    digitalWrite(D4, byte & 16);
    digitalWrite(D5, byte & 32);
    digitalWrite(D6, byte & 64);
    digitalWrite(D7, byte & 128);

#ifndef DISABLE_4_BIT_MODE
    if (four_bit_mode)
      pulseE();

    sendLowHalf(byte);
  }

  void sendLowHalf(uint8_t byte)
  {
#endif // DISABLE_4_BIT_MODE

    digitalWrite(D0, byte & 1);
    digitalWrite(D1, byte & 2);
    digitalWrite(D2, byte & 4);
    digitalWrite(D3, byte & 8);

#ifndef DISABLE_4_BIT_MODE
    pulseE();
  }

  void pulseE()
  {
#endif // DISABLE_4_BIT_MODE

    digitalWrite(E, HIGH);
    delayMicroseconds(3);
    digitalWrite(E, LOW);

    delayMicroseconds(37);
  }

#ifndef DISABLE_DISPLAY_AND_CURSOR_CONTROL
  void displayOnOffControlCmd()
  {
    sendByte(LCD_CMD_DISPLAY_CONTROL | (display_en << 2) | (cursor_en << 1) | cursor_blink); // Display on/off control.
  }
#endif // DISABLE_DISPLAY_AND_CURSOR_CONTROL

public:
#ifndef NO_LIBRARY
  void init(
#ifdef REMOVE_COLS_AND_ROWS
      uint8_t rows,
#endif // REMOVE_COLS_AND_ROWS
#ifndef DISABLE_FONT_SELECTION
      uint8_t font = LCD_FONT_5X8,
#endif // DISABLE_FONT_SELECTION
      uint8_t _ = 0)
  {
#ifndef DISABLE_PRE_INIT_DELAY
    delay(40);
#endif // DISABLE_PRE_INIT_DELAY

#ifndef DISABLE_4_BIT_MODE
    if (four_bit_mode)
    {
      digitalWrite(RS, LOW);
      sendLowHalf(3);
      delayMicroseconds(4063);
      sendLowHalf(3);
      delayMicroseconds(63);
      sendLowHalf(3);
      sendLowHalf(2);
    }
#endif // DISABLE_4_BIT_MODE

    sendByte(LCD_CMD_FUNCTION_SET
#ifndef DISABLE_4_BIT_MODE
             | (!four_bit_mode << 4)
#else
             | 16
#endif // DISABLE_4_BIT_MODE
             | ((rows != 1) << 3)
#ifndef DISABLE_FONT_SELECTION
             | (font << 2)
#else
             | 0
#endif // DISABLE_FONT_SELECTION
    ); // Function set: 8/4-bit, n-line, 5x8 font.

    sendByte(LCD_CMD_ENTRY_MODE_SET | 2); // Entry mod set: increment DDRAM addres, no display shift.

#ifndef DISABLE_DISPLAY_AND_CURSOR_CONTROL
    displayOnOffControlCmd();
#else
    sendByte(LCD_CMD_DISPLAY_CONTROL | 4);
#endif // DISABLE_DISPLAY_AND_CURSOR_CONTROL

#ifndef DISABLE_CLEAR_FUNCTION
    clear();
  }

  void clear()
  {
#endif // DISABLE_CLEAR_FUNCTION

    sendByte(LCD_CMD_CLEAR_DISPLAY); // Clear display.

#ifndef DISABLE_RETURN_HOME_FUNCTION
    returnHome(); // Very short init.
  }

  void returnHome()
  {
#endif // DISABLE_RETURN_HOME_FUNCTION

    sendByte(LCD_CMD_RETURN_HOME); // Return home.
    delayMicroseconds(1483);

#ifndef DISABLE_AUTO_LINE_BREAK
    cur_x = 0;
    cur_y = 0;
#endif // DISABLE_AUTO_LINE_BREAK
  }
#endif // NO_LIBRARY

#ifndef DISABLE_DISPLAY_AND_CURSOR_CONTROL
  void setOn(bool on)
  {
    display_en = on;
    displayOnOffControlCmd();
  }
  void setCursor(bool on, bool blink)
  {
    cursor_en = on;
    cursor_blink = blink;
    displayOnOffControlCmd();
  }
#endif // DISABLE_DISPLAY_AND_CURSOR_CONTROL

#ifndef DISABLE_SET_POSITION_FUNCTION
  void setPosition(uint8_t x, uint8_t y)
  {
#ifndef REMOVE_COLS_AND_ROWS
    x %= cols;
    y %= rows;
#endif // REMOVE_COLS_AND_ROWS

#ifndef DISABLE_4_LINES_SUPPORT
    sendByte(LCD_CMD_SET_DDRAM_ADDRESS | (((y % 2) * 0x40) + (cols * (y >= 2)) + x)); // Set DDRAM address.
#else
    sendByte(LCD_CMD_SET_DDRAM_ADDRESS | (y * 0x40)); // Set DDRAM address.
#endif // DISABLE_4_LINES_SUPPORT

#ifndef DISABLE_AUTO_LINE_BREAK
    cur_x = x;
    cur_y = y;
#endif // DISABLE_AUTO_LINE_BREAK
  }
#endif // DISABLE_SET_POSITION_FUNCTION

#ifndef DISABLE_CUSTOM_CHARACTERS
  void saveCustomChar(uint8_t code, uint8_t symbol[8])
  {
    sendByte(LCD_CMD_SET_CGRAM_ADDRESS | ((code % 8) << 3));
    for (uint8_t i = 0; i < 8; i++)
      sendByte(symbol[i], true);

#ifndef DISABLE_AUTO_LINE_BREAK
    setPosition(cur_x, cur_y); // Set write to DDRAM.
#else
    setPosition(0, 0);
#endif // DISABLE_AUTO_LINE_BREAK
  }
#endif // DISABLE_CUSTOM_CHARACTERS

#ifndef NO_LIBRARY
#ifndef DISABLE_PRINT
  virtual size_t write(uint8_t byte) override
#else
  size_t write(uint8_t byte)
#endif // DISABLE_PRINT
  {
#ifndef DISABLE_SPECIAL_CHARACTERS
    if (byte == '\n')
    {
      setPosition(cur_x, ++cur_y);
      cur_y %= rows;

#ifndef USE_CLASSICAL_SPECIAL_CHARACTERS
      write('\r');
#endif // USE_CLASSICAL_SPECIAL_CHARACTERS
    }
    else if (byte == '\r')
      setPosition(cur_x = 0, cur_y);
    else if (byte == '\b' && cur_x > 0)
    {
      setPosition(--cur_x, cur_y);

#ifndef USE_CLASSICAL_SPECIAL_CHARACTERS
      write(' ');
      setPosition(--cur_x, cur_y);
#endif // USE_CLASSICAL_SPECIAL_CHARACTERS
    }
    else
#endif // DISABLE_SPECIAL_CHARACTERS

    {
      sendByte(byte, true);
      delayMicroseconds(4);

#ifndef DISABLE_AUTO_LINE_BREAK
      if (++cur_x == cols)
        setPosition(cur_x = 0, ++cur_y);
#endif // DISABLE_AUTO_LINE_BREAK
    }

    return 1;
  }
#endif // NO_LIBRARY

#ifndef REMOVE_COLS_AND_ROWS
  Lcd2004(uint8_t RS, uint8_t E, uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7, uint8_t cols, uint8_t rows)
#else
  Lcd2004(uint8_t RS, uint8_t E, uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7)
#endif // REMOVE_COLS_AND_ROWS
  {
#ifndef DISABLE_PIN_MODE
    pinMode(RS, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
#endif // DISABLE_PIN_MODE

    digitalWrite(E, LOW);

    this->RS = RS;
    this->E = E;
    this->D0 = D0;
    this->D1 = D1;
    this->D2 = D2;
    this->D3 = D3;
    this->D4 = D4;
    this->D5 = D5;
    this->D6 = D6;
    this->D7 = D7;

#ifndef REMOVE_COLS_AND_ROWS
    this->cols = cols;
    this->rows = rows;
#endif // REMOVE_COLS_AND_ROWS
  }

#ifndef DISABLE_4_BIT_MODE
#ifndef REMOVE_COLS_AND_ROWS
  Lcd2004(uint8_t RS, uint8_t E, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7, uint8_t cols, uint8_t rows) : Lcd2004(RS, E, D4, D5, D6, D7, D4, D5, D6, D7, cols, rows)
#else
  Lcd2004(uint8_t RS, uint8_t E, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7) : Lcd2004(RS, E, D4, D5, D6, D7, D4, D5, D6, D7)
#endif // REMOVE_COLS_AND_ROWS
  {
    four_bit_mode = true;
  }
#endif

  ~Lcd2004() {}
};
