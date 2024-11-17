# Hexapod Operator

My hexapod robot consists of two parts: a Controller and an Operator. The Controller is responsible for generating commands, which are then sent to the Operator for execution. I used a Raspberry Pi as Controller  and a Servo2040 board as Operator, for handling low-level control of the hexapod's servos.
This repository contains the firmware for the Servo2040 board.

For a complete overview of the project refer to the [main Hexapod repository](https://github.com/ggldnl/Hexapod.git). Take also a look to the [repository containing the Controller's code](https://github.com/ggldnl/Hexapod-Controller.git). 

[Here](protocol.md) you can find more information on the communication protocol I designed.

Below, you will find instructions on how to build and deploy the code.

# Build and deploy

Before you start, take a look at this [template](https://github.com/pimoroni/pico-boilerplate?tab=readme-ov-file#before-you-start). This served as starting point to develop the firmware.

It's easier if you make a `pico` directory or similar in which you keep the SDK, Pimoroni Libraries and this project. This makes it easier to include libraries. At the end you will have this directory structure:

```
pico
├── Hexapod-Operator
├── pico-sdk
└── pimoroni-pico
```

Feel free to use another name for the `pico` directory. I will use this out of simplicity. 

## Prepare the build environment

Install build requirements:

```bash
sudo apt update
sudo apt install cmake gcc-arm-none-eabi build-essential
```

## Download the pico SDK

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

## Download the Pimoroni libraries

Download the Pimoroni libraries in the `pico` directory:

```bash
git clone https://github.com/pimoroni/pimoroni-pico
```

## Clone the project

```bash
git clone https://github.com/ggldnl/Hexapod-Operator
```

If you have not or don't want to set `PICO_SDK_PATH` you can edit `.vscode/settings.json` to pass the path directly to CMake.

## Build

Create a build directory in the root folder of the project and compile.

```bash
mkdir build
cd build
cmake ..
make
```

Once you compile the project you will end up with a `Hexapod.uf2` file inside the `build` directory.

## Delpoy

- Connect the servo2040 board to the computer;
- Hold down the `boot/user` button, press the `reset` button at the same time, and let go of both buttons. The Servo2040 should now appear as drive to the computer;
- Drag and drop the `Hexapod.uf2` image file to the Servo2040 drive, the device will automatically reboot and start the loaded program.
