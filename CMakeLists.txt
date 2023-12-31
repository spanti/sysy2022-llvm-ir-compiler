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
# import google test - 外部依赖库 
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# find LLVM library
find_package(LLVM REQUIRED CONFIG)
find_package(Threads REQUIRED)

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
include_directories(src/jit)
include_directories(antlr-runtime)
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS})
message(STATUS "DIR_SRC: ${DIR_SRC}")
#生成可执行文件
add_executable(c_edu_compiler ${DIR_SRC} ${DIR_LIB})
#add_executable(${PROJECT_NAME} test.cpp)
# Find the libraries that correspond to the LLVM components
# that we wish to use
# use llvm-config --components to list all name support core irreader passes orcjit
#llvm_map_components_to_libnames(llvm_libs support core irreader passes orcjit native target codegen)
set(llvm_libs LLVM)
# Link against LLVM libraries - 链接库
target_link_libraries(c_edu_compiler ${llvm_libs} Threads::Threads)
# The last two lines enable CMake’s test runner to discover the tests included in the binary, using the GoogleTest CMake module
#include(GoogleTest)
#gtest_discover_tests(c_edu_compiler)
enable_testing()
#可执行测试构建
#将测试文件链接为可执行文件
add_executable(
  hello_test
  src/google_test/hello_test.cc
  src/common/common.cpp
)
#GTest::gtest_main defines a suitable entry point
target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)

