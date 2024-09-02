#include <stdio.h>
#include "servo2040.hpp"
#include "pico/stdlib.h"

#include "dispatcher.hpp"
#include "commands/get_voltage_command.hpp"
#include "commands/get_current_command.hpp"

using namespace plasma;
using namespace servo;


int main() {

    stdio_init_all();  // Initialize standard I/O for USB or serial communication

    // Initialize the dispatcher
    CommandDispatcher dispatcher;

    // Register the commands
    dispatcher.registerCommand(0x01, std::make_unique<GetVoltageCommand>());
    dispatcher.registerCommand(0x02, std::make_unique<GetCurrentCommand>());

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
                putchar(opCode)
                for (uint8_t byte : response) {
                    putchar(byte);
                }
            }
        }
    }

    return 0;  // Return is unnecessary here but included for completeness
}
