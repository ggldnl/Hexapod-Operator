from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt
import numpy as np
import argparse
import serial
import struct
import time


if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--port", type=str, default='/dev/ttyACM0',
                        help="Device port. Defaults to '/dev/ttyACM0'.")
    parser.add_argument("-b", "--baudrate", type=int, default=115200,
                        help="Baud rate (bytes per second). Defaults to 115200.")
    parser.add_argument("-t", "--timeout", type=int, default=1,
                        help="Timeout: seconds to wait for a response from the device. Defaults to 1.")
    parser.add_argument("-s", "--sensor", type=str, default='voltage',
                        help="Sensor to be read. Can be either \'voltage\' or \'current\'.")    
    args = parser.parse_args()

    limits = {
        'voltage': (-1, 8),
        'current': (-1, 8)
    }

    # Configure the serial port
    ser = serial.Serial(
        port=args.port,
        baudrate=args.baudrate,
        # timeout=args.timeout
    )

    def send_command(opcode, *args):
        ser.write(bytes([opcode]))
        for arg in args:
            ser.write(arg)

    def get_voltage():
        send_command(0x01)
        response = ser.read(4)  # Float is 4 bytes
        return struct.unpack('f', response)[0]

    def get_current():
        send_command(0x02)
        response = ser.read(4)  # Float is 4 bytes
        return struct.unpack('f', response)[0]

    # Give the board some time to reset after connecting
    time.sleep(2)

    # Initialize data storage
    x_data = []  # Time
    y_data = []  # Voltage/Current

    # Create a figure and axis
    fig, ax = plt.subplots()
    line, = ax.plot(x_data, y_data, 'b-', linewidth=2)
    ax.set_xlabel('Time (s)')
    ax.set_ylabel(f'{args.sensor.capitalize()}')
    ax.set_title(f'Real-Time {args.sensor.capitalize()} Reading')
    ax.set_ylim(limits[args.sensor.lower()][0], limits[args.sensor.lower()][1])

    # Update the plot
    def update(frame):

        if args.sensor.lower() == 'voltage':
            data = get_voltage()
            print(f"Voltage: {data}")
        else:
            data = get_current()
            print(f"Current: {data}")
        current_time = time.time()  # Get current time

        # Append new data to the lists
        x_data.append(current_time)
        y_data.append(data)

        # Keep the last 100 points to maintain a dynamic plot
        if len(x_data) > 100:
            x_data.pop(0)
            y_data.pop(0)

        # Update the data of the line plot
        line.set_xdata(x_data)
        line.set_ydata(y_data)

        # Adjust plot limits
        ax.relim()
        ax.autoscale_view()

        return line,

# Create animation
ani = FuncAnimation(fig, update, frames=None, interval=5)

# Display the plot
plt.show()

# Close the serial port when done
ser.close()
