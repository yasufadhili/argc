#pragma once

#include <fstream>
#include <string>
#include <memory>


namespace utils {


class Reader {
public:
    Reader(const std::string& filename);
    auto stream() -> std::istream&;
    auto is_open() const -> bool;
private:
    std::unique_ptr<std::ifstream> file_stream_;
    std::istream* stream_;
};


} // namespace utils


