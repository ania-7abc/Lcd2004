#pragma once

#include <Arduino.h>

#ifndef LCD_NO_I2C
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

class Lcd2004 : public Print
{
protected:
    // Pins
    uint8_t RS_, E_,
            D0_, D1_, D2_, D3_,
            D4_, D5_, D6_, D7_;

    uint8_t cols_, rows_; // Resolution

#ifndef LCD_LOW_MEM
    int8_t BLA_; // if < 0 no backlight control
#endif

    bool four_bit_mode_ = false;

#ifndef LCD_NO_I2C
    bool i2c_mode_;
    uint8_t i2c_data_ = 0;
    uint8_t i2c_addr_ = 0;
#endif

#ifndef LCD_LOW_MEM
    bool display_ = true;
    bool cursor_ = false, cursor_blink_ = false;
#endif

    uint8_t cur_x_ = 0, cur_y_ = 0; // Position

#ifdef LCD_USE_BUFFER
    uint8_t position_ = 0; // Buffer position
    uint8_t buffer_[0x68] = {}; // Buffer
#endif

#ifdef LCD_NO_I2C
    static void setPin(const uint8_t pin, const bool state)
    {
        digitalWrite(pin, state);
    }
#else
    void setPin(const uint8_t pin, const bool state)
    {
        if (i2c_mode_)
            bitWrite(i2c_data_, pin, state);
        else
            digitalWrite(pin, state);
    }
#endif

    void sendByte(const uint8_t byte, const bool is_data = false)
    {
        setPin(RS_, is_data);

        setPin(D4_, byte & 16);
        setPin(D5_, byte & 32);
        setPin(D6_, byte & 64);
        setPin(D7_, byte & 128);

        if (four_bit_mode_)
            pulseEnable();

        sendNibble(byte);
    }

    void sendNibble(const uint8_t byte)
    {
        setPin(D0_, byte & 1);
        setPin(D1_, byte & 2);
        setPin(D2_, byte & 4);
        setPin(D3_, byte & 8);

        pulseEnable();
    }

#ifndef LCD_NO_I2C
    void pulseEnable()
    {
        if (i2c_mode_)
        {
            setPin(E_, HIGH);
            Wire.beginTransmission(i2c_addr_);
            Wire.write(i2c_data_);
            Wire.endTransmission(true);

            delayMicroseconds(3);

            setPin(E_, LOW);
            Wire.beginTransmission(i2c_addr_);
            Wire.write(i2c_data_);
            Wire.endTransmission(true);

            delayMicroseconds(37);
            return;
        }

        setPin(E_, HIGH);
        delayMicroseconds(3);
        setPin(E_, LOW);

        delayMicroseconds(37);
    }
#else
    void pulseEnable() const
    {
        setPin(E_, HIGH);
        delayMicroseconds(3);
        setPin(E_, LOW);

        delayMicroseconds(37);
    }
#endif

#ifndef LCD_LOW_MEM
    void displayControlCmd()
    {
        sendByte(LCD_CMD_DISPLAY_CONTROL | display_ << 2 | cursor_ << 1 | cursor_blink_);
    }
#endif

public:
    void init(const uint8_t font = LCD_FONT_5X8)
    {
        delay(40);

        if (four_bit_mode_)
        {
            setPin(RS_, LOW);
            sendNibble(3);
            delayMicroseconds(4063);
            sendNibble(3);
            delayMicroseconds(63);
            sendNibble(3);
            sendNibble(2);
        }

        sendByte(LCD_CMD_FUNCTION_SET | !four_bit_mode_ << 4 | (rows_ != 1) << 3 | font << 2);
        sendByte(LCD_CMD_ENTRY_MODE_SET | 2); // Entry mod set: increment DDRAM address, no display shift
        sendByte(LCD_CMD_DISPLAY_CONTROL | 4); // Turn on display

#ifndef LCD_LOW_MEM
        setBacklight(true);
#endif

        clear();
#ifdef LCD_USE_BUFFER
        flush();
#endif
    }

    void clear()
    {
#ifdef LCD_USE_BUFFER
        for (unsigned char& i : buffer_)
            i = ' ';
#else
        sendByte(LCD_CMD_CLEAR_DISPLAY); // Clear display
#endif
        delayMicroseconds(1483);
        returnHome(); // Very short init
    }

    void returnHome()
    {
        sendByte(LCD_CMD_RETURN_HOME); // Return home
        delayMicroseconds(1483);

        cur_x_ = 0;
        cur_y_ = 0;

#ifdef LCD_USE_BUFFER
        position_ = 0;
#endif
    }

#ifndef LCD_LOW_MEM
    void setOn(const bool on)
    {
        display_ = on;
        displayControlCmd();
    }

    void setCursor(const bool on, const bool blink = false)
    {
        cursor_ = on;
        cursor_blink_ = blink;
        displayControlCmd();
    }
#endif

    void setPosition(uint8_t x, uint8_t y)
    {
        x %= cols_;
        y %= rows_;

        const uint8_t addr = y % 2 * 0x40 + x + cols_ * (y >= 2);
#ifdef LCD_USE_BUFFER
        position_ = addr;
#endif
        cur_x_ = x;
        cur_y_ = y;

        sendByte(LCD_CMD_SET_DDRAM_ADDRESS | addr);
    }

#ifndef LCD_LOW_MEM
    void saveCustomChar(const uint8_t code, const uint8_t symbol[5])
    {
        sendByte(LCD_CMD_SET_CGRAM_ADDRESS | (code % 8) << 3);

        for (uint8_t row = 0; row < 8; row++)
        {
            uint8_t row_val = 0;
            for (uint8_t col = 0; col < 5; col++)
                row_val |= (symbol[4 - col] >> row & 1) << col;

            sendByte(row_val, true);
        }

        setPosition(cur_x_, cur_y_); // Set write to DDRAM, but not CGRAM
    }

protected:
    /**
     * Called before outputting a character
     * @param byte The character to be output
     * @return Continue outputting the character?
     */
    virtual bool preWrite(uint8_t& byte) { return true; }
#endif

public:
    size_t write(uint8_t byte) override
    {
#ifndef LCD_LOW_MEM
        if (!preWrite(byte))
            return 1;

        if (byte == '\n')
        {
            setPosition(0, cur_y_ + 1);
            return 1;
        }
        if (byte == '\r')
        {
            setPosition(0, cur_y_);
            return 1;
        }
        if (byte == '\b')
        {
            const uint8_t x = cur_x_ > 0 ? cur_x_ - 1 : cols_ - 1;
            const uint8_t y = cur_x_ > 0 ? cur_y_ : cur_y_ > 0 ? cur_y_ - 1 : rows_ - 1;
            setPosition(x, y);
            write(' ');
            setPosition(x, y);
            return 1;
        }
#endif

#ifdef LCD_USE_BUFFER
        buffer_[position_++] = byte;
#else
        sendByte(byte, true);
        delayMicroseconds(4);
#endif

#ifndef LCD_LOW_MEM
        if (++cur_x_ == cols_)
            write('\n');
#endif

        return 1;
    }

#ifdef LCD_USE_BUFFER
    void flush() override
    {
        sendByte(LCD_CMD_RETURN_HOME);
        delayMicroseconds(1483);

        for (int i = 0; i < (rows_ == 1 ? 80 : 0x68); i++)
        {
            if (rows_ != 1 && i == 0x28)
            {
                i = 0x40;
                sendByte(LCD_CMD_SET_DDRAM_ADDRESS | 0x40);
            }
            sendByte(buffer_[i], true);
            delayMicroseconds(40);
        }
    }
#endif

#ifndef LCD_LOW_MEM
#ifdef LCD_NO_I2C
    void setBacklight(const bool on) const
    {
        if (BLA_ < 0)
            return;
        setPin(BLA_, on);
    }
#else
    void setBacklight(const bool on)
    {
        if (BLA_ < 0)
            return;
        setPin(BLA_, on);
        if (i2c_mode_)
        {
            Wire.beginTransmission(i2c_addr_);
            Wire.write(on << 3);
            Wire.endTransmission(true);
        }
    }
#endif
#endif

    Lcd2004(const uint8_t RS, const uint8_t E,
            const uint8_t D0, const uint8_t D1, const uint8_t D2, const uint8_t D3,
            const uint8_t D4, const uint8_t D5, const uint8_t D6, const uint8_t D7,
            const uint8_t cols, const uint8_t rows, const int8_t BLA = -1, const bool _internal = false)
        : RS_(RS), E_(E),
          D0_(D0), D1_(D1), D2_(D2), D3_(D3),
          D4_(D4), D5_(D5), D6_(D6), D7_(D7),
          cols_(cols), rows_(rows)
#ifndef LCD_LOW_MEM
          , BLA_(BLA)
#endif
#ifndef LCD_NO_I2C
          , i2c_mode_(_internal)
#endif
    {
#ifndef LCD_NO_I2C
        if (!i2c_mode_)
        {
#endif
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
            pinMode(BLA, OUTPUT);
#ifndef LCD_NO_I2C
        }
#endif

        setPin(E, LOW);
    }

    Lcd2004(const uint8_t RS, const uint8_t E,
            const uint8_t D4, const uint8_t D5, const uint8_t D6, const uint8_t D7,
            const uint8_t cols, const uint8_t rows, const int8_t BLA = -1, const bool _internal = false)
        : Lcd2004(RS, E,
                  D4, D5, D6, D7,
                  D4, D5, D6, D7,
                  cols, rows, BLA, _internal)
    {
        four_bit_mode_ = true;
    }

#ifndef LCD_NO_I2C
    Lcd2004(const uint8_t addr,
            const uint8_t cols, const uint8_t rows)
        : Lcd2004(0, 2, 4, 5, 6, 7, cols, rows, 3, true)
    {
        i2c_data_ = 1 << 3;
        i2c_addr_ = addr;
    }
#endif

    virtual ~Lcd2004() = default;
};
