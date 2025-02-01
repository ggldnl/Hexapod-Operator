#ifndef RELAY_HPP
#define RELAY_HPP

#include "servo2040.hpp"

using namespace servo;

class Relay {

private:
    uint8_t pin;
    bool state;

public:

    Relay(uint8_t gpio_pin) : pin(gpio_pin), state(false) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, 0); // Ensure the relay starts in OFF state
    }

    // Turn the relay ON
    void on() {
        gpio_put(pin, 1);
        state = true;
    }

    // Turn the relay OFF
    void off() {
        gpio_put(pin, 0);
        state = false;
    }

    // Toggle the relay state
    void toggle() {
        state = !state;
        gpio_put(pin, state);
    }

    // Get the current relay state
    bool is_on() const {
        return state;
    }
};

#endif // RELAY_HPP
