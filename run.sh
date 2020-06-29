#!/bin/bash
set -eu

ls input > bin/list.txt
./bin/calibration --input-dir input --output-dir output --image-list bin/list.txt --conf-json conf.json
