#include "include_handler.hh"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace yy {

bool IncludeHandler::enter_file(const std::string& filename) {
    namespace fs = std::filesystem;
    
    // Convert to absolute path
    fs::path abs_path = fs::absolute(filename);
    std::string abs_filename = abs_path.string();
    
    // Check for circular includes
    if (currently_processed_files_.find(abs_filename) != currently_processed_files_.end()) {
        report_error("Circular include detected: " + filename);
        return false;
    }
    
    // Open the new file
    auto file = std::make_unique<std::ifstream>(abs_filename);
    if (!file->is_open()) {
        report_error("Could not open include file: " + filename);
        return false;
    }
    
    // Set up for new file
    FileInfo new_info;
    new_info.filename = abs_filename;
    new_info.file = std::move(file);
    new_info.line_number = 1;
    file_stack_.push(std::move(new_info));
    
    // Track this file to detect circular includes
    currently_processed_files_.insert(abs_filename);
    
    // Reset module declaration flag for new file
    module_declared_ = false;
    
    return true;
}

bool IncludeHandler::exit_file() {
    if (file_stack_.empty()) {
        return false;
    }
    
    // Get current file info
    FileInfo current = std::move(file_stack_.top());
    file_stack_.pop();
    
    // Remove from processed files set
    currently_processed_files_.erase(current.filename);
    
    if (file_stack_.empty()) {
        return false;
    }
    
    return true;
}

void IncludeHandler::report_error(const std::string& msg) const {
    std::string file_info = file_stack_.empty() ?  "unknown" : file_stack_.top().filename + ":" +  std::to_string(file_stack_.top().line_number);
    
    std::cerr << file_info << ": " << msg << std::endl;
}

} // namespace yy 