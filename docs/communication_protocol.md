# Communication protocol

This document outlines the specifications for the communication protocol. Commands are sent from the controlling machine (Raspberry Pi) to the Servo2040 board over a serial connection. The two must agreen on the instruction table and on pin configuration (input/output) beforehand. 

## Instruction list

The following table describes the supported operations, their corresponding opcodes, the expected arguments, and the response format:

| Operation                 | OpCode (Hex) | Arguments                                                      | Response          |
|---------------------------|--------------|----------------------------------------------------------------|-------------------|
| Get Voltage               | `0x01`       | None                                                           | `<val>` (4 bytes) |
| Get Current               | `0x02`       | None                                                           | `<val>` (4 bytes) |
| Read Sensor               | `0x03`       | `<pin>` (1 byte)                                               | `<val>` (4 bytes) |
| Set LED                   | `0x04`       | `<pin>` (1 byte) `<r>` (1 byte) `<g>` (1 byte) `<b>` (1 byte)  | `0x00`  (1 byte)  |
| Set LEDs                  | `0x05`       | `<num>` (1 byte) `<pin>` `<r>` `<g>` `<b>` (4 bytes) x num     | `0x00`  (1 byte)  |
| Attach Servos             | `0x06`       | None                                                           | `0x00`  (1 byte)  |
| Detach Servos             | `0x07`       | None                                                           | `0x00`  (1 byte)  |
| Set Servo Pulse Width     | `0x08`       | `<pin>` (1 byte) `<pulse_width>` (2 bytes)                     | `0x00`  (1 byte)  |
| Set Servos Pulse Width    | `0x09`       | `<num>` (1 byte) `<pin>` `<pulse_width>` (3 bytes) x num       | `0x00`  (1 byte)  |

Description table

| Operation                 | Description                                                                                       |
|---------------------------|---------------------------------------------------------------------------------------------------|
| Get Voltage               | Reads the voltage on the external trace                                                           |
| Get Current               | Reads the current on the external trace                                                           |
| Read Sensor               | Reads the voltage value of an analog pin                                                          |
| Set LED                   | Sets the rgb value for the given LED                                                              |
| Set LEDs                  | For each LED pin, sets the respective rgb value                                                   |
| Attach Servos             | Attaches all the servos                                                                           |
| Detach Servos             | Detaches all the servos                                                                           |
| Set Servo Pulse Width     | Set the pulse width for the specified servo                                                       |
| Set Servos Pulse Width    | For each Servo pin, sets the respective pulse width                                               |


The protocol is designed using the Command Design Pattern, which simplifies the addition of new commands. 

## Implementation details

We start defining a shared object pool. 

```cpp
// Shared object pool
ServoCluster servos = ServoCluster(pio0, 0, servo2040::SERVO_1, servo2040::NUM_SERVOS);
WS2812 leds(servo2040::NUM_LEDS, pio1, 0, servo2040::LED_DATA);
AnalogReader reader;

servos.init();
leds.start();
```

Each command will take a reference to the object(s) it needs to work with. Commands that need, for example, to read from a sensor (internal or external), will have a reference to the `AnalogReader`, a utility class that encapsulates the logic for multiplexing and reading; the same way, commands that need to work with servos will take a reference to a unique `ServoCluster` object. This limits potential interference between commands and redundancy.

```cpp
// Initialize the dispatcher
CommandDispatcher dispatcher;

// Register the commands
dispatcher.registerCommand(0x01, std::make_unique<GetVoltageCommand>(reader));
```

Upon receipt of a message, the `dispatcher` handles it. The first byte, the opcode, is used to lookup and dispatch the corresponding command. If the opcode matches a registered command, the `dispatcher` executes the command with the remainig bytes in the message as arguments. The response from the command is then sent back to the controlling machine over the serial connection.

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
    dispatcher.registerCommand(0x01, std::make_unique<GetVoltageCommand>(reader));
    dispatcher.registerCommand(0x02, std::make_unique<GetCurrentCommand>(reader));
    // ...

    // Register the new Toggle Status command
    dispatcher.registerCommand(0x08, std::make_unique<ToggleStatusCommand>());

    // ... (rest of the main function)
}
```

I used opcode `0x08` as it's the first available. Once registered, the dispatcher will automatically invoke the new `ToggleStatusCommand` when the opcode `0x08` is received as first byte over the serial connection. The following bytes are treated as arguments and interpreted.
