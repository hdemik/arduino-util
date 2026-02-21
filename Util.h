#ifndef UTIL_UTIL_H
#define UTIL_UTIL_H

#include <stdint.h>

#if defined(ARDUINO) || defined(ESP8266) || defined(ESP32)
  #include <Arduino.h>
  static inline uint32_t util_nowMillis() { return ::millis(); }
#else
  #include <chrono>
  static inline uint32_t util_nowMillis() {
    using namespace std::chrono;
    return static_cast<uint32_t>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
  }
#endif

namespace Util {

class Timer {
public:
    using ms_t = uint32_t;

    // Construct with optional interval (defaults to 1000ms). 
    Timer(ms_t interval = 1000)
        : _interval(interval), _start(util_nowMillis())
    {}

    // Start (or restart) the timer counting from now.
    void restart() { _start = util_nowMillis(); }

    // Set or get the interval (milliseconds).
    void setInterval(ms_t ms) { _interval = ms; }
    ms_t interval() const { return _interval; }

    // Return elapsed milliseconds since start (0 if not running).
    ms_t elapsed() const { return static_cast<ms_t>(util_nowMillis() - _start); }

    // Return remaining milliseconds until interval expires.
    // If not running returns the full interval. If expired returns 0.
    ms_t remaining() const {
        ms_t e = elapsed();
        return (e >= _interval) ? 0u : (_interval - e);
    }

    // True if the timer is running and the interval has been reached or passed.
    bool expired() const { return (elapsed() >= _interval); }

    // If expired, advance the start by one interval (useful for periodic timers)
    // and return true. Otherwise return false.
    bool checkAndRestart() {
        if (!expired()) return false;
        // advance start by interval rather than setting to now to avoid drift
        _start += _interval;
        return true;
    }

private:
    ms_t _interval;
    ms_t _start;
};

} // namespace Util

#endif // UTIL_UTIL_H