#ifndef COMMAND_DISPATCHER_HPP
#define COMMAND_DISPATCHER_HPP

#include "command.hpp"
#include <map>
#include <memory>

class CommandDispatcher {
    std::map<uint8_t, std::unique_ptr<Command>> commandMap;

public:
    void registerCommand(uint8_t opCode, std::unique_ptr<Command> command) {
        commandMap[opCode] = std::move(command);
    }

    std::vector<uint8_t> dispatch(uint8_t opCode, const std::vector<uint8_t>& args) {
        if (commandMap.find(opCode) != commandMap.end()) {
            commandMap[opCode]->execute(args);
            return commandMap[opCode]->getResponse();
        } else {
            return {0xFF, 0x01}; // Unknown command error
        }
    }
};

#endif // COMMAND_DISPATCHER_HPP
