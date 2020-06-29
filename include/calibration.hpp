#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d.hpp>


class Calibration {
private:
    cv::Size chess_size_ = {};
    cv::Size image_size_ = {};
    float grid_size_ = 0;
    std::vector<std::vector<cv::Point3f>> object_points_;
    std::vector<std::vector<cv::Point2f>> image_points_;
    cv::Mat1d K_ = cv::Mat1d::eye(3, 3);
    cv::Mat1d D_ = cv::Mat1d::zeros(1, 5);

public:
    Calibration(const cv::Size& chess_size, float grid_size) : chess_size_(chess_size), grid_size_(grid_size) {};
    bool add_image(cv::Mat& image);

    float do_calibration();

    inline cv::Mat1d get_K() {
        return K_;
    };

    inline cv::Mat1d get_D() {
        return D_;
    };
};

#endif
