#ifndef SET_LEDS_COMMAND_HPP
#define SET_LEDS_COMMAND_HPP

#include "command.hpp"
#include "pico/stdlib.h"
#include "servo2040.hpp"

using namespace plasma;
using namespace servo;

class SetLEDsCommand : public Command {

private:

    WS2812& led_bar;

public:

    SetLEDsCommand(WS2812& led_bar) :
        led_bar(led_bar) {}

    void execute(const std::vector<uint8_t>& args) override {

        if (args.empty()) {
            return;
        }

        // The first argument specifies the number of 4-byte groups
        uint8_t group_count = args[0];

        // Each group is 4 bytes, so we expect (1 + 4 * group_count) total bytes
        if (args.size() != (1 + 4 * group_count)) {
            return;
        }

        for (uint8_t i = 0; i < group_count; ++i) {
            uint8_t base_index = 1 + i * 4;
            uint8_t pin = args[base_index];

            // Ensure the pin is within valid range
            if (pin >= servo2040::NUM_LEDS) {
                continue; // Skip this group if the pin is out of range
            }

            // Set the LED color for the given pin
            led_bar.set_rgb(pin, args[base_index + 1], args[base_index + 2], args[base_index + 3]);
        }

    }

    std::vector<uint8_t> getResponse() override {
        return {0x00};
    }
};

#endif // SET_LEDS_COMMAND_HPP