#ifndef GET_VOLTAGE_COMMAND_HPP
#define GET_VOLTAGE_COMMAND_HPP

#include "command.hpp"
#include "analog.hpp"
#include "analogmux.hpp"

using namespace servo;

class GetVoltageCommand : public Command {

private:

    uint8_t voltage;
    Analog vol_adc;    // Analog input for reading voltage
    AnalogMux mux;     // Multiplexer for selecting input

public:

    GetVoltageCommand() : 
        voltage(0),
        vol_adc(servo2040::SHARED_ADC, servo2040::VOLTAGE_GAIN),
        mux(servo2040::ADC_ADDR_0, servo2040::ADC_ADDR_1, servo2040::ADC_ADDR_2, 
            PIN_UNUSED, servo2040::SHARED_ADC) {}

    void execute(const std::vector<uint8_t>& args) override {
        
        // Select the voltage sense
        mux.select(servo2040::VOLTAGE_SENSE_ADDR);

        // Perform the actual reading
        voltage = vol_adc.read_voltage();

    }

    std::vector<uint8_t> getResponse() override {
        return {voltage};
    }
};

#endif // GET_VOLTAGE_COMMAND_HPP
