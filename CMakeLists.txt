cmake_minimum_required(VERSION 3.10)
project(vision_cpp)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)           # 系统里找 OpenCV，找不到就报错
add_executable(vision main.cpp)
target_link_libraries(vision ${OpenCV_LIBS})   # 把 OpenCV 的库链接进来