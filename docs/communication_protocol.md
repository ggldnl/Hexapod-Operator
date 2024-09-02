# Communication protocol

Following are the specs for the communication protocol used to let the servo2040 and the raspberry pi exchange information.

## Instruction list

| Operation                 | OpCode (Hex) | Arguments                                                                                   | Response           |
|---------------------------|--------------|---------------------------------------------------------------------------------------------|--------------------|
| Get Voltage               | `0x01`       | None                                                                                        | `0x01 <voltage>`   |
| Get Current               | `0x02`       | None                                                                                        | `0x02 <current>`   |
| Set Servo Pulse Width     | `0x03`       | `<pin>` (1 byte) `<pulse_width>` (2 bytes: values in range 0-3000)                          | `0x03 0x00`        |
| Set Pin State             | `0x04`       | `<pin>` (1 byte) `<state>` (1 byte: 0x01 for HIGH, 0x00 for LOW)                            | `0x04 0x00`        |
| Get Digital Pin           | `0x05`       | `<pin>` (1 byte)                                                                            | `0x05 <value>`     |
| Get Analog Pin            | `0x06`       | `<pin>` (1 byte)                                                                            | `0x06 <value>`     |
| Set Multiple Parameters   | `0x07`       | `<num_pairs>` (1 byte) `<arg1> <value1> ... <argN> <valueN>` (#num_pair * (1 + 2) bytes)    | `0x07 0x00`        |
| Error                     | `0xFF`       | None                                                                                        | `0xFF <error_code>`|

