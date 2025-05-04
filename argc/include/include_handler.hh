#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#ifndef YY_BUF_SIZE
#define YY_BUF_SIZE 16384
#endif

#ifndef YY_CURRENT_BUFFER
#define YY_CURRENT_BUFFER (yy_current_buffer)
#endif

#include <string>
#include <stack>
#include <unordered_set>
#include <filesystem>
#include <memory>
#include "error_handler.hh"

namespace yy {

struct FileInfo {
    std::string filename;
    int line_number = 1;
    struct yy_buffer_state* buffer = nullptr;
    std::unique_ptr<std::ifstream> file;
};

class IncludeHandler {
private:
    std::stack<FileInfo> file_stack_;
    std::unordered_set<std::string> currently_processed_files_;
    bool module_declared_ = false;

public:
    IncludeHandler() = default;
    ~IncludeHandler() = default;

    // Prevent copying
    IncludeHandler(const IncludeHandler&) = delete;
    IncludeHandler& operator=(const IncludeHandler&) = delete;

    bool enter_file(const std::string& filename);
    bool exit_file();
    bool is_module_declared() const { return module_declared_; }
    void set_module_declared(bool value) { module_declared_ = value; }
    const FileInfo& current_file() const { return file_stack_.top(); }
    bool has_files() const { return !file_stack_.empty(); }
    void report_error(const std::string& msg, const yy::location& loc) const;
};

} // namespace yy 