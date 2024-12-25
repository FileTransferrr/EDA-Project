#!/bin/bash

source /home/public/env.bash

export PATH=/home/public/pre_submission_testcase/verisim/bin:$PATH

if [ -z "$1" ]; then
    Case="./testcase/gate_2000_2000_100.v"
else
    Case=$1
fi

rm -rf verisim_history.db verisim.env verisim.db verisim_work verisim.log modinfo.txt result_*.txt

export Case

SCRIPT_DIR=$(dirname $0)

CFile=${SCRIPT_DIR}/work/setUpSystemFunc_verisim.cpp
PliMap=${SCRIPT_DIR}/work/systf.tab

# verisim +acc+c ${Case} -P ${PliMap} ${CFile}  -no-mold -work ${Case}.work -image image
verisim +acc+c ${Case} -P ${PliMap} ${CFile}  -no-mold

rm -rf verisim_history.db verisim.env verisim.db verisim_work verisim.log modinfo.txt
