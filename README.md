# Hexapod Operator

My hexapod robot consists of two parts: a Controller and an Operator. The Controller is responsible for generating commands, which are then sent to the Operator for execution. I used a Raspberry Pi as Controller  and a Servo2040 board as Operator, for handling low-level control of the hexapod's servos.
This repository contains the firmware for the Servo2040 board.

For a complete overview of the project refer to the [main Hexapod repository](https://github.com/ggldnl/Hexapod.git). Take also a look to the [repository containing the Controller's code](https://github.com/ggldnl/Hexapod-Controller.git). 

Below, you will find instructions on how to build and deploy the code and info on how the communication protocol I designed works.

## ⚠️ Warn

Use this branch if for whatever reason you prefer to use USB over UART.

## 🛠️ Build and deployment

Before you start, take a look at this [template](https://github.com/pimoroni/pico-boilerplate?tab=readme-ov-file#before-you-start). This served as starting point to develop the firmware.

It's easier if you make a `pico` directory or similar in which you keep the SDK, Pimoroni Libraries and this project. This makes it easier to include libraries. At the end you will have this directory structure:

```
pico
├── Hexapod-Operator
├── pico-sdk
└── pimoroni-pico
```

Feel free to use another name for the `pico` directory. I will use this out of simplicity. 

### Prepare the build environment

Install build requirements:

```bash
sudo apt update
sudo apt install cmake gcc-arm-none-eabi build-essential
```

### Download the pico SDK

Download the pico SDK in the `pico` directory:

```bash
cd pico
git clone https://github.com/raspberrypi/pico-sdk
cd pico-sdk
git submodule update --init
export PICO_SDK_PATH=`pwd`
cd ../
```

The `PICO_SDK_PATH` set above will only last the duration of your session. To make it persistant you can add it to your `.bashrc`.

```bash
echo 'export PICO_SDK_PATH="/path/to/pico-sdk"' >> ~/.bashrc
```

### Download the Pimoroni libraries

Download the Pimoroni libraries in the `pico` directory:

```bash
git clone https://github.com/pimoroni/pimoroni-pico
```

### Clone the project

```bash
git clone https://github.com/ggldnl/Hexapod-Operator
```

If you have not or don't want to set `PICO_SDK_PATH` and you are using vscode, you can edit `.vscode/settings.json` to pass the path directly to CMake.

### Build

Create a build directory in the root folder of the project and compile.

```bash
mkdir build
cd build
cmake ..
make
```

Once you compile the project you will end up with a `Hexapod.uf2` file inside the `build` directory.

### Delpoy

- Connect the servo2040 board to the computer;
- Hold down the `boot/user` button, press the `reset` button at the same time, and let go of both buttons. The Servo2040 should now appear as drive to the computer;
- Drag and drop the `Hexapod.uf2` image file to the Servo2040 drive, the device will automatically reboot and start the loaded program.

If you built the firmware on the raspberry pi that you will use for the Hexapod and you happen to be connected to it with ssh, you can:

- Connect the servo2040 board to the raspberry through usb;
- Hold down the `boot/user` button, press the `reset` button at the same time, and let go of both buttons. The Servo2040 should now appear as a block device when issuing `lsblk`;
- Look for the new drive (e.g. `/dev/sda1` mounted at `/media/<username>/RPI-RP2`);
- From the `build` directory, `mv Hexapod.uf2 /media/<username>/RPI-RP2`, the device will automatically reboot and start the loaded program.

## 📡 Communication protocol

This paragraph outlines the specifications for the communication protocol. Commands are sent from the controlling machine (Raspberry Pi) to the operator (Servo2040) over a serial connection. The two must agreen on the instruction table beforehand. 

### Instruction set

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
| Set Servo Pulse Width     | `0x08`       | `<pin>` (1 byte) `<pulse_width>` (4 bytes)                     | `0x00`  (1 byte)  |
| Set Servos Pulse Width    | `0x09`       | `<num>` (1 byte) `<pin>` `<pulse_width>` (5 bytes) x num       | `0x00`  (1 byte)  |
| Set Servo Angle           | `0x0A`       | `<pin>` (1 byte) `<angle>` (4 bytes)                           | `0x00`  (1 byte)  |
| Set Servo Angles          | `0x0B`       | `<num>` (1 byte) `<pin>` `<angle>` (5 bytes) x num             | `0x00`  (1 byte)  |
| Connect Relay          | `0x0C`       | None                                                          | `0x00`  (1 byte)  |
| Disconnect Relay       | `0x0D`       | None                                                          | `0x00`  (1 byte)  |

Description table:

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
| Set Servo Angle           | Set the angle for the specified servo                                                             |
| Set Servo Angles          | For each Servo pin, sets the respective angle                                                     |
| Connect Relay          | Turns the relay on, giving power to the servos      |
| Disconnect Relay       | Torns the relay off, disconnecting the servos       |

Leading `0xAA` and trailing `0xFF` bytes are added and serve as packet delimiters. 

The protocol is designed using the Command Design Pattern, which simplifies the addition of new commands. 

### Implementation details

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

### Adding a new command

As an example we can add a command that toggles the status of a variable. It will need no arguments and return a single byte each time, `0x00`. 

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

I used opcode `0x0E` as it's the first available. Once registered, the dispatcher will automatically invoke the new `ToggleStatusCommand` when the opcode `0x0E` is received as first byte over the serial connection. The following bytes are treated as arguments and interpreted.

## 🤝 Contribution

Feel free to contribute by opening issues or submitting pull requests. For further information, check out the [main Hexapod repository](https://github.com/ggldnl/Hexapod).
