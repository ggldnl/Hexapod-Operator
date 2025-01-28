#ifndef SET_SERVO_PULSES_COMMAND_HPP
#define SET_SERVO_PULSES_COMMAND_HPP

#include "command.hpp"
#include "utils/conversion.hpp"

using namespace servo;

class SetServoPulsesCommand : public Command {

private:

    ServoCluster& servos;

public:

    SetServoPulsesCommand(ServoCluster& servos) :
        servos(servos) {}

    void execute(const std::vector<uint8_t>& args) override {
        
        if (args.empty()) {
            return;
        }

        // The first argument specifies the number of 4-byte groups
        uint8_t group_count = args[0];

        // Each group is 4 bytes, so we expect (1 + 5 * group_count) total bytes
        if (args.size() != (1 + 5 * group_count)) {
            return;
        }

        for (uint8_t i = 0; i < group_count; ++i) {
            uint8_t base_index = 1 + i * 5;
            uint8_t pin = args[base_index];

            // Ensure the pin is within valid range
            if (pin >= servo2040::NUM_SERVOS) {
                continue; // Skip this group if the pin is out of range
            }

            float pulse_width = vec2float(args, base_index + 1);

            // Set the pulse
            servos.pulse(pin, pulse_width);
        }
    }

    std::vector<uint8_t> getResponse() override {
        return {0x00};
    }
};

#endif // SET_SERVO_PULSES_COMMAND_HPP