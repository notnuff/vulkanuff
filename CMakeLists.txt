cmake_minimum_required(VERSION 3.30)

# TODO: try clang and install packages from github with cmake

# Project setup
project(vulkanuff LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Find GLFW
find_package(glfw3 REQUIRED)

# Find GLM
find_package(glm REQUIRED)

# Include directories
include_directories(
        ${Vulkan_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.inl
)

# Collect all source files
file(GLOB_RECURSE COMPILATION_UNITS CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c++
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
)

add_executable(${PROJECT_NAME}
        src/main.cpp
        ${COMPILATION_UNITS}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src/visual/vulkan_ctx")

include_directories(${CMAKE_SOURCE_DIR}/third/stb)
include_directories(${CMAKE_SOURCE_DIR}/third/tinyobjloader)
#link_directories(${CMAKE_SOURCE_DIR}/third/stb)


# Link libraries
target_link_libraries(${PROJECT_NAME}
        Vulkan::Vulkan
        glfw
        glm::glm
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_VALIDATION_LAYERS)
endif()
