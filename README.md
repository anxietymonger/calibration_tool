1. put calibration images into `input/`
1. edit `conf.json`
1. build
    ```
    mkdir build
    cd build
    cmake ..
    make
    cd ..
    ```
1. run
    ```
    ./run.sh
    ```
1. `camera.json` will be in `output/`, together with calibrated images
