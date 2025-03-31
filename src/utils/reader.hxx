#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <stdexcept>

namespace utils::reader {

/**
 * Provides file reading capabilities with proper resource management
 */
class FileReader {
private:
    std::string filepath;
    std::ifstream file;
    bool is_file_open = false;

public:
    FileReader() = default;

    /**
     * Creates a file reader and opens the specified file
     * 
     * @param path Path to the file to open
     * @throws std::runtime_error if file cannot be opened
     */
    explicit FileReader(const std::string& path) : filepath(path) {
        open(path);
    }

    ~FileReader() {
        close();
    }

    // Disable copy operations
    FileReader(const FileReader&) = delete;
    FileReader& operator=(const FileReader&) = delete;

    /**
     * Move constructor
     */
    FileReader(FileReader&& other) noexcept {
        *this = std::move(other);
    }

    /**
     * Move assignment operator
     */
    FileReader& operator=(FileReader&& other) noexcept {
        if (this != &other) {
            close();
            filepath = std::move(other.filepath);
            file = std::move(other.file);
            is_file_open = other.is_file_open;
            other.is_file_open = false;
        }
        return *this;
    }

    /**
     * Opens a file for reading
     * 
     * @param path Path to the file to open
     * @return true if file was opened successfully
     * @throws std::runtime_error if file cannot be opened
     */
    auto open(const std::string& path) -> bool {
        close();
        filepath = path;
        file.open(path, std::ios::in);
        
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + path);
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

    [[nodiscard]] bool is_open() const {
        return is_file_open;
    }
    
    [[nodiscard]] std::string get_filepath() const {
        return filepath;
    }

    /**
     * Reads the entire file into a string
     * 
     * @return The file contents as a string
     * @throws std::runtime_error if file is not open
     */
    [[nodiscard]] std::string read_all() {
        if (!is_file_open) {
            throw std::runtime_error("Cannot read from closed file");
        }
        
        auto current_pos = file.tellg();
        file.seekg(0, std::ios::beg);
        
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        
        file.seekg(current_pos);
        return content;
    }

    /**
     * Reads the file line by line
     * 
     * @return Vector of strings, each representing a line in the file
     * @throws std::runtime_error if file is not open
     */
    [[nodiscard]] std::vector<std::string> read_lines() {
        if (!is_file_open) {
            throw std::runtime_error("Cannot read from closed file");
        }

        auto current_pos = file.tellg();
        file.seekg(0, std::ios::beg);
        
        std::vector<std::string> lines;
        std::string line;
        
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        
        file.clear(); // Clear EOF flag
        file.seekg(current_pos);
        
        return lines;
    }

    /**
     * Reads a specific line from the file
     * 
     * @param line_number The line number to read (0-based)
     * @return The line content if found, std::nullopt otherwise
     * @throws std::runtime_error if file is not open
     */
    [[nodiscard]] std::optional<std::string> read_line(size_t line_number) {
        if (!is_file_open) {
            throw std::runtime_error("Cannot read from closed file");
        }

        auto current_pos = file.tellg();
        file.seekg(0, std::ios::beg);
        
        std::string line;
        size_t current_line = 0;
        
        while (current_line < line_number && std::getline(file, line)) {
            current_line++;
        }
        
        std::optional<std::string> result;
        if (current_line == line_number && std::getline(file, line)) {
            result = line;
        }
        
        file.clear(); // Clear EOF flag
        file.seekg(current_pos);
        
        return result;
    }

    /**
     * Reads a chunk of the file from a specific position
     * 
     * @param start Starting position in the file
     * @param length Number of characters to read
     * @return The read chunk as a string
     * @throws std::runtime_error if file is not open or if read fails
     */
    [[nodiscard]] std::string read_chunk(std::streampos start, size_t length) {
        if (!is_file_open) {
            throw std::runtime_error("Cannot read from closed file");
        }

        auto current_pos = file.tellg();
        file.seekg(start);
        
        if (file.fail()) {
            file.seekg(current_pos);
            throw std::runtime_error("Failed to seek to position " + std::to_string(start));
        }
        
        std::string chunk;
        chunk.resize(length);
        
        file.read(&chunk[0], static_cast<std::streamsize>(length));
        chunk.resize(static_cast<size_t>(file.gcount())); // Adjust size if read less than requested
        
        file.seekg(current_pos);
        return chunk;
    }
};

/**
 * Checks if a file exists
 * 
 * @param filepath Path to the file
 * @return true if file exists, false otherwise
 */
[[nodiscard]] inline bool file_exists(const std::string& filepath) {
    return std::filesystem::exists(filepath);
}

/**
 * Gets the size of a file in bytes
 * 
 * @param filepath Path to the file
 * @return Size of the file in bytes
 * @throws std::runtime_error if file does not exist
 */
[[nodiscard]] inline size_t file_size(const std::string& filepath) {
    if (!file_exists(filepath)) {
        throw std::runtime_error("File does not exist: " + filepath);
    }
    return static_cast<size_t>(std::filesystem::file_size(filepath));
}

/**
 * Reads an entire file into a string
 * 
 * @param filepath Path to the file
 * @return The file contents as a string
 * @throws std::runtime_error if file cannot be opened
 */
[[nodiscard]] inline std::string read_file(const std::string& filepath) {
    FileReader reader(filepath);
    return reader.read_all();
}

/**
 * Reads a file line by line
 * 
 * @param filepath Path to the file
 * @return Vector of strings, each representing a line in the file
 * @throws std::runtime_error if file cannot be opened
 */
[[nodiscard]] inline std::vector<std::string> read_lines(const std::string& filepath) {
    FileReader reader(filepath);
    return reader.read_lines();
}

} 
