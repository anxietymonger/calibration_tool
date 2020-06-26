1. put calibration images into `input/`
1. put a `list.txt` which includes filename of all calibration images into `input/`
1. edit `conf.json`
1. build and run
    ```
    mkdir build
    cd build
    cmake ..
    make
    cd ../bin
    ./calibration
    ```
1. `calibration_result.txt` and `camera.json` will be in `output/`, together with calibrated images
