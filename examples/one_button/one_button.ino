/*
  Example for the MyButton.h library https://github.com/iavorvel/MyButton

  one_button.ino - push-button gestures (click, double-click, long-press)

  Connect a push-button between BUTTON_PIN and GND

  Test how the button works with and without interrupts (USE_INTERRUPTS)

*/

#include "MyButton.h"

#define USE_INTERRUPTS
#define BUTTON_PIN 3

MyButton button(BUTTON_PIN);

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.print("The button is connected to pin ");
  Serial.println(BUTTON_PIN);

#ifdef USE_INTERRUPTS
  if (button.useInterrupt())
    Serial.println("Using interrupts!");
  else
    Serial.println("This pin does not accept interrupts...\nUsing a less-efficient algorithm.");
#endif
}

void loop()
{
  // button responds to every gesture individually
  switch (button.check())
  {
  case MyButton::CLICK:
    Serial.println("Button> click!");
    break;
  case MyButton::DOUBLE_CLICK:
    Serial.println("Button> double-click!!");
    break;
  case MyButton::LONG_PRESS:
    Serial.println("Button> long press...");
    break;
  default:
    break;
  }
}
