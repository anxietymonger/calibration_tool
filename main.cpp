#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>

#include <json11.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>


std::pair<cv::Mat1d, cv::Mat1d> do_calibration(std::string data_folder, std::string save_folder, cv::Size chess_size, float grid_size) {
    std::ifstream ifs {data_folder + "/list.txt"};
    std::string line = "";
    std::vector<std::string> image_list = {};
    while (ifs >> line) {
        image_list.push_back(line);
    }
    ifs.close();
    
    // image size (width x height)
    cv::Size sz;
    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<cv::Point2f>> image_points;
    for (auto image : image_list) {
        cv::Mat img = cv::imread(data_folder + "/" + image, cv::IMREAD_COLOR);
        if (img.empty()) {
            std::cout << "image loading error: " << image << std::endl;
            continue;
        }
        sz = img.size();
        std::vector<cv::Point2f> corners;
        cv::findChessboardCorners(img, chess_size, corners);
        if (!corners.size()) {
            std::cout << image << " failed" << std::endl;
            continue;
        }
        
        cv::drawChessboardCorners(img, chess_size, corners, true);
        cv::imwrite(save_folder + "/" + image, img);
        
        std::vector<cv::Point3f> w_points;
        for (auto i = 0; i < chess_size.height; ++i) {
            for (auto j = 0; j < chess_size.width; ++j) {
                const auto x = (float)j * grid_size;
                const auto y = (float)i * grid_size;
                cv::Point3f pt{ x, y, 0.0f };
                w_points.push_back(pt);
            }
        }
        if (w_points.size() != chess_size.height * chess_size.width || corners.size() != chess_size.height * chess_size.width) {
            std::cout << image << " failed" << std::endl;
            continue;
        }
        object_points.push_back(w_points);
        image_points.push_back(corners);
    }

    cv::Mat1d K = cv::Mat1d::eye(3, 3);
    cv::Mat1d D = cv::Mat1d::zeros(1, 5);
    std::vector<cv::Mat> rvecs, tvecs;
    // Execute calibration
    auto repr = calibrateCamera(object_points, image_points, sz, K, D, rvecs, tvecs);
    // Output result
    std::ofstream ofs {save_folder + "/calibration_result.txt"};
    ofs << "fx: " << K(0, 0) << std::endl;
    ofs << "fy: " << K(1, 1) << std::endl;
    ofs << "cx: " << K(0, 2) << std::endl;
    ofs << "cy: " << K(1, 2) << std::endl;
    ofs << "k1: " << D(0, 0) << std::endl;
    ofs << "k2: " << D(0, 1) << std::endl;
    ofs << "p1: " << D(0, 2) << std::endl;
    ofs << "p2: " << D(0, 3) << std::endl;
    ofs << "k3: " << D(0, 4) << std::endl;
    ofs << std::endl;
    ofs << "reprojection error: " << repr << std::endl;
    std::cout << "reprojection error: " << repr << std::endl;
    ofs.close();

    return std::make_pair(K, D);
}

int main() {
    std::ifstream ifs {"../conf.json"};
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();

    std::string err = "";
    auto conf = json11::Json::parse(buffer.str(), err);
    cv::Size chess_size = {conf["chess_size"]["num_row"].int_value(), conf["chess_size"]["num_col"].int_value()};
    float grid_size = conf["grid_size"].number_value();

    std::pair<cv::Mat1d, cv::Mat1d> result = do_calibration("../input", "../output", chess_size, grid_size);
    auto K = result.first;
    auto D = result.second;
    auto project_matrix = json11::Json::array { 
        json11::Json::array {K(0, 0), 0, K(0, 2)},
        json11::Json::array {0, K(1, 1), K(1, 2)},
        json11::Json::array {0, 0, 1},
    };
    auto camera_extrinsic = json11::Json::array {
        json11::Json::array {0, 0, 1},
        json11::Json::array {0, 1, 0},
        json11::Json::array {0, 0, 1},
    };
    auto camera_json = json11::Json::object {
        {"ProjectMatrix" , project_matrix},
        {"CameraExtrinsic", camera_extrinsic}
    };
    std::ofstream ofs {"../output/camera.json"};
    ofs << json11::Json(camera_json).dump() << std::endl;
    ofs.close();
    return 0;
}