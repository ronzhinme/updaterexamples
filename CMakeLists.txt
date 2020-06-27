cmake_minimum_required(VERSION 2.8)
project(updateExamples)

add_compile_options(-std=c++11)

include_directories("${PROJECT_SOURCE_DIR}/3d-party/appupdater/include")
include_directories("${PROJECT_SOURCE_DIR}/3d-party/pugixml/src")

if (WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
        link_directories("${PROJECT_SOURCE_DIR}/3d-party/appupdater/lib_x64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32 bits
        link_directories("${PROJECT_SOURCE_DIR}/3d-party/appupdater/lib_x86")
    endif()
else()
    link_directories("${PROJECT_SOURCE_DIR}/3d-party/appupdater/lib")
endif()


add_executable("updateCUI" main.cpp)
add_executable("updateCUIAllinOne" allInOne.cpp)
add_executable("updateWithPauses" pause_resume.cpp)

target_link_libraries("updateCUI" "appUpdaterShared")
target_link_libraries("updateCUIAllinOne" "appUpdaterShared")
target_link_libraries("updateWithPauses" "appUpdaterShared")