cmake_minimum_required(VERSION 3.10)

project(GamerServer)

set(CMAKE_CXX_FLAGS "-g -Wall ")

set(INC_DIR "/usr/include/mysql")
set(LINK_DIR "/usr/lib64/mysql")

include_directories(${INC_DIR})
link_directories(${LINK_DIR})

include_directories($ PROJECT_SOURCE_DIR/src/Common)


file(GLOB_RECURSE cpp_files ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_compile_options(-std=c++11)
add_executable(main ${cpp_files})

 target_link_libraries(main mysqlclient) 