#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <sstream>
#include <functional>
#include <optional>
#include <fstream>
#include <filesystem>


namespace file_utils {


  class FileException : public std::runtime_error {
  public:
    explicit FileException(const std::string& message) : std::runtime_error(message) {}
  };

  class FileNotFoundException final : public FileException {
  public:
    explicit FileNotFoundException(const std::string& filename)
        : FileException("File not found: " + filename) {}
  };

  class FileAccessException final : public FileException {
  public:
    explicit FileAccessException(const std::string& filename)
        : FileException("Cannot access file: " + filename) {}
  };


  std::string readFileToString(const std::string& filename);

  std::vector<std::string> readFileLines(const std::string& filename);

  std::optional<std::string> readFirstNonEmptyLine(const std::string& filename);

  // Read specific line from a file (0-based index)
  std::optional<std::string> readLine(const std::string& filename, size_t lineNumber);

  size_t countLines(const std::string& filename);

  void processFileLineByLine(const std::string& filename,
                            const std::function<void(const std::string&, size_t)>& processor);

  bool writeStringToFile(const std::string& filename, const std::string& content, bool append = false);

  // Write vector of strings to file (one line per string)
  bool writeLinesToFile(const std::string& filename, const std::vector<std::string>& lines, bool append = false);

  bool fileExists(const std::string& filename);

  // Get file size in bytes
  std::optional<uintmax_t> getFileSize(const std::string& filename);

  bool copyFile(const std::string& source, const std::string& destination);

  bool moveFile(const std::string& source, const std::string& destination);

  bool deleteFile(const std::string& filename);

  bool createDirectory(const std::string& dirPath);

  bool isDirectory(const std::string& path);

  std::vector<std::string> listDirectory(const std::string& dirPath);

  std::string getFileExtension(const std::string& filename);

  // Get file name without path
  std::string getFileName(const std::string& filepath);

  std::string getDirectoryPath(const std::string& filepath);

  // Read binary file to vector of bytes
  std::vector<uint8_t> readBinaryFile(const std::string& filename);

  // Write binary data to file
  bool writeBinaryFile(const std::string& filename, const std::vector<uint8_t>& data);


  // Stream file reader class for memory-efficient reading of large files
  class FileReader {

    std::ifstream file;
    std::string currentLine;
    size_t lineNumber;
    std::string filename;

  public:
    explicit FileReader(const std::string& filename);
    ~FileReader();

    bool isOpen() const;
    bool hasNextLine();
    std::string nextLine();
    size_t getCurrentLineNumber() const;
    void reset();
    void close();
  };



  inline std::string readFileToString(const std::string& filename) {
        if (!fileExists(filename)) {
            throw FileNotFoundException(filename);
        }

        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw FileAccessException(filename);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    inline std::vector<std::string> readFileLines(const std::string& filename) {
        if (!fileExists(filename)) {
            throw FileNotFoundException(filename);
        }

        std::ifstream file(filename, std::ios::in);
        if (!file.is_open()) {
            throw FileAccessException(filename);
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        return lines;
    }

    inline std::optional<std::string> readFirstNonEmptyLine(const std::string& filename) {
        if (!fileExists(filename)) {
            throw FileNotFoundException(filename);
        }

        std::ifstream file(filename, std::ios::in);
        if (!file.is_open()) {
            throw FileAccessException(filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            // Trim whitespace from the line
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (!line.empty()) {
                return line;
            }
        }
        return std::nullopt;
    }

    inline std::optional<std::string> readLine(const std::string& filename, size_t lineNumber) {
        if (!fileExists(filename)) {
            throw FileNotFoundException(filename);
        }

        std::ifstream file(filename, std::ios::in);
        if (!file.is_open()) {
            throw FileAccessException(filename);
        }

        std::string line;
        size_t currentLine = 0;

        while (std::getline(file, line)) {
            if (currentLine == lineNumber) {
                return line;
            }
            currentLine++;
        }

        return std::nullopt;
    }

    inline size_t countLines(const std::string& filename) {
        if (!fileExists(filename)) {
            throw FileNotFoundException(filename);
        }

        std::ifstream file(filename, std::ios::in);
        if (!file.is_open()) {
            throw FileAccessException(filename);
        }

        size_t lineCount = 0;
        std::string line;
        while (std::getline(file, line)) {
            lineCount++;
        }
        return lineCount;
    }

    inline void processFileLineByLine(const std::string& filename,
                               const std::function<void(const std::string&, size_t)>& processor) {
        if (!fileExists(filename)) {
            throw FileNotFoundException(filename);
        }

        std::ifstream file(filename, std::ios::in);
        if (!file.is_open()) {
            throw FileAccessException(filename);
        }

        std::string line;
        size_t lineNumber = 0;

        while (std::getline(file, line)) {
            processor(line, lineNumber);
            lineNumber++;
        }
    }

    inline bool writeStringToFile(const std::string& filename, const std::string& content, bool append) {
        std::ios_base::openmode mode = std::ios::out;
        if (append) {
            mode |= std::ios::app;
        }

        std::ofstream file(filename, mode);
        if (!file.is_open()) {
            return false;
        }

        file << content;
        return file.good();
    }

    inline bool writeLinesToFile(const std::string& filename, const std::vector<std::string>& lines, bool append) {
        std::ios_base::openmode mode = std::ios::out;
        if (append) {
            mode |= std::ios::app;
        }

        std::ofstream file(filename, mode);
        if (!file.is_open()) {
            return false;
        }

        for (const auto& line : lines) {
            file << line << '\n';
        }
        return file.good();
    }

    inline bool fileExists(const std::string& filename) {
        return std::filesystem::exists(filename);
    }

    inline std::optional<uintmax_t> getFileSize(const std::string& filename) {
        if (!fileExists(filename)) {
            return std::nullopt;
        }

        std::error_code ec;
        auto size = std::filesystem::file_size(filename, ec);
        if (ec) {
            return std::nullopt;
        }
        return size;
    }

    inline bool copyFile(const std::string& source, const std::string& destination) {
        if (!fileExists(source)) {
            return false;
        }

        std::error_code ec;
        std::filesystem::copy_file(source, destination,
                                  std::filesystem::copy_options::overwrite_existing, ec);
        return !ec;
    }

    inline bool moveFile(const std::string& source, const std::string& destination) {
        if (!fileExists(source)) {
            return false;
        }

        std::error_code ec;
        std::filesystem::rename(source, destination, ec);
        return !ec;
    }

    inline bool deleteFile(const std::string& filename) {
        if (!fileExists(filename)) {
            return false;
        }

        std::error_code ec;
        return std::filesystem::remove(filename, ec);
    }

    inline bool createDirectory(const std::string& dirPath) {
        std::error_code ec;
        return std::filesystem::create_directories(dirPath, ec);
    }

    inline bool isDirectory(const std::string& path) {
        return std::filesystem::is_directory(path);
    }

    inline std::vector<std::string> listDirectory(const std::string& dirPath) {
        if (!isDirectory(dirPath)) {
            throw FileException(dirPath + " is not a directory");
        }

        std::vector<std::string> files;
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            files.push_back(entry.path().string());
        }
        return files;
    }

    inline std::string getFileExtension(const std::string& filename) {
        size_t dotPos = filename.find_last_of('.');
        if (dotPos != std::string::npos) {
            return filename.substr(dotPos);
        }
        return "";
    }

    inline std::string getFileName(const std::string& filepath) {
        return std::filesystem::path(filepath).filename().string();
    }

    inline std::string getDirectoryPath(const std::string& filepath) {
        return std::filesystem::path(filepath).parent_path().string();
    }

    inline std::vector<uint8_t> readBinaryFile(const std::string& filename) {
        if (!fileExists(filename)) {
            throw FileNotFoundException(filename);
        }

        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw FileAccessException(filename);
        }

        // Get file size
        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read file content into vector
        std::vector<uint8_t> buffer(size);
        if (size > 0) {
            file.read(reinterpret_cast<char*>(buffer.data()), size);
        }

        return buffer;
    }

    inline bool writeBinaryFile(const std::string& filename, const std::vector<uint8_t>& data) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            return false;
        }

        if (!data.empty()) {
            file.write(reinterpret_cast<const char*>(data.data()), data.size());
        }

        return file.good();
    }


    inline FileReader::FileReader(const std::string& filename) : filename(filename), lineNumber(0) {
        if (!fileExists(filename)) {
            throw FileNotFoundException(filename);
        }

        file.open(filename);
        if (!file.is_open()) {
            throw FileAccessException(filename);
        }
    }

    inline FileReader::~FileReader() {
        close();
    }

    inline bool FileReader::isOpen() const {
        return file.is_open();
    }

    inline bool FileReader::hasNextLine() {
        if (!file.is_open()) {
            return false;
        }

        return file.peek() != EOF;
    }

    inline std::string FileReader::nextLine() {
        if (!file.is_open() || !hasNextLine()) {
            throw FileException("No more lines to read");
        }

        std::getline(file, currentLine);
        lineNumber++;
        return currentLine;
    }

    inline size_t FileReader::getCurrentLineNumber() const {
        return lineNumber;
    }

    inline void FileReader::reset() {
        if (file.is_open()) {
            file.clear();
            file.seekg(0);
            lineNumber = 0;
        }
    }

    inline void FileReader::close() {
        if (file.is_open()) {
            file.close();
        }
    }


}



/*
 * * USAGE
 *
*try {
std::string content {file_utils::readFileToString(input_file)};

if (const auto first_line = file_utils::readFirstNonEmptyLine(input_file)) {
std::cout << *first_line << std::endl;
}

// Process large file line by line
file_utils::processFileLineByLine(input_file, [](const std::string& line, size_t line_num) {
std::cout << line << std::endl;
});

} catch (file_utils::FileException& e) {
std::cerr << "Error: " << e.what() << std::endl;
}

// Memory Efficient reading

try {
file_utils::FileReader reader(input_file);
while (reader.hasNextLine()) {
std::string line {reader.nextLine()};
std::cout << line << std::endl;
}
} catch (const file_utils::FileException& e) {
std::cerr << "Error: " << e.what() << std::endl;
}
 *
*/



