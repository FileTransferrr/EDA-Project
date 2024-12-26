#!/bin/bash

mkdir -p verisim_work
rm -rf verisim_work/image.so
ln -s $IMAGE_DIR/$1.work/image.so verisim_work/image.so
rm -rf result*.txt