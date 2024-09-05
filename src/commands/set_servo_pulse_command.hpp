#ifndef SET_SERVO_PULSE_COMMAND_HPP
#define SET_SERVO_PULSE_COMMAND_HPP

#include "command.hpp"
#include "utils/conversion.hpp"

using namespace servo;

class SetServoPulseCommand : public Command {

private:

    ServoCluster& servos;

public:

    SetServoPulseCommand(ServoCluster& servos) :
        servos(servos) {}

    void execute(const std::vector<uint8_t>& args) override {
        
        // Error: one byte required
        if (args.size() != 1) {
            return;
        }

        uint8_t pin = args[0];
        float pulse_width = vec2float(args, 1);        

        // Set the pulse
        servos.pulse(pin, pulse_width);
    }

    std::vector<uint8_t> getResponse() override {
        return {0x00}
    }
};

#endif // SET_SERVO_PULSE_COMMAND_HPP