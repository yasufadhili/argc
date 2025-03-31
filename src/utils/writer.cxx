#include "writer.hxx"
#include <iostream>

namespace utils::writer {

// Example implementation of additional functionality or specialized writers

/**
 * @brief Example function to write content to a file with logging
 * @param filepath Path to the file
 * @param content Content to write
 * @param append Whether to append to the file (true) or overwrite it (false)
 * @return true if operation was successful, false otherwise
 */
bool write_with_logging(const std::string& filepath, const std::string& content, bool append = false) {
    try {
        std::cout << "Writing to file: " << filepath << (append ? " (append mode)" : " (overwrite mode)") << std::endl;
        
        auto dir = std::filesystem::path(filepath).parent_path();
        if (!dir.empty() && !directory_exists(dir.string())) {
            std::cout << "Creating directory: " << dir.string() << std::endl;
            if (!create_directory(dir.string())) {
                std::cerr << "Failed to create directory: " << dir.string() << std::endl;
                return false;
            }
        }
        
        FileWriter writer(filepath, append);
        std::cout << "File opened successfully: " << writer.get_filepath() << std::endl;
        
        if (!writer.write(content)) {
            std::cerr << "Failed to write content to file" << std::endl;
            return false;
        }
        
        if (!writer.flush()) {
            std::cerr << "Failed to flush file buffer" << std::endl;
            return false;
        }
        
        std::cout << "Successfully wrote " << content.size() << " bytes to file" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error writing to file: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Example function to write a log entry to a log file
 * @param logfile Path to the log file
 * @param message Log message to write
 * @return true if operation was successful, false otherwise
 */
bool write_log_entry(const std::string& logfile, const std::string& message) {
    try {
        // Get current time
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::string timestamp = std::ctime(&time);
        
        if (!timestamp.empty() && timestamp.back() == '\n') {
            timestamp.pop_back();
        }
        
        std::string logEntry = "[" + timestamp + "] " + message;
        
        return append_to_file(logfile, logEntry + "\n");
    } catch (const std::exception& e) {
        std::cerr << "Error writing log entry: " << e.what() << std::endl;
        return false;
    }
}

}
