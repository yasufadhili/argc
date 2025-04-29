#include <filesystem>

#include "lexer.hh"
#include "include/driver.hh"
#include "include/util_logger.hh"

namespace fs = std::filesystem;

auto main(const int argc, char* argv[]) -> int {

  auto& logger { logger::Logger::instance() };

  const auto console_sink = std::make_shared<logger::ConsoleSink>();
  logger.add_sink(console_sink);
  logger.set_level(logger::LogLevel::TRACE);

  const driver::OptionParser option_parser(argc, argv);
  config::Config config;

  if (!option_parser.parse(config)) {
    LOG_ERROR("Failed to parse config");
    return EXIT_FAILURE;
  }

  if (config.show_help) {
    driver::display_help(argv[0]);
    return EXIT_SUCCESS;
  }

  if (config.show_version) {
    driver::display_version();
    return EXIT_SUCCESS;
  }

  if (!driver::validate_input_files(config)) {
    LOG_ERROR("Failed to validate input files");
    return EXIT_FAILURE;
  }

  // Convert the input file to an absolute path
  fs::path input_path = fs::absolute(config.input_files.at(0));
  std::ifstream in(input_path);

  if (!in.is_open()) {
    LOG_ERROR("Unable to open file '" + input_path.string() + "'");
    return EXIT_FAILURE;
  }

  yy::Lexer lexer{ in, std::cerr };
  if (config.debug) {
    lexer.set_debug(true);
  }

  if (!lexer.enter_file(input_path.string())) {
    LOG_ERROR("Failed to initialise lexer input file");
    return EXIT_FAILURE;
  }

  return 0;
}