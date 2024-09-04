# Communication protocol

This document outlines the specifications for the communication protocol used to enable data exchange between the Servo2040 board and the Raspberry Pi over a serial connection. They must agreen on the instruction table and on pin configuration (input/output) beforehand.

## Instruction list

The following table describes the supported operations, their corresponding opcodes, the expected arguments, and the response format:

| Operation                 | OpCode (Hex) | Arguments                                                                                   | Response                           |
|---------------------------|--------------|---------------------------------------------------------------------------------------------|------------------------------------|
| Get Voltage               | `0x01`       | None                                                                                        | `<val>` or `0xFF` (4 or 1 byte(s)) |
| Get Current               | `0x02`       | None                                                                                        | `<val>` or `0xFF` (4 or 1 byte(s)) |
| Read Analog Pin           | `0x03`       | `<pin>` (1 byte)                                                                            | `<val>` or `0xFF` (4 or 1 byte(s)) |
| Set LED                   | `0x04`       | `<pin>` (1 byte) `<hue>` (4 bytes: float) `<brightness>` (4 bytes: float)                   | `0x00`  or `0xFF` (1 byte)         |
| Set Servo Pulse Width     | `0x05`       | `<pin>` (1 byte) `<pulse_width>` (2 bytes: values in range 0-3000)                          | `0x00`  or `0xFF` (1 byte)         |


The protocol is designed using the Command Design Pattern, which simplifies the addition of new commands. This pattern separates command creation and execution, making 
the code more modular and easier to add commands in the future.

## How it works

In this implementation, the communication between the Raspberry Pi and the Servo2040 follows these steps:

1. Serial Data Reading: The Raspberry Pi reads incoming bytes from the serial connection. The first byte represents the opcode, and the subsequent bytes are the arguments for the command.
2. Command Dispatching: The opcode is used to lookup and dispatch the corresponding command via the `CommandDispatcher`. If the opcode matches a registered command, the dispatcher executes the command with the provided arguments.
3. Command Execution: Each command class implements the `execute()` method to perform its specific task and the `getResponse()` method to return the appropriate response.
4. Response Handling: The response from the command is then sent back to the Raspberry Pi over the serial connection.

## Adding a new command

As an example we can add a command that toggles the status of a variable. It will need no arguments and return a single byte each time, `0x00`. 

### Define the command class

Create a new header file named `toggle_status_command.hpp` in the commands directory. Implement the class as follows:

```cpp
#ifndef TOGGLE_STATUS_COMMAND_HPP
#define TOGGLE_STATUS_COMMAND_HPP

#include "command.hpp"

class ToggleStatusCommand : public Command {

private:

    bool status;

public:

    ToggleStatusCommand() : status(false) {}

    void execute(const std::vector<uint8_t>& args) override {
        status = !status;
    }

    std::vector<uint8_t> getResponse() override {
        return {0x00};
    }
};

#endif // TOGGLE_STATUS_COMMAND_HPP
```

To be a valid command, the new class must extend the `Command` base class and implement the `execute()` and `getResponse()` methods. The `execute()` method contains the logic for toggling the status variable, and the `getResponse()` method returns a success response.

### Register the command with the dispatcher

Next, include the new command in your main program and register it with the dispatcher using an available opcode. Here’s how to do it:

```cpp
// Other includes
#include "commands/toggle_status_command.hpp"

// ...

int main() {

    // ...

    // Initialize the dispatcher
    CommandDispatcher dispatcher;

    // Register the commands
    dispatcher.registerCommand(0x01, std::make_unique<GetVoltageCommand>());
    dispatcher.registerCommand(0x02, std::make_unique<GetCurrentCommand>());
    // ...

    // Register the new Toggle Status command
    dispatcher.registerCommand(0x08, std::make_unique<ToggleStatusCommand>());

    // ... (rest of the main function)
}
```

I used opcode `0x08` as it's the first available one. Once registered, the dispatcher will automatically invoke the new `ToggleStatusCommand` when the opcode `0x08` is received as first byte over the serial connection.
