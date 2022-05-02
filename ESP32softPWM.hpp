
#ifndef _ESP32softPWM_HPP_
#define _ESP32softPWM_HPP_

#include <Arduino.h>

#define	ESP32softPWM_CHANNELS	  16
#define ESP32softPWM_TIMER_BIT  10
#define ESP32softPWM_BASE_FREQ  1000
#define ESP32softPWM_MAXVAL     ((1 << ESP32softPWM_TIMER_BIT) - 1)

class ESP32softPWM {

  private:
    uint8_t  _pin  [ESP32softPWM_CHANNELS] = {};
		uint16_t _cuval[ESP32softPWM_CHANNELS];
		uint16_t _toval[ESP32softPWM_CHANNELS];
		uint16_t _tostp[ESP32softPWM_CHANNELS];
		uint16_t _tocnt[ESP32softPWM_CHANNELS];

	public:

	void attach(uint8_t channel, uint8_t pin, uint32_t freq = ESP32softPWM_BASE_FREQ, uint8_t bitw = ESP32softPWM_TIMER_BIT) {
    pinMode(pin, OUTPUT);
    if (channel < ESP32softPWM_CHANNELS) {
      _pin[channel] = pin;    /* GPIO0 is not used */
      ledcSetup(channel, freq, bitw);
      ledcAttachPin(pin, channel);
      ledcWrite(channel, 0);
    }
  }

  void set(uint8_t channel, uint16_t toval, uint16_t tocnt) {
    tocnt += 2;
    _toval[channel] = toval;
    _tocnt[channel] = tocnt;
    if (toval > _cuval[channel]) {
      _tostp[channel] = (toval - _cuval[channel] + (tocnt - 1)) / tocnt;
    } else {
      _tostp[channel] = (_cuval[channel] - toval + (tocnt - 1)) / tocnt;
    }
    ledcWrite(channel, _cuval[channel]);
  }

  uint16_t getcnt(uint8_t channel) {
    return _tocnt[channel];
  }

  void update() {
    for (int i = 0; (i < ESP32softPWM_CHANNELS); i++) {
      if (_tocnt[i]) {
        if (_toval[i] > _cuval[i]) {
          _cuval[i] += _tostp[i];
        } else {
          _cuval[i] -= _tostp[i];
        }
        if (--_tocnt[i] < 1) {
          _cuval[i] = _toval[i];
        }
        ledcWrite(i, _cuval[i]);
#if (0)
      if (_tocnt[i] < 1) Serial.println();
      Serial.printf("%4d, %2d, %4d, %4d\n", _cuval[i], _tostp[i], _toval[i], _tocnt[i]);
#endif
      }
    }
  }

};

#endif