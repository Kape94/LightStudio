cmake_minimum_required(VERSION 3.22)
project(LightStudio)

add_subdirectory(AppToolkit)

file(GLOB SRC_FILES Src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE AppToolkit)
