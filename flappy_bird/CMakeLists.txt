cmake_minimum_required(VERSION 3.10)

project(FlappyBird)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

set(TARGET_NAME FlappyBird)

set(SOURCE_DIR src)
file(GLOB_RECURSE SOURCE_FILES "${SOURCE_DIR}/*.cpp")

add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_link_libraries(${TARGET_NAME} PRIVATE sfml-system sfml-window sfml-graphics)
target_include_directories(${TARGET_NAME} PRIVATE include)

set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${TARGET_NAME}/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${TARGET_NAME}/Release"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/${TARGET_NAME}/RelWithDebInfo"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/${TARGET_NAME}/MinSizeRel"
)

add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${TARGET_NAME}>/resources
)
