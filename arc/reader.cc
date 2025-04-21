
#include <iostream>

#include "include/reader.hh"

namespace utils {

Reader::Reader(const std::string& filename) : file_stream_(nullptr), stream_(nullptr) {
    if (filename.empty() || filename == "-") {
        stream_ = &(std::cin);
    } else {
        file_stream_ = std::make_unique<std::ifstream>(filename);
        if (!file_stream_->is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }
        stream_ = file_stream_.get();
    }
}

auto Reader::stream() -> std::istream& {
    return *stream_;
}

auto Reader::is_open() const -> bool {
    return stream_ != nullptr && (stream_ == &(std::cin) || (file_stream_ && file_stream_->is_open()));
}

} // namespace utils
