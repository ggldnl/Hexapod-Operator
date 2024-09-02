#ifndef GET_CURRENT_COMMAND_HPP
#define GET_CURRENT_COMMAND_HPP

#include "command.hpp"
#include "analog.hpp"
#include "analogmux.hpp"

using namespace servo;

class GetCurrentCommand : public Command {

private:

    uint8_t current;
    Analog cur_adc;    // Analog input for reading current
    AnalogMux mux;     // Multiplexer for selecting input

public:

    GetCurrentCommand() : 
        current(0),
        cur_adc(servo2040::SHARED_ADC, servo2040::CURRENT_GAIN,
            servo2040::SHUNT_RESISTOR, servo2040::CURRENT_OFFSET),
        mux(servo2040::ADC_ADDR_0, servo2040::ADC_ADDR_1, servo2040::ADC_ADDR_2, 
            PIN_UNUSED, servo2040::SHARED_ADC) {}

    void execute(const std::vector<uint8_t>& args) override {

        // Select the current sense
        mux.select(servo2040::CURRENT_SENSE_ADDR);

        // Perform the actual reading
        current = cur_adc.read_current();

    }

    std::vector<uint8_t> getResponse() override {
        return {current};
    }
};

#endif // GET_CURRENT_COMMAND_HPP
