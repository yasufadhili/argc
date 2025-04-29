
#include "include/util_logger.hh"

auto main() -> int {

  auto& logger { logger::Logger::instance() };

  const auto console_sink = std::make_shared<logger::ConsoleSink>();
  logger.add_sink(console_sink);
  logger.set_level(logger::LogLevel::INFO);

  logger.info("Hello World!");
  logger.error("This is an error message");
  logger.warning("This is a warning");
  logger.debug("This is an debug message");
  logger.fatal("This is an fatal message");
  logger.trace("This is an trace message");

  LOG_INFO_IF(5 > 1, "Five is greater");
  LOG_DEBUG("Debug");
  LOG_TRACE("Trace");

  return 0;
}