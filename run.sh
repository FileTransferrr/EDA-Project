#!/bin/bash

if [ -z "$1" ]; then
    Case="./testcase/gate_2000_2000_100.v"
else
    Case=$1
fi

export Case

./work/bin/logicLoopChecker $Case

base_name=$(basename $Case .v)

python3 ./work/src/checker.py result_1.txt ./Soln/${base_name}/result_1.txt
python3 ./work/src/checker.py result_2.txt ./Soln/${base_name}/result_2.txt
python3 ./work/src/checker.py result_3.txt ./Soln/${base_name}/result_3.txt
python3 ./work/src/checker_R4.py ./Soln/${base_name}/result_4.txt result_4.txt
