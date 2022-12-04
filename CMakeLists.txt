cmake_minimum_required(VERSION 3.10)
project(Compiler)

add_compile_options(-std=c++17 -O3)

file(GLOB Sources src/*.cpp)
file(GLOB Tests tests/*.sol)

add_executable(main main.cpp ${Sources})
target_include_directories(main PRIVATE "src")

file(COPY ${Tests} DESTINATION ${CMAKE_BINARY_DIR}/tests)
