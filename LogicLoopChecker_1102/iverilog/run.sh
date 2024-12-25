#!/bin/bash

export PATH=IVERILOG_PATH/bin:${PATH}

source ../caseSetup.sh


CFile=../code/setUpSystemFunc_iverilog.cpp
VPI=myvpi

rm -rf *.vvp *.vpi obj_dir *.o ${VPI}.c

touch ${VPI}.c

iverilog ${Case} -o testcase.vvp
iverilog-vpi -L. ${VPI}.c ${CFile}
vvp -M. -m${VPI} testcase.vvp

rm -rf *.vvp *.vpi obj_dir *.o ${VPI}.c

