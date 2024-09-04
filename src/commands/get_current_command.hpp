#ifndef GET_CURRENT_COMMAND_HPP
#define GET_CURRENT_COMMAND_HPP

#include "command.hpp"
#include "utils/conversion.hpp"
#include "utils/analog_reader.hpp"

using namespace servo;

class GetCurrentCommand : public Command {

private:

    AnalogReader& reader;
    float current;

public:

    GetCurrentCommand(AnalogReader& reader):
        reader(reader),
        current(0.0f) {}

    void execute(const std::vector<uint8_t>& args) override {
        current = reader.readCurrent();
    }

    std::vector<uint8_t> getResponse() override {
        return float2vec(current);
    }
};

#endif // GET_CURRENT_COMMAND_HPP