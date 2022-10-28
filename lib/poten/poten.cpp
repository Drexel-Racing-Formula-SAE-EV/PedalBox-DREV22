#include "poten.h"

#include <Arduino.h>

poten::poten(int pin, poten_range range) : m_pin{pin}, m_range{range} {}

int poten::read() {
    return analogRead(m_pin);
}

int poten::read_percent() {
    // Maps from 100 to 0 so no throttle input is "0%"
    return map(read(), m_range.lower_bound, m_range.upper_bound, 100, 0);
}
