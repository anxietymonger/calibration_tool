#include "calibration.hpp"


bool Calibration::add_image(cv::Mat& image) {
    if (image_size_.height == 0 && image_size_.width == 0) {
        image_size_ = image.size();
    } else if (image_size_.height != image.size().height || image_size_.width != image.size().width) {
        std::cout << "image resolution does not match." << std::endl;
        return false;
    }

    std::vector<cv::Point2f> corners;
    cv::findChessboardCorners(image, chess_size_, corners);
    if (!corners.size()) {
        return false;
    }
    cv::drawChessboardCorners(image, chess_size_, corners, true);
    
    std::vector<cv::Point3f> w_points;
    for (auto i = 0; i < chess_size_.height; ++i) {
        for (auto j = 0; j < chess_size_.width; ++j) {
            const auto x = (float)j * grid_size_;
            const auto y = (float)i * grid_size_;
            cv::Point3f pt{ x, y, 0.0f };
            w_points.push_back(pt);
        }
    }
    if (w_points.size() != chess_size_.height * chess_size_.width || corners.size() != chess_size_.height * chess_size_.width) {
        return false;
    }
    object_points_.push_back(w_points);
    image_points_.push_back(corners);
    return true;
}


float Calibration::do_calibration() {
    std::vector<cv::Mat> rvecs, tvecs;
    // Execute calibration
    auto repr = calibrateCamera(object_points_, image_points_, image_size_, K_, D_, rvecs, tvecs);
    // Output result
    std::cout << "fx: " << K_(0, 0) << std::endl;
    std::cout << "fy: " << K_(1, 1) << std::endl;
    std::cout << "cx: " << K_(0, 2) << std::endl;
    std::cout << "cy: " << K_(1, 2) << std::endl;
    std::cout << "k1: " << D_(0, 0) << std::endl;
    std::cout << "k2: " << D_(0, 1) << std::endl;
    std::cout << "p1: " << D_(0, 2) << std::endl;
    std::cout << "p2: " << D_(0, 3) << std::endl;
    std::cout << "k3: " << D_(0, 4) << std::endl;
    std::cout << "reprojection error: " << repr << std::endl;
    return repr;
}
