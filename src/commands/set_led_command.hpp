#ifndef SET_LED_COMMAND_HPP
#define SET_LED_COMMAND_HPP

#include "command.hpp"
#include "pico/stdlib.h"
#include "servo2040.hpp"

using namespace plasma;
using namespace servo;

class SetLEDCommand : public Command {

private:

    WS2812& led_bar;

public:

    SetLEDCommand(WS2812& led_bar) :
        led_bar(led_bar) {}

    void execute(const std::vector<uint8_t>& args) override {

        if (args.size() != 4) {
            return;
        }

        uint8_t pin = args[0];
        if (pin > 5) {
            return;
        }

        led_bar.set_rgb(pin, args[1], args[2], args[3]);

    }

    std::vector<uint8_t> getResponse() override {
        return {0x00};
    }
};

#endif // SET_LED_COMMAND_HPP