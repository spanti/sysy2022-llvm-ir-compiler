#cmake minimum version 
#haben't link antlr4 runtime for link
cmake_minimum_required(VERSION 3.20)
# project name
project (c_edu_compiler)
#include debug information
set(CMAKE_BUILD_TYPE Debug)
# compiler must be 17 with LLVM
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# find LLVM library
find_package(LLVM REQUIRED CONFIG)

message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
#store antlr4 cpps in variable DIR_SRC
file (GLOB_RECURSE DIR_SRC "src/*.cpp")
file(GLOB_RECURSE DIR_LIB "antlr-runtime/*.cpp")
#llvm libraries
include_directories(${LLVM_INCLUDE_DIRS})
#添加头文件检索路径
include_directories(src)
include_directories(src/ast)
include_directories(src/semantic)
include_directories(src/common)
include_directories(antlr-runtime)
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
message(STATUS "DIR_SRC: ${DIR_SRC}")
#生成可执行文件
add_executable(c_edu_compiler ${DIR_SRC} ${DIR_LIB})
#add_executable(${PROJECT_NAME} test.cpp)
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)
# Link against LLVM libraries - 链接库
target_link_libraries(c_edu_compiler ${llvm_libs})
