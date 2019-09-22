cmake_minimum_required(VERSION 2.8)
project(updateExamples)

add_compile_options(-std=c++11)

include_directories("${PROJECT_SOURCE_DIR}/appupdaterlib/src")
include_directories("${PROJECT_SOURCE_DIR}/appupdaterlib/3d-party/pugixml/src")

link_directories("${PROJECT_SOURCE_DIR}/appupdaterlib/build/src")

add_executable("updateCUI" main.cpp)
target_link_libraries("updateCUI" "appUpdaterShared")