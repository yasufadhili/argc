#include "../include/logger.hh"


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
  const std::string &function_name) :
timestamp(std::chrono::system_clock::now()),
level(lvl),
message(msg),
file_name(file_name),
line_number(line_number),
function_name(function_name),
thread_id(std::this_thread::get_id())
{

}

