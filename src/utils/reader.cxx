#include "reader.hxx"
#include <iostream>

namespace utils::reader {

/**
 * Example function to read and print file contents
 * 
 * @param filepath Path to the file
 * @return true if operation was successful, false otherwise
 */
bool print_file_contents(const std::string& filepath) {
    try {
        if (!file_exists(filepath)) {
            std::cerr << "File does not exist: " << filepath << std::endl;
            return false;
        }

        std::cout << "File size: " << file_size(filepath) << " bytes" << std::endl;
        
        FileReader reader(filepath);
        std::cout << "Reading file: " << reader.get_filepath() << std::endl;
        
        // Read and print all lines
        auto lines = reader.read_lines();
        std::cout << "File contains " << lines.size() << " lines:" << std::endl;
        
        for (size_t i = 0; i < lines.size(); ++i) {
            std::cout << "[" << i << "] " << lines[i] << std::endl;
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error reading file: " << e.what() << std::endl;
        return false;
    }
}

} 
