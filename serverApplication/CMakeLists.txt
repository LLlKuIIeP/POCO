cmake_minimum_required(VERSION 2.8)

project(serverApplication)
add_executable(${PROJECT_NAME} "main.cpp")

find_package(Poco COMPONENTS Util)

target_link_libraries(${PROJECT_NAME} Poco::Util)
