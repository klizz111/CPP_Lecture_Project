# CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(Complex VERSION 1.0)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "g++")

# 添加源文件
file(GLOB_RECURSE SOURCES "main.cpp" "complexobject/complexObject.cpp"  "complexobject/math/mul.cpp" "ast/ast.cpp"  )
list(APPEND SOURCES "parser.cpp")
add_executable(Complex main.cpp ${SOURCES})
#设定输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
# 包含目录
#include_directories(${PROJECT_SOURCE_DIR}/include)

