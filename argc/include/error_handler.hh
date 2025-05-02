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
  private:
    std::string message_;
    Severity severity_;
    yy::location location_;
    std::optional<std::string> code_snippet_;
    std::optional<std::string> suggestion_;

  public:
    DiagnosticMessage(std::string message, Severity severity,
                      const yy::location &location,
                      std::optional<std::string> code_snippet = std::nullopt,
                      std::optional<std::string> suggestion = std::nullopt)
      : message_(std::move(message)),
        severity_(severity),
        location_(location),
        code_snippet_(std::move(code_snippet)),
        suggestion_(std::move(suggestion)) {
    }

    const std::string &message() const { return message_; }
    Severity severity() const { return severity_; }
    const yy::location &location() const { return location_; }
    const std::optional<std::string> &code_snippet() const { return code_snippet_; }
    const std::optional<std::string> &suggestion() const { return suggestion_; }

    std::string formatted_message() const {
      std::stringstream ss;

      switch (severity_) {
        case Severity::INFO: ss << "INFO: ";
          break;
        case Severity::WARNING: ss << "WARNING: ";
          break;
        case Severity::ERROR: ss << "ERROR: ";
          break;
        case Severity::FATAL: ss << "FATAL ERROR: ";
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

      return ss.str();
    }
  };

  // The main error handler class - singleton pattern
  class DiagnosticHandler {
  private:
    std::vector<DiagnosticMessage> messages_;
    bool has_error_ = false;
    bool has_fatal_error_ = false;

    // Private constructor for singleton
    DiagnosticHandler() = default;

  public:
    // Delete copy constructor and assignment operator
    DiagnosticHandler(const DiagnosticHandler &) = delete;

    DiagnosticHandler &operator=(const DiagnosticHandler &) = delete;

    // Singleton instance getter
    static DiagnosticHandler &instance() {
      static DiagnosticHandler instance;
      return instance;
    }

    void report(const std::string &message, Severity severity,
                const yy::location &location,
                std::optional<std::string> code_snippet = std::nullopt,
                std::optional<std::string> suggestion = std::nullopt) {
      messages_.emplace_back(message, severity, location, code_snippet, suggestion);

      if (severity == Severity::ERROR || severity == Severity::FATAL) {
        has_error_ = true;
        if (severity == Severity::FATAL) {
          has_fatal_error_ = true;
        }
      }
    }

    void info(const std::string &message, const yy::location &location,
              std::optional<std::string> code_snippet = std::nullopt,
              std::optional<std::string> suggestion = std::nullopt) {
      report(message, Severity::INFO, location, code_snippet, suggestion);
    }

    void warning(const std::string &message, const yy::location &location,
                 std::optional<std::string> code_snippet = std::nullopt,
                 std::optional<std::string> suggestion = std::nullopt) {
      report(message, Severity::WARNING, location, code_snippet, suggestion);
    }

    void error(const std::string &message, const yy::location &location,
               std::optional<std::string> code_snippet = std::nullopt,
               std::optional<std::string> suggestion = std::nullopt) {
      report(message, Severity::ERROR, location, code_snippet, suggestion);
    }

    void fatal(const std::string &message, const yy::location &location,
               std::optional<std::string> code_snippet = std::nullopt,
               std::optional<std::string> suggestion = std::nullopt) {
      report(message, Severity::FATAL, location, code_snippet, suggestion);
    }

    const std::vector<DiagnosticMessage> &messages() const {
      return messages_;
    }

    bool has_errors() const {
      return has_error_;
    }

    bool has_fatal_errors() const {
      return has_fatal_error_;
    }

    void print_all(std::ostream &os = std::cerr) const {
      for (const auto &msg: messages_) {
        os << msg.formatted_message() << std::endl;
      }
    }

    void print_errors(std::ostream &os = std::cerr) const {
      for (const auto &msg: messages_) {
        if (msg.severity() == Severity::ERROR || msg.severity() == Severity::FATAL) {
          os << msg.formatted_message() << std::endl;
        }
      }
    }

    void clear() {
      messages_.clear();
      has_error_ = false;
      has_fatal_error_ = false;
    }

    size_t message_count() const {
      return messages_.size();
    }

    size_t error_count() const {
      size_t count = 0;
      for (const auto &msg: messages_) {
        if (msg.severity() == Severity::ERROR || msg.severity() == Severity::FATAL) {
          count++;
        }
      }
      return count;
    }

    size_t warning_count() const {
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
