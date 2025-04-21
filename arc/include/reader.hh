#pragma once
#include <fstream>
#include <string>
#include <memory>


namespace utils {


class Reader {
public:
    Reader(const std::string& filename);
    std::istream& stream();
    bool is_open() const;
private:
    std::unique_ptr<std::ifstream> file_stream_;
    std::istream* stream_;
};


} // namespace utils


