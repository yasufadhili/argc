#pragma once
#include <chrono>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <functional>
#include <memory>

namespace logger {
  /**
 * @brief Log severity levels
 */
  enum class LogLevel {
    TRACE, // Detailed tracing information
    DEBUG, // Debugging information
    INFO, // General information
    WARNING, // Warnings
    ERROR, // Errors
    FATAL // Critical errors that cause program termination
  };

  /**
   * @brief Converts a LogLevel to its string representation
   * @param level The log level to convert
   * @return String representation of the log level
   */
  inline auto log_level_to_string(LogLevel level) -> std::string;

  /**
 * @brief Converts a string to its corresponding LogLevel
 * @param level_str The string to convert
 * @return The corresponding LogLevel or LogLevel::INFO if not found
 */
  inline auto string_to_log_level(std::string &level_str) -> LogLevel;

  /**
   * @brief Represents a log message with its metadata
  */
  struct LogMessage {
    std::chrono::system_clock::time_point timestamp;
    LogLevel level;
    std::string message;
    std::string file_name;
    size_t line_number;
    std::string function_name;
    std::thread::id thread_id;

    LogMessage(
      LogLevel lvl,
      std::string msg,
      std::string file_name,
      size_t line_number,
      std::string function_name
    );
  };

  /**
   * @brief Abstract base class for log sinks
   *
   * A sink is responsible for writing log messages to a specific destination
  */
  class LogSink {
  public:
    virtual ~LogSink() = default;

    /**
     * @brief Write a log message to the sink
     * @param log_message The message to write
    */
    virtual void write(const LogMessage &log_message) = 0;

    /**
     * @brief Set the minimum log level for this sink
     * @param level The minimum log level
    */
    auto set_level(LogLevel level) -> void;

    /**
     * @brief Get the minimum log level for this sink
     * @return The minimum log level
   */
    auto get_level(LogLevel level) const -> LogLevel;

    /**
     * @brief Set the format string for this sink
     * @param format The format string
     *
     * Format specifiers:
     * %t - timestamp
     * %l - log level
     * %m - message
     * %f - file name
     * %n - line number
     * %F - function name
     * %T - thread id
     * %% - literal % character
    */
    auto set_format(const std::string &format) -> void;

    /**
     * @brief Get the format string for this sink
     * @return The format string
     */
    auto get_format() const -> std::string;

    /**
     * @brief Check if a message with the given level should be logged
     * @param level The log level to check
     * @return true if the message should be logged, false otherwise
     */
    auto should_log(LogLevel level) const -> bool;

    /**
     * @brief Format a log message according to the format string
     * @param log_message The message to format
     * @return The formatted message
     */
    auto format_message(const LogMessage &log_message) -> void;

  protected:
    LogLevel min_level_{LogLevel::INFO};
    std::string format_{"[%t] [%l] %m [%f:%n %F]"};
  };

  /**
   * @brief A sink that writes log messages to a file
  */
  class FileSink : public LogSink {
    std::string file_path_;
    std::ofstream file_stream_;
    std::mutex mutex_;

  public:
    explicit FileSink(const std::string &file_path, bool append = true);

    ~FileSink() override;

    void write(const LogMessage &log_message) override;
  };

  /**
   * @brief A sink that writes log messages to the console
   */
  class ConsoleSink final : public LogSink {
  public:
    ConsoleSink();

    void write(const LogMessage &log_message) override;

  private:
    std::mutex mutex_;
  };

  /**
 * @brief A sink that calls a custom function with the log message
 */
  class FunctionSink final : public LogSink {
  public:
    using LogHandlerFn = std::function<void(const LogMessage&, const std::string&)>;

    /**
     * @brief Construct a new Function Sink
     * @param handler The function to call with log messages
     */
    explicit FunctionSink(LogHandlerFn handler);
    void write(const LogMessage &log_message) override;

  private:
    LogHandlerFn handler_;
    std::mutex mutex_;
  };

  /**
 * @brief The main logger class
 */
  class Logger {
  public:
    /**
     * @brief Get the singleton instance of the logger
     * @return The logger instance
     */
    static Logger& instance();

    /**
     * @brief Add a sink to the logger
     * @param sink The sink to add
     */
    auto add_sink(std::shared_ptr<LogSink> sink) -> void;

    /**
     * @brief Remove a sink from the logger
     * @param sink The sink to remove
     */
    auto remove_sink(std::shared_ptr<LogSink> sink) -> void;

    /**
     * @brief Remove all sinks from the logger
     */
    auto clear_sinks() -> void;

    /**
     * @brief Set the global minimum log level
     * @param level The minimum log level
     */
    auto set_level(LogLevel level) -> void;

    /**
     * @brief Get the global minimum log level
     * @return The minimum log level
     */
    LogLevel get_level() const;

    /**
     * @brief Log a message
     * @param level The log level
     * @param message The message to log
     * @param file_name The name of the file where the log was called
     * @param line_number The line number where the log was called
     * @param function_name The name of the function where the log was called
     */
    auto log(LogLevel level, const std::string& message,
             const std::string& file_name = "",
             int line_number = 0,
             const std::string& function_name = "") -> void;

    /**
     * @brief Log a message with the TRACE level
     */
    auto trace(const std::string& message,
               const std::string& file_name = "",
               size_t line_number = 0,
               const std::string& function_name = "") -> void;

    /**
     * @brief Log a message with the DEBUG level
     */
    auto debug(const std::string& message,
               const std::string& file_name = "",
               size_t line_number = 0,
               const std::string& function_name = "") -> void;

    /**
     * @brief Log a message with the INFO level
     */
    auto info(const std::string& message,
              const std::string& file_name = "",
              size_t line_number = 0,
              const std::string& function_name = "") -> void;

    /**
     * @brief Log a message with the WARNING level
     */
    auto warning(const std::string& message,
              const std::string& file_name = "",
              size_t line_number = 0,
              const std::string& function_name = "") -> void;

    /**
     * @brief Log a message with the ERROR level
     */
    auto error(const std::string& message,
              const std::string& file_name = "",
              size_t line_number = 0,
              const std::string& function_name = "") -> void;

    /**
     * @brief Log a message with the FATAL level
     */
    auto fatal(const std::string& message,
              const std::string& file_name = "",
              size_t line_number = 0,
              const std::string& function_name = "") -> void;

    /**
     * @brief Checks if a message with the given level should be logged
     * @param level The log level to check
     * @return true if the message should be logged, false otherwise
     */
    auto should_log(LogLevel level) const -> bool;

  private:
    Logger();
    ~Logger();

    // Prevent copying and assignment
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    //LogLevel global_level_{LogLevel::INFO};
    LogLevel global_level_;
    std::vector<std::shared_ptr<LogSink>> sinks_;
    std::mutex mutex_;

  };
}


// Convenience macros for Logging

#define LOG_TRACE(message) \
logger::Logger::instance().trace(message, __FILE__, __LINE__, __func__)

#define LOG_DEBUG(message) \
logger::Logger::instance().debug(message, __FILE__, __LINE__, __func__)

#define LOG_INFO(message) \
logger::Logger::instance().info(message, __FILE__, __LINE__, __func__)

#define LOG_WARNING(message) \
logger::Logger::instance().warning(message, __FILE__, __LINE__, __func__)

#define LOG_ERROR(message) \
logger::Logger::instance().error(message, __FILE__, __LINE__, __func__)

#define LOG_FATAL(message) \
logger::Logger::instance().fatal(message, __FILE__, __LINE__, __func__)

// Conditional logging macros
#define LOG_TRACE_IF(condition, message) \
if (condition) LOG_TRACE(message)

#define LOG_DEBUG_IF(condition, message) \
if (condition) LOG_DEBUG(message)

#define LOG_INFO_IF(condition, message) \
if (condition) LOG_INFO(message)

#define LOG_WARNING_IF(condition, message) \
if (condition) LOG_WARNING(message)

#define LOG_ERROR_IF(condition, message) \
if (condition) LOG_ERROR(message)

#define LOG_FATAL_IF(condition, message) \
if (condition) LOG_FATAL(message)
