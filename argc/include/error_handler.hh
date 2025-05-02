#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <optional>
#include "location.hh"

namespace error {
  enum class Severity {
    INFO,
    WARNING,
    ERROR,
    FATAL
  };

  class DiagnosticMessage {
    std::string message_;
    Severity severity_;
    yy::location location_;
    std::optional<std::string> code_snippet_;
    std::optional<std::string> suggestion_;

  public:
    DiagnosticMessage(std::string message, const Severity severity,
                      const yy::location &location,
                      std::optional<std::string> code_snippet = std::nullopt,
                      std::optional<std::string> suggestion = std::nullopt)
      : message_(std::move(message)),
        severity_(severity),
        location_(location),
        code_snippet_(std::move(code_snippet)),
        suggestion_(std::move(suggestion)) {
    }

    [[nodiscard]] auto message () const -> const std::string& { return message_; }
    [[nodiscard]] auto severity () const-> Severity { return severity_; }
    [[nodiscard]] auto location () const -> const yy::location& { return location_; }
    [[nodiscard]] auto code_snippet () const -> const std::optional<std::string>&  { return code_snippet_; }
    [[nodiscard]] auto suggestion () const -> const std::optional<std::string>& { return suggestion_; }

    [[nodiscard]] auto formatted_message () const -> std::string {
      std::stringstream ss;

      switch (severity_) {
        case Severity::INFO: ss << "[INFO] ";
          break;
        case Severity::WARNING: ss << "\033[33m" << "[WARNING] " << "\033[0m";
          break;
        case Severity::ERROR: ss << "\033[31m" << "[ERROR] " << "\033[0m";
          break;
        case Severity::FATAL: ss << "\033[31m" <<"[FATAL] " << "\033[0m";
          break;
      }

      // Add location information if available
      ss << location_.begin.line << ":" << location_.begin.column << "-"
          << location_.end.line << ":" << location_.end.column << ": ";

      ss << message_;

      // Add code snippet if available
      if (code_snippet_.has_value()) {
        ss << "\n    " << *code_snippet_;
      }

      if (suggestion_.has_value()) {
        ss << "\n    Suggestion: " << *suggestion_;
      }

      ss << "\033[0m";
      return ss.str();
    }
  };

  // The main error handler class - singleton pattern
  class DiagnosticHandler {
    std::vector<DiagnosticMessage> messages_;
    bool has_error_ = false;
    bool has_fatal_error_ = false;

    // Private constructor for singleton
    DiagnosticHandler () = default;

  public:
    // Delete copy constructor and assignment operator
    DiagnosticHandler (const DiagnosticHandler &) = delete;

    DiagnosticHandler &operator = (const DiagnosticHandler &) = delete;

    // Singleton instance getter
    static auto instance () -> DiagnosticHandler & {
      static DiagnosticHandler instance;
      return instance;
    }

    auto report (const std::string &message, Severity severity,
                const yy::location &location,
                const std::optional<std::string>& code_snippet = std::nullopt,
                const std::optional<std::string>& suggestion = std::nullopt) -> void  {
      messages_.emplace_back(message, severity, location, code_snippet, suggestion);

      if (severity == Severity::ERROR || severity == Severity::FATAL) {
        has_error_ = true;
        if (severity == Severity::FATAL) {
          has_fatal_error_ = true;
        }
      }
    }

    auto info (const std::string &message, const yy::location &location,
              const std::optional<std::string>& code_snippet = std::nullopt,
              const std::optional<std::string>& suggestion = std::nullopt) -> void  {
      report(message, Severity::INFO, location, code_snippet, suggestion);
    }

    auto warning (const std::string &message, const yy::location &location,
                 const std::optional<std::string>& code_snippet = std::nullopt,
                 const std::optional<std::string>& suggestion = std::nullopt) -> void {
      report(message, Severity::WARNING, location, code_snippet, suggestion);
    }

    auto error (const std::string &message, const yy::location &location,
               const std::optional<std::string>& code_snippet = std::nullopt,
               const std::optional<std::string>& suggestion = std::nullopt) -> void {
      report(message, Severity::ERROR, location, code_snippet, suggestion);
    }

    auto fatal (const std::string &message, const yy::location &location,
               const std::optional<std::string>& code_snippet = std::nullopt,
               const std::optional<std::string>& suggestion = std::nullopt) -> void {
      report(message, Severity::FATAL, location, code_snippet, suggestion);
    }

    [[nodiscard]] auto messages () const-> const std::vector<DiagnosticMessage> & {
      return messages_;
    }

    [[nodiscard]] auto has_errors () const -> bool {
      return has_error_;
    }

    [[nodiscard]] auto has_fatal_errors () const -> bool {
      return has_fatal_error_;
    }

    auto print_all (std::ostream &os = std::cerr) const -> void {
      for (const auto &msg: messages_) {
        os << msg.formatted_message() << std::endl;
      }
    }

    auto print_errors (std::ostream &os = std::cerr) const -> void {
      for (const auto &msg: messages_) {
        if (msg.severity() == Severity::ERROR || msg.severity() == Severity::FATAL) {
          os << msg.formatted_message() << std::endl;
        }
      }
    }

    auto clear () -> void {
      messages_.clear();
      has_error_ = false;
      has_fatal_error_ = false;
    }

    [[nodiscard]] auto message_count () const -> size_t {
      return messages_.size();
    }

    [[nodiscard]] auto error_count () const -> size_t {
      size_t count = 0;
      for (const auto &msg: messages_) {
        if (msg.severity() == Severity::ERROR || msg.severity() == Severity::FATAL) {
          count++;
        }
      }
      return count;
    }

    [[nodiscard]] auto warning_count() const -> size_t {
      size_t count = 0;
      for (const auto &msg: messages_) {
        if (msg.severity() == Severity::WARNING) {
          count++;
        }
      }
      return count;
    }
  };

  // Helper macros for easier usage
#define REPORT_INFO(message, location) \
    error::DiagnosticHandler::instance().info(message, location)

#define REPORT_WARNING(message, location) \
    error::DiagnosticHandler::instance().warning(message, location)

#define REPORT_ERROR(message, location) \
    error::DiagnosticHandler::instance().error(message, location)

#define REPORT_FATAL(message, location) \
    error::DiagnosticHandler::instance().fatal(message, location)
}
