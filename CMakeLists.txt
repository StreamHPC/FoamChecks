project(ClangDelta LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.18)

find_package(LLVM  REQUIRED)
find_package(Clang REQUIRED)

set(CMAKE_CXX_EXTENSIONS OFF)

add_library(common_options INTERFACE)
target_compile_features(common_options INTERFACE cxx_std_17)

add_subdirectory(src)
