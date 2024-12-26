#!/bin/bash


Case=$1

export Case

CFile=./work/setUpSystemFunc_verisim.cpp
PliMap=./work/systf.tab

verisim +acc+c ${Case} -P ${PliMap} ${CFile}  -no-mold 


