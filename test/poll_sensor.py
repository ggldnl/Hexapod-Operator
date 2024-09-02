from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt
import numpy as np
import argparse
import serial
import time


if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--port", type=str, default='/dev/ttyACM0',
                        help="Device port. Defaults to '/dev/ttyACM0'.")
    parser.add_argument("-b", "--baudrate", type=int, default=115200,
                        help="Baud rate (bytes per second). Defaults to 115200.")
    parser.add_argument("-t", "--timeout", type=int, default=1,
                        help="Timeout: seconds to wait for a response from the device. Defaults to 1.")
    parser.add_argument("-s", "--sensor", type=int, default=0x01,
                        help="Sensor index. Can be either 1 (voltage) or 2 (current).")    
    args = parser.parse_args()

    # Configure the serial port
    ser = serial.Serial(
        port=args.port,
        baudrate=args.baudrate,
        timeout=args.timeout
    )

    # Give the board some time to reset after connecting
    time.sleep(2)

    # Initialize data storage
    x_data = []  # Time
    y_data = []  # Voltage/Current

    # Create a figure and axis
    fig, ax = plt.subplots()
    line, = ax.plot(x_data, y_data, 'b-', linewidth=2)
    ax.set_xlabel('Time (s)')
    ax.set_ylabel('Voltage')
    ax.set_title('Real-Time Voltage Reading')

    # Update the plot
    def update(frame):

        # Send a single byte
        ser.write(args.sensor)

        # Wait for a response
        response = ser.read(1)

        if response:
    
            value = int.from_bytes(response, "little")  # Convert byte to integer
            current_time = time.time()  # Get current time
            print(f'{"Current" if args.sensor else "Voltage"}: {value}')

            # Append new data to the lists
            x_data.append(current_time)
            y_data.append(value)

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
ani = FuncAnimation(fig, update, frames=None, interval=50)

# Display the plot
plt.show()

# Close the serial port when done
ser.close()

"""
            # Send a single byte
            ser.write(args.sensor)

            # Wait for a response
            response = ser.read(1)

            # Check if the response is valid
            if len(response) == 2:
                
                voltage_byte = response[1]

                # Calculate elapsed time
                elapsed_time = time.time() - start_time

                # Convert the voltage byte to a voltage value
                voltage = voltage_byte  # This is an example, adjust based on your device

                # Store the data
                times.append(elapsed_time)
                voltages.append(voltage)

                # Update the plot
                line.set_xdata(times)
                line.set_ydata(voltages)
                ax.relim()
                ax.autoscale_view()
                plt.draw()
                plt.pause(0.1)  # Pause to update the plot, adjust if needed

            else:
                print("Did not receive the expected number of bytes.")

            # Delay before next poll
            time.sleep(1)  # Adjust the polling rate if needed

    except KeyboardInterrupt:
        print("Polling stopped by user.")

    finally:
        # Close the serial port
        ser.close()
        plt.ioff()  # Turn off interactive mode
        plt.show()  # Keep the plot open after the loop ends

"""