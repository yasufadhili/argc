#include "../include/util_logger.hh"


using namespace logger;


auto logger::log_level_to_string(const LogLevel level) -> std::string {
  static const std::unordered_map<LogLevel, std::string> levelMap = {
    {LogLevel::TRACE, "TRACE"},
    {LogLevel::DEBUG, "DEBUG"},
    {LogLevel::INFO, "INFO"},
    {LogLevel::WARNING, "WARNING"},
    {LogLevel::ERROR, "ERROR"},
    {LogLevel::FATAL, "FATAL"}
  };
  const auto it = levelMap.find(level);
  return (it != levelMap.end()) ? it->second : "UNKNOWN";
}

auto logger::string_to_log_level(const std::string &level_str) -> LogLevel {
  static const std::unordered_map<std::string, LogLevel> levelMap = {
    {"TRACE", LogLevel::TRACE},
    {"DEBUG", LogLevel::DEBUG},
    {"INFO", LogLevel::INFO},
    {"WARNING", LogLevel::WARNING},
    {"ERROR", LogLevel::ERROR},
    {"FATAL", LogLevel::FATAL}
  };
  const auto it = levelMap.find(level_str);
  return (it != levelMap.end()) ? it->second : LogLevel::INFO;
}

LogMessage::LogMessage(const LogLevel lvl,
                       const std::string &msg,
                       const std::string &file_name,
                       const size_t line_number,
                       const std::string &function_name) : timestamp(std::chrono::system_clock::now()),
                                                           level(lvl),
                                                           message(msg),
                                                           file_name(file_name),
                                                           line_number(line_number),
                                                           function_name(function_name),
                                                           thread_id(std::this_thread::get_id()) {
}

auto LogSink::format_message(const LogMessage &log_message) const -> std::string {
  std::ostringstream result;

  size_t i = 0;
  while (i < format_.size()) {
    if (format_[i] == '%' && i + 1 < format_.size()) {
      switch (format_[i + 1]) {
        case 't': {
          // Format timestamp
          auto timeT = std::chrono::system_clock::to_time_t(log_message.timestamp);
          auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      log_message.timestamp.time_since_epoch()) % 1000;
          tm tmBuf;
          localtime_r(&timeT, &tmBuf);

          result << std::put_time(&tmBuf, "%d-%m-%Y %H:%M:%S")
              << '.' << std::setfill('0') << std::setw(3) << ms.count();
          break;
        }
        case 'l':
          // Format log level
          result << log_level_to_string(log_message.level);
          break;
        case 'm':
          // Format message
          result << log_message.message;
          break;
        case 'f':
          // Format file name
          result << log_message.file_name;
          break;
        case 'n':
          // Format line number
          result << log_message.line_number;
          break;
        case 'F':
          // Format function name
          result << log_message.function_name;
          break;
        case 'T': {
          // Format thread id
          std::ostringstream threadIdStream;
          threadIdStream << log_message.thread_id;
          result << threadIdStream.str();
          break;
        }
        case '%':
          // Literal %
          result << '%';
          break;
        default:
          // Unknown format specifier, output as is
          result << '%' << format_[i + 1];
          break;
      }
      i += 2;
    } else {
      result << format_[i++];
    }
  }

  return result.str();
}

auto LogSink::get_level() const -> LogLevel {
  return min_level_;
}

auto LogSink::set_level(LogLevel level) -> void {
  min_level_ = level;
}

auto LogSink::get_format() const -> std::string {
  return format_;
}

auto LogSink::set_format(const std::string &format) -> void {
  format_ = format;
}

auto LogSink::should_log(const LogLevel level) const -> bool {
  return level <= min_level_;
}


FileSink::FileSink(const std::string& filePath, bool append)
    : file_path_(filePath) {
  file_stream_.open(filePath, append ? std::ios::app : std::ios::trunc);
  if (!file_stream_) {
    std::cerr << "Failed to open log file: " << filePath << std::endl;
  }
}

FileSink::~FileSink() {
  if (file_stream_.is_open()) {
    file_stream_.close();
  }
}

void FileSink::write(const LogMessage &log_message) {
  if (!should_log(log_message.level) || !file_stream_.is_open()) {
    return;
  }

  const std::string formatted_message = format_message(log_message);
    
  std::lock_guard<std::mutex> lock(mutex_);
  file_stream_ << formatted_message << std::endl;
  file_stream_.flush();
}

ConsoleSink::ConsoleSink() {
  // Use a simpler default format for console output
  set_format("[%t] [%l] %m");
}

void ConsoleSink::write(const LogMessage &log_message) {
  if (!should_log(log_message.level)) {
    return;
  }

  const std::string formatted_message = format_message(log_message);
    
  std::lock_guard<std::mutex> lock(mutex_);
    
  // Choose the right output stream and set colour based on log level
  if (log_message.level >= LogLevel::ERROR) {
    std::cerr << "\033[31m" << formatted_message << "\033[0m" << std::endl; // Red for errors
  } else if (log_message.level == LogLevel::WARNING) {
    std::cerr << "\033[33m" << formatted_message << "\033[0m" << std::endl; // Yellow for warnings
  } else if (log_message.level == LogLevel::INFO) {
    std::cout << "\033[32m" << formatted_message << "\033[0m" << std::endl; // Green for info
  } else {
    std::cout << formatted_message << std::endl; // Normal for debug and trace
  }
}

FunctionSink::FunctionSink(LogHandlerFn handler)
    : handler_(std::move(handler)) {
}

void FunctionSink::write(const LogMessage &log_message) {
  if (!should_log(log_message.level) || !handler_) {
    return;
  }

  const std::string formatted_message = format_message(log_message);
    
  std::lock_guard<std::mutex> lock(mutex_);
  handler_(log_message, formatted_message);
}

Logger::Logger()
    : global_level_(LogLevel::INFO) {
}

Logger::~Logger() {
  clear_sinks();
}

Logger& Logger::instance() {
  static Logger instance;
  return instance;
}

auto Logger::add_sink(std::shared_ptr<LogSink> sink) -> void {
  if (!sink) {
    return;
  }
    
  std::lock_guard<std::mutex> lock(mutex_);
  sinks_.push_back(std::move(sink));
}

auto Logger::remove_sink(std::shared_ptr<LogSink> sink) -> void {
  if (!sink) {
    return;
  }
    
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = std::find(sinks_.begin(), sinks_.end(), sink);
  if (it != sinks_.end()) {
    sinks_.erase(it);
  }
}

auto Logger::clear_sinks() -> void {
  std::lock_guard<std::mutex> lock(mutex_);
  sinks_.clear();
}

auto Logger::set_level(LogLevel level) -> void {
  std::lock_guard<std::mutex> lock(mutex_);
  global_level_ = level;
}


LogLevel Logger::get_level() const {
  return global_level_;
}

auto Logger::should_log(const LogLevel level) const -> bool {
  return level >= global_level_;
}

auto Logger::log(const LogLevel level, const std::string &message, const std::string &file_name, const int line_number, const std::string &function_name) -> void {
  if (!should_log(level)) {
    return;
  }

  const LogMessage logMessage(level, message, file_name, line_number, function_name);

  std::lock_guard<std::mutex> lock(mutex_);
  for (const auto& sink : sinks_) {
    sink->write(logMessage);
  }
}

auto Logger::trace(const std::string &message, const std::string &file_name, size_t line_number, const std::string &function_name) -> void {
  log(LogLevel::TRACE, message, file_name, line_number, function_name);
}

auto Logger::debug(const std::string &message, const std::string &file_name, size_t line_number, const std::string &function_name) -> void {
  log(LogLevel::DEBUG, message, file_name, line_number, function_name);
}

auto Logger::info(const std::string &message, const std::string &file_name, size_t line_number, const std::string &function_name) -> void {
  log(LogLevel::INFO, message, file_name, line_number, function_name);
}

auto Logger::warning(const std::string &message, const std::string &file_name, size_t line_number, const std::string &function_name) -> void {
  log(LogLevel::WARNING, message, file_name, line_number, function_name);
}

auto Logger::error(const std::string &message, const std::string &file_name, size_t line_number, const std::string &function_name) -> void {
  log(LogLevel::ERROR, message, file_name, line_number, function_name);
}

auto Logger::fatal(const std::string &message, const std::string &file_name, size_t line_number, const std::string &function_name) -> void {
  log(LogLevel::FATAL, message, file_name, line_number, function_name);
}



