#!/bin/bash

verisim=/home/public/software/verisim/bin/verisim

source ../caseSetup.sh


CFile=../code/setUpSystemFunc_verisim.cpp
PliMap=../code/systf.tab


rm -rf verisim_history.db verisim.env verisim.db verisim_work verisim.log

${verisim} +acc+c ${Case} -P ${PliMap} ${CFile} -no-mold 

rm -rf verisim_history.zdb verisim.env verisim.zdb verisim_work verisim.log

