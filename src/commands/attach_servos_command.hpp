#ifndef ATTACH_SERVOS_COMMAND_HPP
#define ATTACH_SERVOS_COMMAND_HPP

#include "command.hpp"
#include "pico/stdlib.h"
#include "servo2040.hpp"

using namespace servo;

class AttachServosCommand : public Command {

private:

    ServoCluster& servos;

public:

    AttachServosCommand(ServoCluster& servos) :
        servos(servos) {}

    void execute(const std::vector<uint8_t>& args) override {

        servos.enable_all();

    }

    std::vector<uint8_t> getResponse() override {
        return {0x00};
    }
};

#endif // ATTACH_SERVOS_COMMAND_HPP