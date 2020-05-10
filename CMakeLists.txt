cmake_minimum_required(VERSION 3.10)

project(Huffman)

set(EXECUTABLE_OUTPUT_PATH ./bin/)

add_executable(reader src/reader.cpp src/BitReader.cpp)
add_executable(writer src/writer.cpp src/BitWriter.cpp src/Heap.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
target_compile_options(reader PRIVATE -std=c++17 -Iinclude)
target_compile_options(writer PRIVATE -std=c++17 -Iinclude)

