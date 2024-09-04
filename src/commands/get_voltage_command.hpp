#ifndef GET_VOLTAGE_COMMAND_HPP
#define GET_VOLTAGE_COMMAND_HPP

#include "command.hpp"
#include "utils/conversion.hpp"
#include "utils/analog_reader.hpp"

using namespace servo;

class GetVoltageCommand : public Command {

private:

    AnalogReader& reader;
    float voltage;

public:

    GetVoltageCommand(AnalogReader& reader) :
        reader(reader),
        voltage(0.0f) {}

    void execute(const std::vector<uint8_t>& args) override {
        voltage = reader.readVoltage();
    }

    std::vector<uint8_t> getResponse() override {
        return float2vec(voltage);
    }
};

#endif // GET_VOLTAGE_COMMAND_HPP