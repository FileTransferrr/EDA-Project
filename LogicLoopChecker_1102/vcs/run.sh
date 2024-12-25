#!/bin/bash

source ../caseSetup.sh


CFile=../code/setUpSystemFunc.cpp

rm -rf csrc simv.daidir .simv ucli.key

#vcs -R -full64 -debug_access+all -sverilog +vpi demo/setUpSystemFunc.c -P demo/systf.tab  test.sv 

vcs -R -full64 -debug_access+all -sverilog ${Case} +vpi ${CFile} -P ../code/systf.tab

rm -rf csrc simv.daidir .simv ucli.key simv
