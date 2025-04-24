# Logger Documentation

## Overview

The Logger provides logging for the argon compiler. 
It provides a flexible, thread-safe logging system with support for multiple output destinations (sinks), configurable log levels, and customisable formatting.

Key features:
- Thread-safe logging
- Multiple output destinations (console, file, custom handler)
- Six log levels (TRACE, DEBUG, INFO, WARNING, ERROR, FATAL)
- Customisable message formatting
- File and line number tracking
- Conditional logging
- Colour-coded console output

## Table of Contents

1. [Setup](#setup)
2. [Basic Usage](#basic-usage)
3. [Log Levels](#log-levels)
4. [Logging Sinks](#logging-sinks)
5. [Message Formatting](#message-formatting)
6. [Conditional Logging](#conditional-logging)
7. [Thread Safety](#thread-safety)
8. [Configuration](#configuration)
9. [Best Practices](#best-practices)
10. [Advanced Usage](#advanced-usage)

## Setup

Include the header logger.hh` in any file where logging functionality id needed:

```cpp
#include "logger.hh"
```

## Basic Usage

The logger is implemented as a singleton that can be accessed from anywhere in the code:

```cpp
// Get the logger instance
auto& logger = logger::Logger::instance();

// Add a console sink
auto console_sink = std::make_shared<logger::ConsoleSink>();
logger.add_sink(console_sink);

// Log messages
logger.info("This is an info message");
logger.error("This is an error message");
```

For convenience, the logger provides macros that automatically include file, line number, and function name:

```cpp
LOG_INFO("Starting the compiler");
LOG_ERROR("Failed to open file");
```

```cpp

// Example logs using the convenience macros
LOG_TRACE("This is a trace message");
LOG_DEBUG("This is a debug message");
LOG_INFO("This is an info message");
LOG_WARNING("This is a warning message");
LOG_ERROR("This is an error message");
LOG_FATAL("This is a fatal message");

// Example of conditional logging
bool condition = true;
LOG_INFO_IF(condition, "This message will be logged because the condition is true");
LOG_INFO_IF(!condition, "This message will not be logged because the condition is false");

// Example of logging in a multi-threaded context
std::thread t1([]() {
    LOG_INFO("Log from thread 1");
    sleep(1);  // Simulate some work
    LOG_DEBUG("More logs from thread 1");
});

std::thread t2([]() {
    LOG_INFO("Log from thread 2");
    sleep(1);  // Simulate some work
    LOG_ERROR("Error in thread 2");
});

// Wait for threads to finish
t1.join();
t2.join();

```

## Log Levels

The logger supports six log levels, in order of increasing severity:

| Level | Description | Use Case |
|-------|-------------|----------|
| TRACE | Detailed tracing information | Fine-grained debugging of algorithms |
| DEBUG | Debugging information | Developer-level debugging |
| INFO | General information | Runtime status updates |
| WARNING | Warnings | Potential issues that don't stop execution |
| ERROR | Errors | Serious issues that may affect functionality |
| FATAL | Critical errors | Issues that prevent the program from continuing |

We can set the minimum log level to control which messages are logged:

```cpp
// Only log messages at INFO level or higher
logger.set_level(logger::LogLevel::INFO);
```

## Logging Sinks

Sinks are destinations for log messages. The logger includes three types:

### Console Sink

Logs messages to the console (stdout/stderr) with colour coding:

```cpp
auto console_sink = std::make_shared<logger::ConsoleSink>();
console_sink->set_level(logger::LogLevel::DEBUG); // Set minimum level for just this sink
logger.add_sink(console_sink);
```

### File Sink

Logs messages to a file:

```cpp
auto file_sink = std::make_shared<logger::FileSink>("argc.log", true); // Append to file
logger.add_sink(file_sink);
```

### Function Sink

Calls a custom function with each log message:

```cpp
void my_log_handler(const logger::LogMessage& log_message, const std::string& formatted_message) {
    // Custom handling
}

auto functionSink = std::make_shared<logger::FunctionSink>(my_log_handler);
logger.add_sink(functionSink);
```

## Message Formatting

Each sink can have its own format string, which controls how log messages are presented:

```cpp
file_sink->set_format("[%t] [%l] [%f:%n] %m");
```

Format specifiers:
- `%t` - Timestamp
- `%l` - Log level
- `%m` - Message
- `%f` - File name
- `%n` - Line number
- `%F` - Function name
- `%T` - Thread ID
- `%%` - Literal % character

## Conditional Logging

For performance-critical sections, we can use conditional logging macros:

```cpp
LOG_DEBUG_IF(is_debugging, "This is only logged if isDebugging is true");
```

## Thread Safety

The logger is thread-safe. Each sink has its own mutex to prevent interleaved messages, and the logger itself uses a mutex to protect its internal state.

## Configuration

We can configure each sink separately:

```cpp
// Console will show INFO and above
console_sink->set_level(logger::LogLevel::INFO);

// File will show DEBUG and above with a custom format
file_sink->set_level(logger::LogLevel::DEBUG);
file_sink->set_format("[%t] [%l] [%f:%n] %m");

// Function sink will only receive ERROR and FATAL
functionSink->set_level(logger::LogLevel::ERROR);
```

## Best Practices

1. **Set Appropriate Log Levels**:
    - Use TRACE and DEBUG for development
    - Use INFO for normal operation
    - Use WARNING, ERROR, and FATAL for issues

2. **Include Contextual Information**:
    - Add relevant details to log messages
    - Use the `%f`, `%n`, and `%F` format specifiers

3. **Log Performance-Critical Code Conditionally**:
    - Use the `LOG_*_IF` macros
    - Check if the log level is enabled before complex message construction

4. **Log Rotation**:
    - We may need to implement log rotation for file sinks in future

## Advanced Usage

### Custom Log Sinks

We can create custom log sinks by inheriting from `LogSink`:

```cpp
class DatabaseSink : public logger::LogSink {
public:
    DatabaseSink(const std::string& connection_string) {
        // Initialise database connection
    }
    
    void write(const logger::LogMessage& log_message) override {
        // Write log message to database
    }
};
```

### Integrating with Compiler Components

We might want to log specific events from different components:

```cpp
// In the lexer
LOG_DEBUG("Token recognised: " + token.to_string());

// In the parser
LOG_INFO("Starting to parse function: " + function_name);

// In the semantic analyzer
LOG_ERROR("Type mismatch in expression at " + location.to_string());

// In the code generator
LOG_INFO("Generating code for function: " + function_name);
```

### Buffered Logging

For performance-critical sections, we can implement a buffered logging approach:

```cpp
```
