#!/bin/bash

cd physical_channel/Debug/
make
cd ../../

cd transport_channel/Debug/
make
cd ../../

cd logical_channel/Debug/
make
cd ../../

cd test_channel_mapping/Debug/
make
cd ../../






