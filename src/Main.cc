#include "ArgonParser.h"
#include "ArgonLexer.h"
#include "ErrorReporter.hh"
#include "ConfigHandler.hh"
#include "SymbolCollector.hh"
#include <string>
#include <fstream>
#include <fmt/core.h>
#include <fmt/color.h>


auto main(const int argc, char *argv[]) -> int {
  argc::err::ErrorReporter error_reporter(true, 100, "compiler_errors.log");
  error_reporter.setVerbose(true);

  argc::ConfigHandler config(error_reporter);

  if (!config.parseArgs(argc, argv)) {
    fmt::print(stderr, fg(fmt::color::crimson), "Error: ");
    fmt::print(stderr, "Failed to parse command line arguments\n");
    return 1;
  }

  for (const auto &input_file_path: config.getInputFiles()) {
    fmt::print("Processing file: {}\n", input_file_path);

    std::ifstream input_file(input_file_path);
    if (!input_file.is_open()) {
      error_reporter.reportQuick(
        argc::err::ErrorCode::InvalidToken,
        argc::err::CompileStage::Lexing,
        argc::err::ErrorSeverity::Fatal,
        "Could not open input file provided"
      );
      return 1;
    }

    std::string input_text((std::istreambuf_iterator<char>(input_file)),
                           std::istreambuf_iterator<char>());
    input_file.close();

    try {
      // === LEXICAL ANALYSIS ===
      if (config.getVerbosity() >= 1) {
        fmt::print("Stage: Lexical Analysis\n");
      }

      antlr4::ANTLRInputStream input(input_text);
      ArgonLexer lexer(&input);
      antlr4::CommonTokenStream tokens(&lexer);

      // === PARSING ===
      if (config.getVerbosity() >= 1) {
        fmt::print("Stage: Parsing\n");
      }

      ArgonParser parser(&tokens);
      ArgonParser::ModuleDeclarationContext *parse_tree = parser.moduleDeclaration();

      if (!parse_tree) {
        error_reporter.reportQuick(
          argc::err::ErrorCode::SyntaxError,
          argc::err::CompileStage::Parsing,
          argc::err::ErrorSeverity::Fatal,
          "Failed to parse module declaration"
        );
        return 1;
      }

      // === SYMBOL COLLECTION ===
      if (config.getVerbosity() >= 1) {
        fmt::print("Stage: Symbol Collection\n");
      }

      argc::SymbolCollector symbol_collector(error_reporter);
      symbol_collector.visitModuleDeclaration(parse_tree);

      // Get the populated symbol table
      auto &symbol_table = symbol_collector.getSymbolTable();

      if (config.getVerbosity() >= 2) {
        fmt::print("=== Symbol Table Contents ===\n");
        symbol_table.dump_current_scope(std::cout);
        fmt::print("=============================\n");
      }

      if (!error_reporter.shouldContinue()) {
        fmt::print(stderr, fg(fmt::color::red),
                   "Compilation aborted due to {} error(s) and {} warning(s)\n",
                   error_reporter.errorCount() - error_reporter.warningCount(),
                   error_reporter.warningCount());
        return 1;
      }

      // === FUTURE STAGES ===
      // Shall continue with:
      // - Semantic Analysis
      // - Type Checking
      // - Code Generation
      // - Optimisation (if enabled)

      if (config.getVerbosity() >= 1) {
        fmt::print("Symbol collection completed successfully for {}\n", input_file_path);
      }
    } catch (const std::runtime_error &e) {
      fmt::print(stderr, fg(fmt::color::crimson), "Fatal Error: ");
      fmt::print(stderr, "{}\n", e.what());
      return 1;
    } catch (const std::exception &e) {
      fmt::print(stderr, fg(fmt::color::crimson), "Unexpected Error: ");
      fmt::print(stderr, "{}\n", e.what());
      return 1;
    }
  }

  if (error_reporter.errorCount() > 0) {
    fmt::print("Compilation completed with {} error(s) and {} warning(s)\n",
              error_reporter.errorCount() - error_reporter.warningCount(),
              error_reporter.warningCount());
    return error_reporter.fatalCount() > 0 ? 1 : 0;
  } else {
    fmt::print(fg(fmt::color::green), "Compilation successful!\n");
    if (config.getVerbosity() >= 1) {
      fmt::print("Output file: {}\n", config.getOutputFile());
      fmt::print("Target architecture: {}\n", config.getTargetArchString());
      fmt::print("Optimisation level: {}\n", static_cast<int>(config.getOptimisationLevel()));
    }
  }

  return 0;
}
