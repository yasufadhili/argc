#pragma once

#include <string>
#include <vector>


namespace analyser {

class SemanticError {
public:
    enum class ErrorLevel {
        WARNING,
        ERROR
    };

    struct Location {
        size_t line;
        size_t column;
        std::string filename;

        Location(size_t line, size_t column, std::string filename)
            : line(line), column(column), filename(std::move(filename)) {}
    };

private:
    std::string message_;
    Location location_;
    ErrorLevel level_;

public:
    SemanticError(std::string message, const Location& location, ErrorLevel level = ErrorLevel::ERROR)
        : message_(std::move(message)), location_(location), level_(level) {}

    [[nodiscard]] const std::string& message() const { return message_; }
    [[nodiscard]] const Location& location() const { return location_; }
    [[nodiscard]] ErrorLevel level() const { return level_; }

    [[nodiscard]] std::string to_string() const {
        std::string prefix = level_ == ErrorLevel::ERROR ? "Error" : "Warning";
        return prefix + " at " + location_.filename + ":" + 
               std::to_string(location_.line) + ":" + std::to_string(location_.column) + 
               ": " + message_;
    }
};

class ErrorReporter {
private:
    std::vector<SemanticError> errors_;
    bool has_errors_ = false;

public:
    void report(const SemanticError& error) {
        errors_.push_back(error);
        if (error.level() == SemanticError::ErrorLevel::ERROR) {
            has_errors_ = true;
        }
    }

    void report(const std::string& message, const SemanticError::Location& location, 
                SemanticError::ErrorLevel level = SemanticError::ErrorLevel::ERROR) {
        report(SemanticError(message, location, level));
    }

    [[nodiscard]] bool has_errors() const { return has_errors_; }
    [[nodiscard]] const std::vector<SemanticError>& errors() const { return errors_; }

    void print_errors() const {
        for (const auto& error : errors_) {
            fprintf(stderr, "%s\n", error.to_string().c_str());
        }
    }
};

} // namespace analyser
