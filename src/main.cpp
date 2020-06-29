#include <iostream>
#include <vector>
#include <fstream>
#include <streambuf>

#include <json11.hpp>
#include <gflags/gflags.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "calibration.hpp"
#include "image_reader.hpp"


DEFINE_string(conf_json, "", "[required] path to JSON configuration file.");
DEFINE_string(input_dir, "", "[required] directory of input images.");
DEFINE_string(output_dir, "", "[required] directory of output files.");
DEFINE_string(image_list, "", "[required] a .txt file which includes filenames of all input images (each image a line). ");


int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    if (FLAGS_input_dir.empty() || FLAGS_conf_json.empty() || FLAGS_output_dir.empty() || FLAGS_image_list.empty()) {
        std::cout << "invalid arguments, use --help for help." << std::endl;
        return -1;
    }

    //read parameters from json
    std::ifstream ifs(FLAGS_conf_json);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();

    std::string err = "";
    auto conf = json11::Json::parse(buffer.str(), err);
    cv::Size chess_size = {conf["chess_size"]["num_row"].int_value(), conf["chess_size"]["num_col"].int_value()};
    float grid_size = conf["grid_size"].number_value();

    //calibration
    ImageReader image_reader(FLAGS_input_dir, FLAGS_image_list);
    Calibration calibration(chess_size, grid_size);

    cv::Mat image = {};
    while (image_reader.read_image(image)) {
        if (!calibration.add_image(image)) {
            std::cout << image_reader.get_current() << " failed." << std::endl;
        }
        cv::imwrite(FLAGS_output_dir + "/" + image_reader.get_current(), image);
    }

    calibration.do_calibration();
    
    auto K = calibration.get_K();
    auto D = calibration.get_D();
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
    std::ofstream ofs(FLAGS_output_dir + "/camera.json");
    ofs << json11::Json(camera_json).dump() << std::endl;
    ofs.close();
    return 0;
}
