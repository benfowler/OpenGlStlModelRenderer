cmake_minimum_required(VERSION 4.1.1)

project(OpenGL_STL_Viewer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_language(OBJC)

# On macOS, we need to find the OpenGL framework
if(APPLE)
    find_package(OpenGL REQUIRED)
    find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
endif()

# --- Library dependencies
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLM CONFIG REQUIRED)

# --- Dependencies using FetchContent ---
include(FetchContent)

# Google Test for unit testing
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)

# --- Main Application Target ---
add_executable(OpenGL_STL_Viewer src/main.cpp)

# Link libraries
target_link_libraries(OpenGL_STL_Viewer PRIVATE
    ${OPENGL_LIBRARIES}
    ${CORE_FOUNDATION_FRAMEWORK}
    GLEW::GLEW
    glfw
    glm::glm
)

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
