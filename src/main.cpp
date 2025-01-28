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
#include "commands/set_servo_angle_command.hpp"
#include "commands/set_servo_angles_command.hpp"
#include "commands/connect_relay_command.hpp"
#include "commands/disconnect_relay_command.hpp"

#include "utils/analog_reader.hpp"
#include "utils/relay.hpp"

#include "config.hpp"

using namespace plasma;
using namespace servo;


int main() {

    stdio_init_all();  // Initialize standard I/O for USB or serial communication

    // Initialize the relay pins
    gpio_init(RELAY_PIN);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);

    // Shared object pool
    ServoCluster servos = ServoCluster(pio0, 0, servo2040::SERVO_1, servo2040::NUM_SERVOS);
    WS2812 leds(servo2040::NUM_LEDS, pio1, 0, servo2040::LED_DATA);
    Relay relay(RELAY_PIN);
    AnalogReader reader;

    servos.init();
    leds.start();

    // Initialize the dispatcher
    CommandDispatcher dispatcher;

    // Register the commands
    dispatcher.registerCommand(GET_VOLTAGE_COMMAND, std::make_unique<GetVoltageCommand>(reader));
    dispatcher.registerCommand(GET_CURRENT_COMMAND, std::make_unique<GetCurrentCommand>(reader));
    dispatcher.registerCommand(READ_SENSOR_COMMAND, std::make_unique<ReadSensorCommand>(reader));
    dispatcher.registerCommand(SET_LED_COMMAND, std::make_unique<SetLEDCommand>(leds));
    dispatcher.registerCommand(SET_LEDS_COMMAND, std::make_unique<SetLEDsCommand>(leds));
    dispatcher.registerCommand(ATTACH_SERVOS_COMMAND, std::make_unique<AttachServosCommand>(servos));
    dispatcher.registerCommand(DETACH_SERVOS_COMMAND, std::make_unique<DetachServosCommand>(servos));
    dispatcher.registerCommand(SET_SERVO_PULSE_COMMAND, std::make_unique<SetServoPulseCommand>(servos));
    dispatcher.registerCommand(SET_SERVO_PULSES_COMMAND, std::make_unique<SetServoPulsesCommand>(servos));
    dispatcher.registerCommand(SET_SERVO_ANGLE_COMMAND, std::make_unique<SetServoAngleCommand>(servos));
    dispatcher.registerCommand(SET_SERVO_ANGLES_COMMAND, std::make_unique<SetServoAnglesCommand>(servos));
    dispatcher.registerCommand(CONNECT_RELAY_COMMAND, std::make_unique<ConnectRelayCommand>(relay));
    dispatcher.registerCommand(DISCONNECT_RELAY_COMMAND, std::make_unique<DisconnectRelayCommand>(relay));

    // Display an animation with the LEDs to signal that the robot is ready
    for (uint8_t i = 0; i < servo2040::NUM_LEDS; ++i) {
        dispatcher.dispatch(0x04, {i, 208, 107, 51});   // This is a cool shade of orange
        sleep_ms(50);                                  // wait for 0.5 seconds
        dispatcher.dispatch(0x04, {i, 0, 0, 0});
    }

    std::vector<uint8_t> response;

    while (true) {

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

                // Take the opCode and process the rest as arguments for the respective operation
                // message = {0xAA, opCode, args, 0xFF}
                uint8_t opCode = buffer[1];
                std::vector<uint8_t> args(buffer.begin() + 1, buffer.end() - 1);

                /*
                // Display the opcode on the LEDs
                for (uint8_t i = 0; i < 6; ++i) {
                    bool isOn = opCode & (1 << i); // Check if the i-th bit is 1
                    if (isOn)
                        dispatcher.dispatch(0x04, {i, 208, 107, 51});   // Same cool shade of orange
                    else
                        dispatcher.dispatch(0x04, {i, 0, 0, 0});
                }
                */

                // Dispatch the command and get the response
                response = dispatcher.dispatch(opCode, args);

                // Send the response back
                for (uint8_t byte : response) {
                    putchar(byte);
                }
            }
        }

        #ifdef AUTO_DISCONNECT_UPON_CURRENT_LIMIT

        std::vector<uint8_t> current_bytes = dispatcher.dispatch(GET_CURRENT_COMMAND);
        float current = vec2float(current_bytes);
        if (current > CURRENT_THRESHOLD) {
            dispatcher.dispatch(DISCONNECT_RELAY_COMMAND);
        }

        #endif

    }

    return 0;
}
