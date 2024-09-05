#include <stdio.h>
#include "servo2040.hpp"
#include "pico/stdlib.h"

#include "dispatcher.hpp"

#include "commands/attach_servos_command.hpp"
#include "commands/detach_servos_command.hpp"
#include "commands/get_current_command.hpp"
#include "commands/get_voltage_command.hpp"
#include "commands/read_sensor_command.hpp"
#include "commands/set_led_command.hpp"
#include "commands/set_leds_command.hpp"
#include "commands/set_servo_pulse_command.hpp"
#include "commands/set_servo_pulses_command.hpp"

#include "utils/analog_reader.hpp"

using namespace plasma;
using namespace servo;


int main() {

    stdio_init_all();  // Initialize standard I/O for USB or serial communication

    // Shared object pool
    ServoCluster servos = ServoCluster(pio0, 0, servo2040::SERVO_1, servo2040::NUM_SERVOS);
    WS2812 leds(servo2040::NUM_LEDS, pio1, 0, servo2040::LED_DATA);
    AnalogReader reader;

    servos.init();
    leds.start();

    // Initialize the dispatcher
    CommandDispatcher dispatcher;

    // Register the commands
    dispatcher.registerCommand(0x04, std::make_unique<AttachServosCommand>(servos));
    dispatcher.registerCommand(0x04, std::make_unique<DetachServosCommand>(servos));
    dispatcher.registerCommand(0x02, std::make_unique<GetCurrentCommand>(reader));
    dispatcher.registerCommand(0x01, std::make_unique<GetVoltageCommand>(reader));
    dispatcher.registerCommand(0x03, std::make_unique<ReadSensorCommand>(reader));
    dispatcher.registerCommand(0x04, std::make_unique<SetLEDCommand>(leds));
    dispatcher.registerCommand(0x04, std::make_unique<SetLEDsCommand>(leds));
    dispatcher.registerCommand(0x04, std::make_unique<SetServoPulseCommand>(servos));
    dispatcher.registerCommand(0x04, std::make_unique<SetServoPulsesCommand>(servos));

    // Where to store the response
    std::vector<uint8_t> response;

    while (true) {  // Infinite loop to continuously read incoming bytes

        // Wait until the USB is connected
        if (stdio_usb_connected()) {

            std::vector<uint8_t> buffer;
            int byte = 0;

            // Read bytes until EOF is received
            while ((byte = getchar_timeout_us(10000)) != PICO_ERROR_TIMEOUT && byte != EOF) {
                buffer.push_back(static_cast<uint8_t>(byte));
            }

            // If buffer is not empty, process the command
            if (!buffer.empty()) {

                // Take the first byte (opcode) and process the rest as arguments for the respective operation
                uint8_t opCode = buffer[0];
                std::vector<uint8_t> args(buffer.begin() + 1, buffer.end());

                // Dispatch the command and get the response
                response = dispatcher.dispatch(opCode, args);

                // Send the response back
                for (uint8_t byte : response) {
                    putchar(byte);
                }
            }
        }
    }

    return 0;
}
