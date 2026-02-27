#pragma once

#define UTIL_UTIL_H

#include <stdint.h>

#if defined(ARDUINO) || defined(ESP8266) || defined(ESP32)
#include <Arduino.h>
static inline uint32_t util_nowMillis() {
  return ::millis();
}
#else
#include <chrono>
static inline uint32_t util_nowMillis() {
  using namespace std::chrono;
  return static_cast<uint32_t>(
    duration_cast<milliseconds>(steady_clock::now().time_since_epoch())
    .count()
  );
}
#endif

namespace Util {

class Timer {
public:
  using ms_t = uint32_t;

  // Construct with optional interval (defaults to 1000ms).
  Timer(ms_t interval = 1000) : _interval(interval), _start(util_nowMillis()) {}

  // If expired, advance the start by one interval (useful for periodic timers)
  // and return true. Otherwise return false.
  bool isExpired() const {
    if (util_nowMillis() < _start) {
      _start = util_nowMillis();
      return true; // handle millis() overflow
    } 

    if (!(util_nowMillis() - _start >= _interval)) return false;

    _start += _interval;
    return true;
  }

  void reset() { _start = util_nowMillis(); }

private:
  ms_t _interval;
  mutable ms_t _start;
};
} // namespace Util