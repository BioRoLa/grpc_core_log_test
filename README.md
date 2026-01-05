# Log Test Subscriber

This program subscribes to the `/log` topic and displays log messages with color-coded log levels.

## How the Logger Works

The grpc_core Logger system is designed for simplicity:

### In any application (e.g., fpga_driver):

```cpp
// main.cpp - Initialize once
#include "Logger.h"

int main() {
    LOG_INIT("fpga_driver");  // Auto-publishes to /log topic
    
    LOG_INFO << "Program started!";
    // ...
}

// any_other_file.cpp - Just use it
#include "Logger.h"

void anyFunction() {
    LOG_INFO << "Just works!";     // No setup needed
    LOG_WARN << "Warning: " << x;
    LOG_ERROR << "Error occurred";
}
```

## Features

- Subscribes to `/log` topic
- Color-coded output based on log level:
  - **CYAN**: DEBUG - Debug information
  - **GREEN**: INFO - Informational messages
  - **YELLOW**: WARN - Warning messages
  - **RED**: ERROR - Error messages
  - **BOLD RED**: FATAL - Fatal error messages
- Formatted timestamp display
- Node name and file:line identification

## Build

```bash
cd log_test
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$HOME/corgi_ws/install -DCMAKE_INSTALL_PREFIX=$HOME/corgi_ws/install -DOPENSSL_ROOT_DIR=$HOME/corgi_ws/install/ssl
make -j16
```

## Run

First, make sure `grpccore` is running:
```bash
grpccore  # terminal 1
```

Then run the fpga_driver (or any app using LOG_*):
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

The log subscriber will automatically connect to the `/log` topic and display all log messages published by any application using the grpc_core Logger.

Press Ctrl+C to exit the program.

## Example Output

```
[2024-01-15 10:30:45.123456] [INFO ] [fpga_driver] [main.cpp:42] FPGA Initialized
[2024-01-15 10:30:45.234567] [DEBUG] [fpga_driver] [fpga_handler.cpp:128] Session opened
[2024-01-15 10:30:45.345678] [WARN ] [fpga_driver] [robot_fsm.cpp:256] CAN timeout detected
[2024-01-15 10:30:45.456789] [ERROR] [fpga_driver] [motor_fsm.cpp:89] Communication error
```

## Log Macros Reference

| Macro | Description |
|-------|-------------|
| `LOG_DEBUG` | Debug information |
| `LOG_INFO` | Informational messages |
| `LOG_WARN` | Warning messages |
| `LOG_ERROR` | Error messages |
| `LOG_FATAL` | Fatal error messages |
| `LOG_INFO_IF(cond)` | Log if condition is true |
| `LOG_WARN_ONCE` | Log only once |
| `LOG_INFO_EVERY_N(n)` | Log every N occurrences |
| `LOG_ERROR_CHANGED(cond)` | Log when condition changes to true |
