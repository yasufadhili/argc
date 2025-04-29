
#include "include/util_logger.hh"

auto main() -> int {

  auto& logger { logger::Logger::instance() };

  const auto console_sink = std::make_shared<logger::ConsoleSink>();
  logger.add_sink(console_sink);
  logger.set_level(logger::LogLevel::TRACE);



  return 0;
}