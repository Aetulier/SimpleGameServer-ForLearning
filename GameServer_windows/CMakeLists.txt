cmake_minimum_required(VERSION 3.10)

project(GamerServer)

set(CMAKE_CXX_FLAGS "-g -Wall -lwsock32")

set(INC_DIR "D:/Program Files/mysql/mysql-8.0.18-winx64/include")
set(LINK_DIR "D:/Program Files/mysql/mysql-8.0.18-winx64/lib")

include_directories(${INC_DIR})
link_directories(${LINK_DIR})

include_directories($ PROJECT_SOURCE_DIR/src/Common)


file(GLOB_RECURSE cpp_files ${PROJECT_SOURCE_DIR}/src/*.cpp)


add_executable(main ${cpp_files})

 target_link_libraries(main mysql) 
 target_link_libraries(main WS2_32)