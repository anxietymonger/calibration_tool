#include "image_reader.hpp"


ImageReader::ImageReader(std::string input_dir, std::string image_list) : input_dir_(input_dir) {
    std::ifstream ifs(image_list);
    if (!ifs.is_open()) {
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
    }
    std::string line = "";
    while (ifs >> line) {
        if (!line.empty()) {
            image_list_.push_back(line);
        }
    }
    ifs.close();
}


bool ImageReader::read_image(cv::Mat& image) {
    current_++;
    if (current_ < image_list_.size()) {
        std::string filename = get_current();
        image = cv::imread(input_dir_ + "/" + filename, cv::IMREAD_COLOR);
        return true;
    } else {
        return false;
    }
}
