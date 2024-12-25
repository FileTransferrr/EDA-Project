#!/bin/bash

if [ -z "$1" ]; then
    Case="./testcase/gate_2000_2000_100.v"
else
    Case=$1
fi

rm -rf *.vvp *.vpi obj_dir *.o ${VPI}.c

export Case

SCRIPT_DIR=$(dirname $0)

CFile=${SCRIPT_DIR}/work/setUpSystemFunc_iverilog.cpp
VPI=myvpi

touch ${VPI}.c

./iverilog/bin/iverilog -o testcase.vvp ${Case}
./iverilog/bin/iverilog-vpi -L. ${VPI}.c ${CFile}
./iverilog/bin/vvp -M. -m ${VPI} testcase.vvp


rm -rf *.vvp *.vpi obj_dir *.o ${VPI}.c
