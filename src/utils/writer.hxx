#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <stdexcept>

namespace utils::writer {

/**
 * Utility class for writing to files with proper resource management
 */
class FileWriter {
private:
    std::string filepath;
    std::ofstream file;
    bool is_file_open = false;
    bool append_mode = false;

public:
    FileWriter() = default;

    /**
     * Creates a file writer and opens the specified file
     * 
     * @param path Path to the file to open
     * @param append Whether to append to the file (true) or overwrite it (false)
     * @throws std::runtime_error if file cannot be opened
     */
    explicit FileWriter(const std::string& path, bool append = false) : filepath(path), append_mode(append) {
        open(path, append);
    }

    ~FileWriter() {
        close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept {
        *this = std::move(other);
    }

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (this != &other) {
            close();
            filepath = std::move(other.filepath);
            file = std::move(other.file);
            is_file_open = other.is_file_open;
            append_mode = other.append_mode;
            other.is_file_open = false;
        }
        return *this;
    }

    /**
     * Opens a file for writing
     * 
     * @param path Path to the file
     * @param append Whether to append to the file (true) or overwrite it (false)
     * @return true if file was opened successfully
     * @throws std::runtime_error if file cannot be opened
     */
    bool open(const std::string& path, bool append = false) {
        close();
        filepath = path;
        append_mode = append;
        
        auto dir = std::filesystem::path(path).parent_path();
        if (!dir.empty() && !std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }
        
        std::ios_base::openmode mode = std::ios::out;
        if (append) {
            mode |= std::ios::app;
        } else {
            mode |= std::ios::trunc;
        }
        
        file.open(path, mode);
        
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + path);
        }
        
        is_file_open = true;
        return true;
    }

    void close() {
        if (is_file_open) {
            file.close();
            is_file_open = false;
        }
    }

    /**
     * Checks if the file is open
     * 
     * @return true if file is open, false otherwise
     */
    [[nodiscard]] bool is_open() const {
        return is_file_open;
    }

    /**
     * Gets the current filepath
     * 
     * @return The current filepath
     */
    [[nodiscard]] std::string get_filepath() const {
        return filepath;
    }

    /**
     * Checks if the writer is in append mode
     * 
     * @return true if in append mode, false otherwise
     */
    [[nodiscard]] bool is_append_mode() const {
        return append_mode;
    }

    /**
     * Writes a string to the file
     * 
     * @param content The string to write
     * @return true if write was successful, false otherwise
     * @throws std::runtime_error if file is not open
     */
    bool write(const std::string& content) {
        if (!is_file_open) {
            throw std::runtime_error("Cannot write to closed file");
        }
        
        file << content;
        return !file.fail();
    }

    /**
     * Writes a string to the file followed by a newline
     * 
     * @param content The string to write
     * @return true if write was successful, false otherwise
     * @throws std::runtime_error if file is not open
     */
    bool write_line(const std::string& content) {
        if (!is_file_open) {
            throw std::runtime_error("Cannot write to closed file");
        }
        
        file << content << '\n';
        return !file.fail();
    }

    /**
     * Writes multiple lines to the file
     * 
     * @param lines Vector of strings to write as lines
     * @return true if all writes were successful, false otherwise
     * @throws std::runtime_error if file is not open
     */
    bool write_lines(const std::vector<std::string>& lines) {
        if (!is_file_open) {
            throw std::runtime_error("Cannot write to closed file");
        }
        
        for (const auto& line : lines) {
            file << line << '\n';
            if (file.fail()) {
                return false;
            }
        }
        
        return true;
    }

    /**
     * Flushes the file buffer to ensure all data is written
     * 
     * @return true if flush was successful, false otherwise
     * @throws std::runtime_error if file is not open
     */
    bool flush() {
        if (!is_file_open) {
            throw std::runtime_error("Cannot flush closed file");
        }
        
        file.flush();
        return !file.fail();
    }
};

/**
 * Checks if a directory exists
 * 
 * @param dirpath Path to the directory
 * @return true if directory exists, false otherwise
 */
[[nodiscard]] inline bool directory_exists(const std::string& dirpath) {
    return std::filesystem::exists(dirpath) && std::filesystem::is_directory(dirpath);
}

/**
 * Creates a directory if it doesn't exist
 * 
 * @param dirpath Path to the directory
 * @return true if directory exists or was created successfully, false otherwise
 */
inline bool create_directory(const std::string& dirpath) {
    if (directory_exists(dirpath)) {
        return true;
    }
    
    return std::filesystem::create_directories(dirpath);
}

/**
 * Writes content to a file
 * 
 * @param filepath Path to the file
 * @param content Content to write
 * @param append Whether to append to the file (true) or overwrite it (false)
 * @return true if write was successful, false otherwise
 * @throws std::runtime_error if file cannot be opened
 */
inline bool write_file(const std::string& filepath, const std::string& content, bool append = false) {
    FileWriter writer(filepath, append);
    return writer.write(content) && writer.flush();
}

/**
 * Writes lines to a file
 * 
 * @param filepath Path to the file
 * @param lines Lines to write
 * @param append Whether to append to the file (true) or overwrite it (false)
 * @return true if write was successful, false otherwise
 * @throws std::runtime_error if file cannot be opened
 */
inline bool write_lines(const std::string& filepath, const std::vector<std::string>& lines, bool append = false) {
    FileWriter writer(filepath, append);
    return writer.write_lines(lines) && writer.flush();
}

/**
 * Appends content to a file
 * 
 * @param filepath Path to the file
 * @param content Content to append
 * @return true if append was successful, false otherwise
 * @throws std::runtime_error if file cannot be opened
 */
inline bool append_to_file(const std::string& filepath, const std::string& content) {
    return write_file(filepath, content, true);
}

}
