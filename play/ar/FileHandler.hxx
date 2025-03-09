#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>


class FileHandler {
  std::string filepath_;
  std::ifstream file_stream_;

public:
  explicit FileHandler(std::string filepath) : filepath_(std::move(filepath)),
                                                            file_stream_() {
  }

  ~FileHandler() {
    close();
  }

  bool open() {
    file_stream_.open(filepath_, std::ios::binary);
    if (file_stream_.is_open()) {
      return true;
    }
    return false;
  }

  void close() {
    if (file_stream_.is_open()) {
      file_stream_.close();
    }
  }

  std::string read_to_string() {
    if (!file_stream_.is_open()) {
      if (!open()) {
        throw std::runtime_error("Error: File is not open and could not be opened: " + filepath_);
      }
    }

    std::stringstream buffer;
    buffer << file_stream_.rdbuf();
    return buffer.str();
  }

  std::string read_line() {
    if (!file_stream_.is_open()) {
      if (!open()) {
        throw std::runtime_error("Error: File is not open and could not be opened: " + filepath_);
      }
    }
    if (std::string line; std::getline(file_stream_, line)) {
      return line;
    }
    return ""; // Return empty string if no more lines
  }

  bool is_open() const {
    return file_stream_.is_open();
  }

  const std::string &get_file_path() const {
    return filepath_;
  }
};

// Example Usage:
/*
int main() {
    try {
        SourceFileHandler fileHandler("example.txt"); // Create a file named example.txt and put some text into it.

        if (fileHandler.open()) {
            std::string content = fileHandler.readToString();
            std::cout << "File Content:\n" << content << std::endl;

            fileHandler.close();

            SourceFileHandler fileHandler2("example.txt");
            if(fileHandler2.open()){
                std::cout << "\nReading line by line:\n";
                std::string line;
                while((line = fileHandler2.readLine()) != ""){
                    std::cout << line << std::endl;
                }
            } else {
                std::cout << "couldnt reopen file" << std::endl;
            }
        } else {
            std::cerr << "Error: Could not open file: " << fileHandler.getFilePath() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
*/