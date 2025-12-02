#ifndef _MYBUTTON_H_
#define _MYBUTTON_H_

/*
  This library introduces the `MyButton` class, which makes it easy
  to process push-button input by Arduino/ESP32.
  https://github.com/iavorvel/MyButton
*/

#include <Arduino.h>

#ifndef ARDUINO_ISR_ATTR
#define ARDUINO_ISR_ATTR
#endif

class MyButton
{
private:
  typedef void (*ButtonPressCallback)();
  bool noISR = true;
  int pin = -1;
  static const unsigned long btn_reset = 800UL, btn_debounce = 40UL, btn_longPress = 300UL;
  volatile unsigned long btn_times[10];
  volatile unsigned char ti = 0;
  int prevState = 1;
  ButtonPressCallback callback[3] = {nullptr, nullptr, nullptr};

public:
  enum Action
  {
    NO_PRESS = 0,
    CLICK,
    DOUBLE_CLICK,
    LONG_PRESS
  };

  /**
   * @brief This function is only internally invoked.
   *
   */
  void ARDUINO_ISR_ATTR buttonISR();

  /**
   * @brief Construct a new My Button object from pin.
   * No interrupt is attached by default. If you want to
   * use interrupt, call MyButton::useInterrupt()
   *
   * @param pin
   */
  MyButton(int pin);

  /**
   * @brief Attempt to set an interrupt to use with the MyButton object.
   *
   * @return true on success
   */
  bool useInterrupt();

  /**
   * @brief Check whether the MyButton object is configured to accept interrupts
   *
   * @return true
   * @return false
   */
  bool usingInterrupt() const;

  /**
   * @brief This method must be called as often as possible in the loop() function
   *
   * @return Action
   */
  Action check();

  /**
   * @brief Sets a callback/lambda for button gesture (Action)
   *
   * @param action
   * @param cb
   * @return true on success
   */
  bool on(Action action, ButtonPressCallback cb);
};

#endif
