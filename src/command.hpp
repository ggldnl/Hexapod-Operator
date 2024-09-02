#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>

class Command {

public:

    Command() = default;
    virtual ~Command() = default;
    virtual void execute(const std::vector<uint8_t>& args) = 0;
    virtual std::vector<uint8_t> getResponse() = 0;

};

#endif // COMMAND_HPP
