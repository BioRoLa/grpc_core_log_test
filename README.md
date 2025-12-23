# Log Test Subscriber

This program subscribes to the `/log` topic published by fpga_driver and displays log messages with color-coded log levels.

## Features

- Subscribes to `/log` topic
- Color-coded output based on log level:
  - **CYAN**: TRACE - Detailed trace information
  - **BLUE**: DEBUG - Debug information
  - **GREEN**: INFO - Informational messages
  - **YELLOW**: WARN - Warning messages
  - **RED**: ERROR - Error messages
  - **BOLD RED**: FATAL - Fatal error messages
- Formatted timestamp display
- Node name identification

## Build

```bash
cd log_test
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$HOME/corgi_ws/install
make -j16
```

## Run

First, make sure `grpccore` is running:
```bash
grpccore  # terminal 1
```

Then run the fpga_driver:
```bash
cd ~/corgi_ws/fpga_driver/build
./fpga_driver  # terminal 2
```

Finally, run the log subscriber:
```bash
cd ~/corgi_ws/log_test/build
./log_subscriber  # terminal 3
```

## Usage

The log subscriber will automatically connect to the `/log` topic and display all log messages published by fpga_driver with color-coded formatting.

Press Ctrl+C to exit the program.

## Example Output

```
[2024-01-15 10:30:45.123456] [INFO ] [fpga_driver] FPGA Initialized
[2024-01-15 10:30:45.234567] [DEBUG] [fpga_driver] Session opened
[2024-01-15 10:30:45.345678] [WARN ] [fpga_driver] CAN timeout detected
[2024-01-15 10:30:45.456789] [ERROR] [fpga_driver] Communication error
```
