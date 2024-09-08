#ifndef SET_SERVO_ANGLE_COMMAND_HPP
#define SET_SERVO_ANGLE_COMMAND_HPP

#include "command.hpp"
#include "utils/conversion.hpp"

using namespace servo;

class SetServoAngleCommand : public Command {

private:

    ServoCluster& servos;

public:

    SetServoAngleCommand(ServoCluster& servos) :
        servos(servos) {}

    void execute(const std::vector<uint8_t>& args) override {
        
        // Error: one byte required
        if (args.size() != 5) {
            return;
        }

        uint8_t pin = args[0];
        float angle = vec2float(args, 1);        

        servos.value(pin, angle);
    }

    std::vector<uint8_t> getResponse() override {
        return {0x00};
    }
};

#endif // SET_SERVO_ANGLE_COMMAND_HPP
