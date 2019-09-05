cmake_minimum_required(VERSION 2.8)
project(updateExamples)

include_directories("${PROJECT_SOURCE_DIR}/lib")
link_directories("${PROJECT_SOURCE_DIR}/lib")

add_executable("updateCUI" main.cpp)
target_link_libraries("updateCUI" "appUpdaterShared")