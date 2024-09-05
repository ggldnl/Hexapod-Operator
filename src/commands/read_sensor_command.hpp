#ifndef READ_SENSOR_COMMAND_HPP
#define READ_SENSOR_COMMAND_HPP

#include "command.hpp"
#include "utils/conversion.hpp"
#include "utils/analog_reader.hpp"

using namespace servo;

class ReadSensorCommand : public Command {

private:

    float value;
    uint8_t pin;
    AnalogReader& reader;

public:

    ReadSensorCommand(AnalogReader& reader) :
        value(0.0f),
        pin(0),
        reader(reader) {}

    void execute(const std::vector<uint8_t>& args) override {
        
        // Error: one byte required
        if (args.size() != 1) {
            value = 0.0f;
            return;
        }

        // Valid pins go from 0 to 5; out of this range the reader will return 0.0f
        value = reader.readSensor(args[0]);
    }

    std::vector<uint8_t> getResponse() override {
        return float2vec(value);
    }
};

#endif // READ_SENSOR_COMMAND_HPP