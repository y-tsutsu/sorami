cmake_minimum_required(VERSION 3.10.0)
project(sorami VERSION 0.1.0 LANGUAGES C CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
endif()

find_package(CURL REQUIRED)

file(GLOB SRC_FILES src/*.cpp)

add_executable(sorami ${SRC_FILES})

target_compile_features(sorami PRIVATE cxx_std_20)
set_property(TARGET sorami PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET sorami PROPERTY CXX_EXTENSIONS OFF)

target_include_directories(sorami PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/third_party)
target_link_libraries(sorami CURL::libcurl)

file(COPY config DESTINATION ${CMAKE_BINARY_DIR})

if(MSVC)
    target_compile_options(sorami PRIVATE /W4 /permissive-)
else()
    target_compile_options(sorami PRIVATE -Wall)
endif()

include(CTest)
enable_testing()
