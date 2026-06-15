#pragma once

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
* LCD_D_REMOVE_COLS_AND_ROWS_VARS
* LCD_D_REMOVE_CUR_X_AND_Y_VARS
* LCD_D_USE_BUFFER
* LCD_D_FULL_ACCESS
* LCD_D_ONLY_INIT_AND_WRITE_FUNCTIONS
* LCD_D_DISABLE_RESET_FUNCTION
* LCD_D_NO_I2C
* LCD_S_BUFFER_SIZE

*/

#include <Arduino.h>

#include <inttypes.h>

#ifdef LCD_D_ONLY_INIT_AND_WRITE_FUNCTIONS
#ifndef LCD_D_DISABLE_SPECIAL_CHARACTERS
#define LCD_D_DISABLE_SPECIAL_CHARACTERS
#endif
#ifndef LCD_D_DISABLE_PRINT
#define LCD_D_DISABLE_PRINT
#endif
#ifndef LCD_D_DISABLE_4_BIT_MODE
#define LCD_D_DISABLE_4_BIT_MODE
#endif
#ifndef LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL
#define LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL
#endif
#ifndef LCD_D_DISABLE_CLEAR_FUNCTION
#define LCD_D_DISABLE_CLEAR_FUNCTION
#endif
#ifndef LCD_D_DISABLE_RETURN_HOME_FUNCTION
#define LCD_D_DISABLE_RETURN_HOME_FUNCTION
#endif
#ifndef LCD_D_DISABLE_CUSTOM_CHARACTERS
#define LCD_D_DISABLE_CUSTOM_CHARACTERS
#endif
#ifndef LCD_D_DISABLE_AUTO_LINE_BREAK
#define LCD_D_DISABLE_AUTO_LINE_BREAK
#endif
#ifndef LCD_D_DISABLE_4_LINES_SUPPORT
#define LCD_D_DISABLE_4_LINES_SUPPORT
#endif
#ifndef LCD_D_DISABLE_SET_POSITION_FUNCTION
#define LCD_D_DISABLE_SET_POSITION_FUNCTION
#endif
#ifndef LCD_D_DISABLE_PRE_INIT_DELAY
#define LCD_D_DISABLE_PRE_INIT_DELAY
#endif
#ifndef LCD_D_DISABLE_FONT_SELECTION
#define LCD_D_DISABLE_FONT_SELECTION
#endif
#ifndef LCD_D_REMOVE_COLS_AND_ROWS_VARS
#define LCD_D_REMOVE_COLS_AND_ROWS_VARS
#endif
#ifndef LCD_D_REMOVE_CUR_X_AND_Y_VARS
#define LCD_D_REMOVE_CUR_X_AND_Y_VARS
#endif
#ifndef LCD_D_DISABLE_RESET_FUNCTION
#define LCD_D_DISABLE_RESET_FUNCTION
#endif
#ifdef LCD_D_USE_BUFFER
#undef LCD_D_USE_BUFFER
#endif
#endif // LCD_D_ONLY_INIT_AND_WRITE_FUNCTIONS

#ifdef LCD_D_DISABLE_SET_POSITION_FUNCTION
#ifndef LCD_D_DISABLE_AUTO_LINE_BREAK
#define LCD_D_DISABLE_AUTO_LINE_BREAK
#endif
#ifndef LCD_D_DISABLE_SPECIAL_CHARACTERS
#define LCD_D_DISABLE_SPECIAL_CHARACTERS
#endif
#ifndef LCD_D_DISABLE_CUSTOM_CHARACTERS
#define LCD_D_DISABLE_CUSTOM_CHARACTERS
#endif
#endif

#ifdef LCD_D_REMOVE_COLS_AND_ROWS_VARS
#ifndef LCD_D_DISABLE_AUTO_LINE_BREAK
#define LCD_D_DISABLE_AUTO_LINE_BREAK
#endif
#ifndef LCD_D_DISABLE_4_LINES_SUPPORT
#define LCD_D_DISABLE_4_LINES_SUPPORT
#endif
#else
#if defined(LCD_D_DISABLE_AUTO_LINE_BREAK) && defined(LCD_D_DISABLE_4_LINES_SUPPORT)
#ifndef LCD_D_REMOVE_COLS_AND_ROWS_VARS
#define LCD_D_REMOVE_COLS_AND_ROWS_VARS
#endif
#endif
#endif

#ifdef LCD_D_REMOVE_CUR_X_AND_Y_VARS
#ifndef LCD_D_DISABLE_AUTO_LINE_BREAK
#define LCD_D_DISABLE_AUTO_LINE_BREAK
#endif
#ifdef LCD_D_USE_BUFFER
#undef LCD_D_USE_BUFFER
#endif
#else
#if defined(LCD_D_DISABLE_AUTO_LINE_BREAK) && !defined(LCD_D_USE_BUFFER)
#ifndef LCD_D_REMOVE_CUR_X_AND_Y_VARS
#define LCD_D_REMOVE_CUR_X_AND_Y_VARS
#endif
#endif
#endif

#ifdef LCD_D_DISABLE_AUTO_LINE_BREAK
#ifndef LCD_D_DISABLE_SPECIAL_CHARACTERS
#define LCD_D_DISABLE_SPECIAL_CHARACTERS
#endif
#endif

#ifdef LCD_D_DISABLE_PRINT
#ifdef LCD_D_USE_BUFFER
#undef LCD_D_USE_BUFFER
#endif
#endif

#ifdef LCD_D_DISABLE_4_BIT_MODE
#ifndef LCD_D_NO_I2C
#define LCD_D_NO_I2C
#endif
#endif

#ifdef LCD_D_USE_BUFFER
#ifndef LCD_S_BUFFER_SIZE
#define LCD_S_BUFFER_SIZE 0x50
#endif
#endif

#ifndef LCD_D_NO_I2C
#include <Wire.h>
#endif

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
#ifndef LCD_D_NO_I2C
  bool i2c_mode = false;
  uint8_t i2c_data;
  uint8_t i2c_addr;
#endif // LCD_D_NO_I2C

#ifndef LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL
  bool display_en = true, cursor_en = false, cursor_blink = false;
#endif // LCD_D_DISABLE_DISPLAY_AND_CURSOR_CONTROL

#ifndef LCD_D_REMOVE_COLS_AND_ROWS_VARS
  uint8_t cols, rows; // Resolution.
#endif                // LCD_D_REMOVE_COLS_AND_ROWS_VARS

#ifndef LCD_D_REMOVE_CUR_X_AND_Y_VARS
  uint8_t cur_x = 0, cur_y = 0; // Position.
#endif                          // LCD_D_REMOVE_CUR_X_AND_Y_VARS

#ifdef LCD_D_USE_BUFFER
  uint8_t position = 0;              // Buffer position.
  uint8_t buffer[LCD_S_BUFFER_SIZE]; // Buffer.
#endif                               // LCD_D_USE_BUFFER

  void custom_dw(uint8_t pin, bool state)
  {
#ifndef LCD_D_NO_I2C
    if (i2c_mode)
      bitWrite(i2c_data, pin, state);
    else
#endif // LCD_D_NO_I2C
      digitalWrite(pin, state);
  }

  void sendByte(uint8_t byte, bool is_data = false)
  {
    custom_dw(RS, is_data);

    custom_dw(D4, byte & 16);
    custom_dw(D5, byte & 32);
    custom_dw(D6, byte & 64);
    custom_dw(D7, byte & 128);

#ifndef LCD_D_DISABLE_4_BIT_MODE
    if (four_bit_mode)
      pulseE();

    sendNibble(byte);
  }

  void sendNibble(uint8_t byte)
  {
#endif // LCD_D_DISABLE_4_BIT_MODE

    custom_dw(D0, byte & 1);
    custom_dw(D1, byte & 2);
    custom_dw(D2, byte & 4);
    custom_dw(D3, byte & 8);

#ifndef LCD_D_DISABLE_4_BIT_MODE
    pulseE();
  }

  void pulseE()
  {
#endif // LCD_D_DISABLE_4_BIT_MODE

#ifndef LCD_D_NO_I2C
    if (i2c_mode)
    {
      custom_dw(E, HIGH);
      Wire.beginTransmission(i2c_addr);
      Wire.write(i2c_data);
      Wire.endTransmission(true);

      delayMicroseconds(3);

      custom_dw(E, LOW);
      Wire.beginTransmission(i2c_addr);
      Wire.write(i2c_data);
      Wire.endTransmission(true);

      delayMicroseconds(37);
      return;
    }
#endif // LCD_D_NO_I2C

    custom_dw(E, HIGH);
    delayMicroseconds(3);
    custom_dw(E, LOW);

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
#ifdef LCD_D_REMOVE_COLS_AND_ROWS_VARS
      uint8_t rows,
#endif // LCD_D_REMOVE_COLS_AND_ROWS_VARS
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
      custom_dw(RS, LOW);
      sendNibble(3);
      delayMicroseconds(4063);
      sendNibble(3);
      delayMicroseconds(63);
      sendNibble(3);
      sendNibble(2);
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

    clear();
#ifdef LCD_D_USE_BUFFER
    flush();
#endif // LCD_D_USE_BUFFER
  }

#ifndef LCD_D_DISABLE_CLEAR_FUNCTION
  void clear()
  {
#ifdef LCD_D_USE_BUFFER
    for (int i = 0; i < LCD_S_BUFFER_SIZE; i++)
      buffer[i] = ' ';
#else
    sendByte(LCD_CMD_CLEAR_DISPLAY); // Clear display.
#endif // LCD_D_USE_BUFFER

#ifndef LCD_D_DISABLE_RETURN_HOME_FUNCTION
    returnHome(); // Very short init.
#endif            // LCD_D_DISABLE_RETURN_HOME_FUNCTION
  }
#endif // LCD_D_DISABLE_CLEAR_FUNCTION

#ifndef LCD_D_DISABLE_RETURN_HOME_FUNCTION
  void returnHome()
  {
    sendByte(LCD_CMD_RETURN_HOME); // Return home.
    delayMicroseconds(1483);

#ifndef LCD_D_REMOVE_CUR_X_AND_Y_VARS
    cur_x = 0;
    cur_y = 0;
#endif // LCD_D_REMOVE_CUR_X_AND_Y_VARS

#ifdef LCD_D_USE_BUFFER
    position = 0;
#endif // LCD_D_USE_BUFFER
  }
#endif // LCD_D_DISABLE_RETURN_HOME_FUNCTION

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
#ifndef LCD_D_REMOVE_COLS_AND_ROWS_VARS
    x %= cols;
    y %= rows;
#endif // LCD_D_REMOVE_COLS_AND_ROWS_VARS

#ifndef LCD_D_DISABLE_4_LINES_SUPPORT
    uint8_t addr = ((y % 2) * 0x40) + (cols * (y >= 2)) + x;
#else
    uint8_t addr = (y % 2) * 0x40 + x;
#endif // LCD_D_DISABLE_4_LINES_SUPPORT

    sendByte(LCD_CMD_SET_DDRAM_ADDRESS | addr); // Set DDRAM address.

#ifndef LCD_D_REMOVE_CUR_X_AND_Y_VARS
    cur_x = x;
    cur_y = y;
#endif // LCD_D_REMOVE_CUR_X_AND_Y_VARS

#ifdef LCD_D_USE_BUFFER
    position = addr;
#endif // LCD_D_USE_BUFFER
  }
#endif // LCD_D_DISABLE_SET_POSITION_FUNCTION

#ifndef LCD_D_DISABLE_CUSTOM_CHARACTERS
  void saveCustomChar(uint8_t code, uint8_t symbol[8])
  {
    sendByte(LCD_CMD_SET_CGRAM_ADDRESS | ((code % 8) << 3));
    for (uint8_t i = 0; i < 8; i++)
      sendByte(symbol[i], true);

#ifndef LCD_D_REMOVE_CUR_X_AND_Y_VARS
    setPosition(cur_x, cur_y); // Set write to DDRAM.
#else
    setPosition(0, 0);
#endif // LCD_D_REMOVE_CUR_X_AND_Y_VARS
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
#ifdef LCD_D_USE_BUFFER
      buffer[position++] = byte;
#else
      sendByte(byte, true);
      delayMicroseconds(4);
#endif // LCD_D_USE_BUFFER

#ifndef LCD_D_DISABLE_AUTO_LINE_BREAK
      if (++cur_x == cols)
        setPosition(cur_x = 0, ++cur_y);
#endif // LCD_D_DISABLE_AUTO_LINE_BREAK
    }

    return 1;
  }

#ifdef LCD_D_USE_BUFFER
  virtual int availableForWrite()
  {
    return LCD_S_BUFFER_SIZE - position;
  }

  virtual void flush()
  {
    sendByte(LCD_CMD_RETURN_HOME); // Return home.
    delayMicroseconds(1483);

    for (int i = 0; i < LCD_S_BUFFER_SIZE; i++)
    {
      sendByte(buffer[i], true);
      delayMicroseconds(40);
    }
  }
#endif // LCD_D_USE_BUFFER

#ifndef LCD_D_DISABLE_RESET_FUNCTION
  void reset()
  {
#ifndef LCD_D_DISABLE_CLEAR_FUNCTION
    clear();
#endif // LCD_D_DISABLE_CLEAR_FUNCTION
#ifdef LCD_D_USE_BUFFER
    flush();
#endif // LCD_D_USE_BUFFER
#ifndef LCD_D_DISABLE_SET_POSITION_FUNCTION
    setPosition(0, 0);
#endif // LCD_D_DISABLE_SET_POSITION_FUNCTION
  }
#endif // LCD_D_DISABLE_RESET_FUNCTION

#ifndef LCD_D_REMOVE_COLS_AND_ROWS_VARS
  Lcd2004(uint8_t RS, uint8_t E, uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7, uint8_t cols, uint8_t rows)
#else
  Lcd2004(uint8_t RS, uint8_t E, uint8_t D0, uint8_t D1, uint8_t D2, uint8_t D3, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7)
#endif // LCD_D_REMOVE_COLS_AND_ROWS_VARS
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

    custom_dw(E, LOW);

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

#ifndef LCD_D_REMOVE_COLS_AND_ROWS_VARS
    this->cols = cols;
    this->rows = rows;
#endif // LCD_D_REMOVE_COLS_AND_ROWS_VARS
  }

#ifndef LCD_D_DISABLE_4_BIT_MODE
#ifndef LCD_D_REMOVE_COLS_AND_ROWS_VARS
  Lcd2004(uint8_t RS, uint8_t E, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7, uint8_t cols, uint8_t rows) : Lcd2004(RS, E, D4, D5, D6, D7, D4, D5, D6, D7, cols, rows)
#else
  Lcd2004(uint8_t RS, uint8_t E, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7) : Lcd2004(RS, E, D4, D5, D6, D7, D4, D5, D6, D7)
#endif // LCD_D_REMOVE_COLS_AND_ROWS_VARS
  {
    four_bit_mode = true;
  }
#endif

#ifndef LCD_D_NO_I2C
#ifndef LCD_D_REMOVE_COLS_AND_ROWS_VARS
  Lcd2004(uint8_t addr, uint8_t cols, uint8_t rows) : Lcd2004(0, 2, 4, 5, 6, 7, cols, rows)
#else
  Lcd2004(uint8_t addr) : Lcd2004(0, 2, 4, 5, 6, 7)
#endif // LCD_D_REMOVE_COLS_AND_ROWS_VARS
  {
    four_bit_mode = true;
    i2c_mode = true;
    i2c_data = 1 << 3;
    i2c_addr = addr;
  }
#endif // LCD_D_NO_I2C

  ~Lcd2004() {}
};
