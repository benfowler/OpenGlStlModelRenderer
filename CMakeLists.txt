cmake_minimum_required(VERSION 3.16)
project(OpenGL_STL_Viewer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# On macOS, we need to find the OpenGL framework
if(APPLE)
    find_package(OpenGL REQUIRED)
    find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
endif()

# --- Dependencies using FetchContent ---
include(FetchContent)

# GLFW for windowing and input
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)

# GLM for mathematics
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

# GLEW for OpenGL function loading
FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/nigels-com/glew.git
    GIT_TAG        glew-2.2.0
)
FetchContent_MakeAvailable(glew)

# Google Test for unit testing
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)


# --- Main Application Target ---
add_executable(OpenGL_STL_Viewer src/main.cpp)

# Include directories for dependencies
target_include_directories(OpenGL_STL_Viewer PUBLIC
    ${glfw_SOURCE_DIR}/include
    ${glm_SOURCE_DIR}
    ${glew_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(OpenGL_STL_Viewer PUBLIC
    glfw
    ${OPENGL_LIBRARIES}
    ${CORE_FOUNDATION_FRAMEWORK}
)
# GLEW needs to be compiled, so we create a library target for it.
# This part is a bit more complex because GLEW's source is in a `src` subdir.
add_library(GLEW_LIB STATIC ${glew_SOURCE_DIR}/src/glew.c)
target_include_directories(GLEW_LIB PUBLIC ${glew_SOURCE_DIR}/include)
target_compile_definitions(GLEW_LIB PRIVATE GLEW_STATIC) # Important for static linking

# Link our main app against our compiled GLEW lib
target_link_libraries(OpenGL_STL_Viewer PRIVATE GLEW_LIB)


# --- Unit Test Target ---
enable_testing()

add_executable(run_tests tests/test_stl_loader.cpp)

target_include_directories(run_tests PUBLIC
    ../src # To find STLLoader.hpp
)

target_link_libraries(run_tests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)

# --- Copy data files to build directory ---
# This ensures the executable can find the shaders and models
file(COPY shaders models DESTINATION ${CMAKE_BINARY_DIR})
