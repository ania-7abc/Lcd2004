#include <Arduino.h>
#include <inttypes.h>

#define NO_LIBRARY
#include "lcd2004.h"

Lcd2004 lcd(3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

void setup()
{
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(4, LOW);
  lcd.sendByte('t', false);
  lcd.sendByte('t', false);
  lcd.sendByte('t', false);
  lcd.sendByte('t', false);
  lcd.sendByte('t', false);
}

size_t print(const char *str)
{
  if (*str == '\0')
    return 0;
  lcd.sendByte(*str, true);
  delay(1);
  return print(str + 1) + 1;
}

void loop()
{
  lcd.sendByte(LCD_CMD_CLEAR_DISPLAY);
  delay(1);
  lcd.sendByte(LCD_CMD_RETURN_HOME);
  delay(1);
  print("Hello, World!");
  delay(700);
}
