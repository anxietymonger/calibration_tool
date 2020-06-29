#ifndef IMAGE_READER_H
#define IMAGE_READER_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <opencv2/core/core.hpp>


class ImageReader {
private:
    std::vector<std::string> image_list_ = {};
    std::string input_dir_ = "";
    int current_ = -1;

public:
    ImageReader(std::string input_dir, std::string image_list);

    bool read_image(cv::Mat& image);

    inline std::string get_current() {
        return image_list_[current_];
    }
};

#endif
