#include "MyButton.h"

namespace ISRbutton {
int ISRcount = 0;
MyButton *pButton[3] = { nullptr, nullptr, nullptr };

void ARDUINO_ISR_ATTR isr0() {
  pButton[0]->buttonISR();
}
void ARDUINO_ISR_ATTR isr1() {
  pButton[1]->buttonISR();
}
void ARDUINO_ISR_ATTR isr2() {
  pButton[2]->buttonISR();
}

void ARDUINO_ISR_ATTR (*MyISR[3])() = { isr0, isr1, isr2 };
}

MyButton::MyButton(int _pin) {
  if (_pin < 0) return;
  pin = _pin;
  pinMode(pin, INPUT_PULLUP);
  prevState = 1;
  noISR = true;
}

bool MyButton::useInterrupt() {
  int interrupt = digitalPinToInterrupt(pin);
    if (interrupt != NOT_AN_INTERRUPT) {
      using namespace ISRbutton;
      if (ISRcount < 3) {
        pButton[ISRcount] = this;
        attachInterrupt(interrupt, MyISR[ISRcount], CHANGE);
        ISRcount++;
        noISR = false;
      } else noISR = true;
    } else noISR = true;
  return !noISR;
}

void ARDUINO_ISR_ATTR MyButton::buttonISR() {
  unsigned long now = millis();
  if (now - btn_times[ti - bool(ti)] > btn_debounce) {
    ti *= (now - btn_times[0] < btn_reset);
    btn_times[ti] = now;
    ti += (ti != 9);
  }
}

MyButton::Action MyButton::check() {
  if ((pin > 0) && noISR) {
    int state = digitalRead(pin);
    if (state != prevState) {
      prevState = state;
      unsigned long now = millis();
      if (now - btn_times[ti - bool(ti)] > btn_debounce) {
        ti *= (now - btn_times[0] < btn_reset);
        btn_times[ti] = now;
        ti += (ti != 9);
      }
      // Serial.println();
      // Serial.println(state);
      // Serial.println(now);
      // Serial.println(ti);
      // for (int i = 1; i < ti; i++) {
      //   Serial.print(btn_times[i]-btn_times[i-1]);
      //   Serial.print(' ');
      // }
      // Serial.println();
    }
  }

  if (millis() - btn_times[0] < btn_reset)
    return NOPRESS;
  int clicks = ti >> 1;
  if (clicks) {
    if (clicks > 2)
      clicks = 2;
    else if ((clicks == 1) && (btn_times[1] - btn_times[0] > btn_longPress))
      clicks = 3;
    ti = 0;
    if (callback[clicks - 1]) callback[clicks - 1]();
  }

  return Action(clicks);
}

bool MyButton::on(MyButton::Action action, ButtonPressCallback cb) {
  if ((pin < 0) || (action == NOPRESS)) return false;
  callback[int(action) - 1] = cb;
  return true;
}

bool MyButton::usingInterrupt() const {
  return ((pin > 0) && !noISR);
}
