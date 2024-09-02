# Compilation

Before you start, take a look at this [template](https://github.com/pimoroni/pico-boilerplate?tab=readme-ov-file#before-you-start). This served as starting point to develop the firmware.

It's easier if you make a `pico` directory or similar in which you keep the SDK, Pimoroni Libraries and this project. This makes it easier to include libraries. At the end you will have this directory structure:

```
pico
   ├── Hexapod
   ├── pico-sdk
   └── pimoroni-pico
```

Feel free to use another name for the `pico` directory. I will use this out of simplicity. 

## Preparing your build environment

Install build requirements:

```bash
sudo apt update
sudo apt install cmake gcc-arm-none-eabi build-essential
```

## Download the pico SDK

Download the pico SDK in the `pico` directory:

```
cd pico
git clone https://github.com/raspberrypi/pico-sdk
cd pico-sdk
git submodule update --init
export PICO_SDK_PATH=`pwd`
cd ../
```

The `PICO_SDK_PATH` set above will only last the duration of your session. To make it persistant you can add it to your `.bashrc`.

```
echo 'export PICO_SDK_PATH="/path/to/pico-sdk"' >> ~/.bashrc
```

## Grab the Pimoroni libraries

Download the Pimoroni libraries in the `pico` directory:

```
git clone https://github.com/pimoroni/pimoroni-pico
```

## Clone this project

```
git clone https://github.com/ggldnl/Hexapod
```

If you have not or don't want to set `PICO_SDK_PATH` you can edit `.vscode/settings.json` to pass the path directly to CMake.

## Build

Create a build directory in the root folder of the project and compile.

```
mkdir build
cd build
cmake ..
make
```

Once you compile the project you will end up with a `Hexapod.uf2` file inside the `build` directory.

- Connect the servo2040 board to the computer;
- Hold down the `boot/user` button, press the `reset` button at the same time, and let go of both buttons. The RP2040 should now appear as drive to the computer;
- Drag and drop the `Hexapod.uf2` image file to the RP2040 drive, the device will automatically reboot and start the loaded program.

