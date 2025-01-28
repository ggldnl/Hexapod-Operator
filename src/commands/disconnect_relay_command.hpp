#ifndef DISCONNECT_RELAY_COMMAND_HPP
#define DISCONNECT_RELAY_COMMAND_HPP

#include "command.hpp"
#include "utils/conversion.hpp"
#include "utils/relay.hpp"

using namespace servo;

class DisconnectRelayCommand : public Command {

private:

    Relay& relay;

public:

    DisconnectRelayCommand(Relay& relay) :
        relay(relay) {}

    void execute(const std::vector<uint8_t>& args) override {
        relay.off();
    }

    std::vector<uint8_t> getResponse() override {
        return {0x00};
    }
};

#endif // DISCONNECT_RELAY_COMMAND_HPP