/*
  Example for the MyButton.h library https://github.com/iavorvel/MyButton

  two_buttons.ino - push-button gestures (click, double-click, long-press) on two buttons

  Connect two push-buttons between BUTTON_PIN1/BUTTON_PIN2 and GND

*/

#include "MyButton.h"

#define USE_INTERRUPTS
#define BUTTON_PIN1 2
#define BUTTON_PIN2 4

MyButton button1(BUTTON_PIN1), button2(BUTTON_PIN2);

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.print("Button1 is connected to pin ");
  Serial.println(BUTTON_PIN1);

#ifdef USE_INTERRUPTS
  if (button1.useInterrupt())
    Serial.println("Using interrupts!");
  else
    Serial.println("This pin does not accept interrupts...\nUsing the less-efficient algorithm.");
#endif

  Serial.print("Button2 is connected to pin ");
  Serial.println(BUTTON_PIN2);

#ifdef USE_INTERRUPTS
  if (button2.useInterrupt())
    Serial.println("Using interrupts!");
  else
    Serial.println("This pin does not accept interrupts...\nUsing a less-efficient algorithm.");
#endif

  // button2 only responds to a double-click
  button2.on(MyButton::DOUBLE_CLICK, []()
             { Serial.println("Button2> double-click!!"); });
}

void loop()
{
  // button1 responds to every gesture individually
  switch (button1.check())
  {
  case MyButton::CLICK:
    Serial.println("Button1> click!");
    break;
  case MyButton::DOUBLE_CLICK:
    Serial.println("Button1> double-click!!");
    break;
  case MyButton::LONG_PRESS:
    Serial.println("Button1> long press...");
    break;
  default:
    break;
  }

  button2.check();
}
