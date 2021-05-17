echo "Build stage 1!"


rm -rf build
mkdir build
cd build
cmake ..
make