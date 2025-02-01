#ifndef CONNECT_RELAY_COMMAND_HPP
#define CONNECT_RELAY_COMMAND_HPP

#include "command.hpp"
#include "utils/conversion.hpp"
#include "utils/relay.hpp"

using namespace servo;

class ConnectRelayCommand : public Command {

private:

    Relay& relay;

public:

    ConnectRelayCommand(Relay& relay) :
        relay(relay) {}

    void execute(const std::vector<uint8_t>& args) override {
        relay.on();
    }

    std::vector<uint8_t> getResponse() override {
        return {0x00};
    }
};

#endif // CONNECT_RELAY_COMMAND_HPP
