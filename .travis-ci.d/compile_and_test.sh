#!/bin/bash
cd /Package
mkdir build
cd build
source ../.gitlab-ci.d/init_x86_64.sh
cmake -GNinja -DBUILD_ALL_MODULES=ON -DGeant4_DIR=$G4LIB -DROOT_DIR=$ROOTSYS -DEigen3_DIR=$Eigen3_DIR .. && \
ninja && \
ninja install && \
ctest --output-on-failure -j4
