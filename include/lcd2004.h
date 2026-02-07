/*

Avalible #define-s:
* LCD_D_DISABLE_SPECIAL_CHARACTERS
* LCD_D_USE_CLASSICAL_SPECIAL_CHARACTERS
* LCD_D_DISABLE_PRINT
* LCD_D_DISABLE_4_BIT_MODE
* LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL
* LCD_D_DISABLE_CLEAR_FUNCTION
* LCD_D_DISABLE_RETURN_HOME_FUNCTION
* LCD_D_DISABLE_CUSTOM_CHARACTERS
* LCD_D_DISABLE_AUTO_LINE_BREAK
* LCD_D_DISABLE_4_LINES_SUPPORT
* LCD_D_DISABLE_SET_POSITION_FUNCTION
* LCD_D_DISABLE_PRE_INIT_DELAY
* LCD_D_DISABLE_PIN_MODE
* LCD_D_DISABLE_FONT_SELECTION
* LCD_D_REMOVE_COLS_AND_ROWS
* LCD_D_FULL_ACCESS
* LCD_D_ONLY_INIT_AND_WRITE_FUNCTIONS

*/

#pragma once
#include <Arduino.h>

#include <inttypes.h>

#ifdef LCD_D_ONLY_INIT_AND_WRITE_FUNCTIONS
#define LCD_D_DISABLE_SPECIAL_CHARACTERS
#define LCD_D_DISABLE_PRINT
#define LCD_D_DISABLE_4_BIT_MODE
#define LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL
#define LCD_D_DISABLE_CLEAR_FUNCTION
#define LCD_D_DISABLE_RETURN_HOME_FUNCTION
#define LCD_D_DISABLE_CUSTOM_CHARACTERS
#define LCD_D_DISABLE_AUTO_LINE_BREAK
#define LCD_D_DISABLE_4_LINES_SUPPORT
#define LCD_D_DISABLE_SET_POSITION_FUNCTION
#define LCD_D_DISABLE_PRE_INIT_DELAY
#define LCD_D_DISABLE_FONT_SELECTION
#define LCD_D_REMOVE_COLS_AND_ROWS
#endif // LCD_D_ONLY_INIT_AND_WRITE_FUNCTIONS

#ifdef LCD_D_DISABLE_SET_POSITION_FUNCTION
#define LCD_D_DISABLE_AUTO_LINE_BREAK
#define LCD_D_DISABLE_SPECIAL_CHARACTERS
#define LCD_D_DISABLE_CUSTOM_CHARACTERS
#endif // LCD_D_DISABLE_SET_POSITION_FUNCTION

#ifdef LCD_D_REMOVE_COLS_AND_ROWS
#define LCD_D_DISABLE_AUTO_LINE_BREAK
#define LCD_D_DISABLE_4_LINES_SUPPORT
#endif // LCD_D_REMOVE_COLS_AND_ROWS

#ifdef LCD_D_DISABLE_AUTO_LINE_BREAK
#define LCD_D_DISABLE_SPECIAL_CHARACTERS
#endif // LCD_D_DISABLE_AUTO_LINE_BREAK

#define LCD_CMD_CLEAR_DISPLAY 0x1
#define LCD_CMD_RETURN_HOME 0x2
#define LCD_CMD_ENTRY_MODE_SET 0x4
#define LCD_CMD_DISPLAY_CONTROL 0x8
#define LCD_CMD_FUNCTION_SET 0x20
#define LCD_CMD_SET_CGRAM_ADDRESS 0x40
#define LCD_CMD_SET_DDRAM_ADDRESS 0x80

#define LCD_FONT_5X8 0
#define LCD_FONT_5X10 1

#ifndef LCD_D_DISABLE_PRINT
class Lcd2004 : public Print
#else
class Lcd2004
#endif // LCD_D_DISABLE_PRINT
{

#ifndef LCD_D_FULL_ACCESS
protected:
#else
public:
#endif // LCD_D_FULL_ACCESS

  uint8_t RS, E, D0, D1, D2, D3, D4, D5, D6, D7; // Pins.

#ifndef LCD_D_DISABLE_4_BIT_MODE
  bool four_bit_mode = false;
#endif // LCD_D_DISABLE_4_BIT_MODE

#ifndef LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL
  bool display_en = true, cursor_en = false, cursor_blink = false;
#endif // LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL

#ifndef LCD_D_REMOVE_COLS_AND_ROWS
  uint8_t cols, rows; // Resolution.
#endif                // LCD_D_REMOVE_COLS_AND_ROWS

#ifndef LCD_D_DISABLE_AUTO_LINE_BREAK
  uint8_t cur_x, cur_y; // Position.
#endif                  // LCD_D_DISABLE_AUTO_LINE_BREAK

  void sendByte(uint8_t byte, bool is_data = false)
  {
    digitalWrite(RS, is_data);

    digitalWrite(D4, byte & 16);
    digitalWrite(D5, byte & 32);
    digitalWrite(D6, byte & 64);
    digitalWrite(D7, byte & 128);

#ifndef LCD_D_DISABLE_4_BIT_MODE
    if (four_bit_mode)
      pulseE();

    sendLowHalf(byte);
  }

  void sendLowHalf(uint8_t byte)
  {
#endif // LCD_D_DISABLE_4_BIT_MODE

    digitalWrite(D0, byte & 1);
    digitalWrite(D1, byte & 2);
    digitalWrite(D2, byte & 4);
    digitalWrite(D3, byte & 8);

#ifndef LCD_D_DISABLE_4_BIT_MODE
    pulseE();
  }

  void pulseE()
  {
#endif // LCD_D_DISABLE_4_BIT_MODE

    digitalWrite(E, HIGH);
    delayMicroseconds(3);
    digitalWrite(E, LOW);

    delayMicroseconds(37);
  }

#ifndef LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL
  void displayOnOffControlCmd()
  {
    sendByte(LCD_CMD_DISPLAY_CONTROL | (display_en << 2) | (cursor_en << 1) | cursor_blink); // Display on/off control.
  }
#endif // LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL

public:
  void init(
#ifdef LCD_D_REMOVE_COLS_AND_ROWS
      uint8_t rows,
#endif // LCD_D_REMOVE_COLS_AND_ROWS
#ifndef LCD_D_DISABLE_FONT_SELECTION
      uint8_t font = LCD_FONT_5X8,
#endif // LCD_D_DISABLE_FONT_SELECTION
      uint8_t _ = 0)
  {
#ifndef LCD_D_DISABLE_PRE_INIT_DELAY
    delay(40);
#endif // LCD_D_DISABLE_PRE_INIT_DELAY

#ifndef LCD_D_DISABLE_4_BIT_MODE
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
#endif // LCD_D_DISABLE_4_BIT_MODE

    sendByte(LCD_CMD_FUNCTION_SET
#ifndef LCD_D_DISABLE_4_BIT_MODE
             | (!four_bit_mode << 4)
#else
             | 16
#endif // LCD_D_DISABLE_4_BIT_MODE
             | ((rows != 1) << 3)
#ifndef LCD_D_DISABLE_FONT_SELECTION
             | (font << 2)
#else
             | 0
#endif // LCD_D_DISABLE_FONT_SELECTION
    ); // Function set: 8/4-bit, n-line, 5x8 font.

    sendByte(LCD_CMD_ENTRY_MODE_SET | 2); // Entry mod set: increment DDRAM addres, no display shift.

#ifndef LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL
    displayOnOffControlCmd();
#else
    sendByte(LCD_CMD_DISPLAY_CONTROL | 4);
#endif // LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL

#ifndef LCD_D_DISABLE_CLEAR_FUNCTION
    clear();
  }

  void clear()
  {
#endif // LCD_D_DISABLE_CLEAR_FUNCTION

    sendByte(LCD_CMD_CLEAR_DISPLAY); // Clear display.

#ifndef LCD_D_DISABLE_RETURN_HOME_FUNCTION
    returnHome(); // Very short init.
  }

  void returnHome()
  {
#endif // LCD_D_DISABLE_RETURN_HOME_FUNCTION

    sendByte(LCD_CMD_RETURN_HOME); // Return home.
    delayMicroseconds(1483);

#ifndef LCD_D_DISABLE_AUTO_LINE_BREAK
    cur_x = 0;
    cur_y = 0;
#endif // LCD_D_DISABLE_AUTO_LINE_BREAK
  }

#ifndef LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL
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
#endif // LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL

#ifndef LCD_D_DISABLE_SET_POSITION_FUNCTION
  void setPosition(uint8_t x, uint8_t y)
  {
#ifndef LCD_D_REMOVE_COLS_AND_ROWS
    x %= cols;
    y %= rows;
#endif // LCD_D_REMOVE_COLS_AND_ROWS

#ifndef LCD_D_DISABLE_4_LINES_SUPPORT
    sendByte(LCD_CMD_SET_DDRAM_ADDRESS | (((y % 2) * 0x40) + (cols * (y >= 2)) + x)); // Set DDRAM address.
#else
    sendByte(LCD_CMD_SET_DDRAM_ADDRESS | (y * 0x40)); // Set DDRAM address.
#endif // LCD_D_DISABLE_4_LINES_SUPPORT

#ifndef LCD_D_DISABLE_AUTO_LINE_BREAK
    cur_x = x;
    cur_y = y;
#endif // LCD_D_DISABLE_AUTO_LINE_BREAK
  }
#endif // LCD_D_DISABLE_SET_POSITION_FUNCTION

#ifndef LCD_D_DISABLE_CUSTOM_CHARACTERS
  void saveCustomChar(uint8_t code, uint8_t symbol[8])
  {
    sendByte(LCD_CMD_SET_CGRAM_ADDRESS | ((code % 8) << 3));
    for (uint8_t i = 0; i < 8; i++)
      sendByte(symbol[i], true);

#ifndef LCD_D_DISABLE_AUTO_LINE_BREAK
    setPosition(cur_x, cur_y); // Set write to DDRAM.
#else
    setPosition(0, 0);
#endif // LCD_D_DISABLE_AUTO_LINE_BREAK
  }
#endif // LCD_D_DISABLE_CUSTOM_CHARACTERS

#ifndef LCD_D_DISABLE_PRINT
  virtual size_t write(uint8_t byte) override
#else
  size_t write(uint8_t byte)
#endif // LCD_D_DISABLE_PRINT
  {
#ifndef LCD_D_DISABLE_SPECIAL_CHARACTERS
    if (byte == '\n')
    {
      setPosition(cur_x, ++cur_y);
      cur_y %= rows;

#ifndef LCD_D_USE_CLASSICAL_SPECIAL_CHARACTERS
      write('\r');
#endif // LCD_D_USE_CLASSICAL_SPECIAL_CHARACTERS
    }
    else if (byte == '\r')
      setPosition(cur_x = 0, cur_y);
    else if (byte == '\b' && cur_x > 0)
    {
      setPosition(--cur_x, cur_y);

#ifndef LCD_D_USE_CLASSICAL_SPECIAL_CHARACTERS
      write(' ');
      setPosition(--cur_x, cur_y);
#endif // LCD_D_USE_CLASSICAL_SPECIAL_CHARACTERS
    }
    else
#endif // LCD_D_DISABLE_SPECIAL_CHARACTERS

    {
      sendByte(byte, true);
      delayMicroseconds(4);

#ifndef LCD_D_DISABLE_AUTO_LINE_BREAK
      if (++cur_x == cols)
        setPosition(cur_x = 0, ++cur_y);
#endif // LCD_D_DISABLE_AUTO_LINE_BREAK
    }

    return 1;
  }

#ifndef LCD_D_REMOVE_COLS_AND_ROWS
  Lcd2004(uint8_t RS, uint8_t E, uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7, uint8_t cols, uint8_t rows)
#else
  Lcd2004(uint8_t RS, uint8_t E, uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7)
#endif // LCD_D_REMOVE_COLS_AND_ROWS
  {
#ifndef LCD_D_DISABLE_PIN_MODE
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
#endif // LCD_D_DISABLE_PIN_MODE

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

#ifndef LCD_D_REMOVE_COLS_AND_ROWS
    this->cols = cols;
    this->rows = rows;
#endif // LCD_D_REMOVE_COLS_AND_ROWS
  }

#ifndef LCD_D_DISABLE_4_BIT_MODE
#ifndef LCD_D_REMOVE_COLS_AND_ROWS
  Lcd2004(uint8_t RS, uint8_t E, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7, uint8_t cols, uint8_t rows) : Lcd2004(RS, E, D4, D5, D6, D7, D4, D5, D6, D7, cols, rows)
#else
  Lcd2004(uint8_t RS, uint8_t E, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7) : Lcd2004(RS, E, D4, D5, D6, D7, D4, D5, D6, D7)
#endif // LCD_D_REMOVE_COLS_AND_ROWS
  {
    four_bit_mode = true;
  }
#endif

  ~Lcd2004() {}
};
