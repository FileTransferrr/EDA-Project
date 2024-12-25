#!/bin/bash

export LD_LIBRARY_PATH=./:${LD_LIBRARY_PATH}


source ../caseSetup.sh


CFile=../code/setUpSystemFunc.cpp
PliMap=../code/systf.tab

rm -rf xcelium.d xmsc.log xmverilog.key xmverilog.history xmverilog.log


xmverilog -64 -access +rwc -sv ${Case} +vpi -plimapfile ${PliMap} ${CFile}

rm -rf xcelium.d xmsc.log xmverilog.key xmverilog.history xmverilog.log .sim*

