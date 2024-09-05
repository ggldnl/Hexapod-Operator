#ifndef DETACH_SERVOS_COMMAND_HPP
#define DETACH_SERVOS_COMMAND_HPP

#include "command.hpp"
#include "pico/stdlib.h"
#include "servo2040.hpp"

using namespace servo;

class DetachServosCommand : public Command {

private:

    ServoCluster& servos;

public:

    DetachServosCommand(ServoCluster& servos) :
        servos(servos) {}

    void execute(const std::vector<uint8_t>& args) override {

        servos.disable_all();

    }

    std::vector<uint8_t> getResponse() override {
        return {0x00};
    }
};

#endif // DETACH_SERVOS_COMMAND_HPP