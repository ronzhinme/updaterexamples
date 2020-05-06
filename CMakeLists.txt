cmake_minimum_required(VERSION 2.8)
project(updateExamples)

add_compile_options(-std=c++11)

include_directories("${PROJECT_SOURCE_DIR}/3d-party/appupdater/include")
include_directories("${PROJECT_SOURCE_DIR}/3d-party/pugixml/src")
link_directories("${PROJECT_SOURCE_DIR}/3d-party/appupdater/lib")

add_executable("updateCUI" main.cpp)
add_executable("updateCUIAllinOne" allInOne.cpp)

target_link_libraries("updateCUI" "appUpdaterShared")
target_link_libraries("updateCUIAllinOne" "appUpdaterShared")