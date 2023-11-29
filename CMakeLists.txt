cmake_minimum_required(VERSION 3.10.0)
project(arduino-test-project VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

include(ExternalProject)

if (WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif (WIN32)

if (UNIX OR APPLE)
    find_package(Threads)
endif ()

# Download and install GoogleTest
ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)
ExternalProject_Get_Property(googletest source_dir binary_dir)

add_library(gtestd STATIC IMPORTED)
add_library(gtest_maind STATIC IMPORTED)

if (WIN32)
    set_property(TARGET gtestd PROPERTY IMPORTED_LOCATION ${binary_dir}/lib/Debug/gtestd.lib)
    set_property(TARGET gtest_maind PROPERTY IMPORTED_LOCATION ${binary_dir}/lib/Debug/gtest_maind.lib)
endif (WIN32)

if (UNIX OR APPLE)
    set_property(TARGET gtestd PROPERTY IMPORTED_LOCATION ${binary_dir}/lib/libgtest.a)
    set_property(TARGET gtest_maind PROPERTY IMPORTED_LOCATION ${binary_dir}/lib/libgtest_main.a)
endif ()

add_dependencies(gtestd googletest)
add_dependencies(gtest_maind googletest)
include_directories(${source_dir}/googletest/include)

# Add recursive .h and .cpp files to build
file(GLOB tmp_src "arduino-stub/*.h" "arduino-stub/*.cpp")

# Cleanup cached variable
set(arduino_test_src "" CACHE STRING "SRC list variable" FORCE)

function(add_source source_list)
    if("${arduino_test_src}" STREQUAL "")
        set(arduino_test_src "${source_list}" CACHE STRING "SRC list variable" FORCE)
    else()
        set(arduino_test_src "${arduino_test_src};${source_list}" CACHE STRING "SRC list variable" FORCE)
    endif()
endfunction(add_source)

add_source("${tmp_src}")

# Directory add include directory

# Cleanup cached variable
set (arduino_include_dirs "" CACHE STRING "Include directories" FORCE)
function(include_dir dir)
    if("${arduino_include_dirs}" STREQUAL "")
        set(arduino_include_dirs "${dir}" CACHE STRING "Include directories" FORCE)
    else()
        set(arduino_include_dirs "${arduino_include_dirs};${dir}" CACHE STRING "Include directories" FORCE)
    endif()
endfunction()

add_subdirectory(arduino-code)
include_directories(arduino-stub)

include_directories("${arduino_include_dirs}")

message("arduino_test_src: ${arduino_test_src}")
message("arduino_include_dirs: ${arduino_include_dirs}")

add_executable(arduino-test-project ${arduino_test_src})
target_compile_definitions(arduino-test-project PUBLIC ARDUINO_TEST)
if (UNIX OR APPLE)
    target_compile_definitions(arduino-test-project PUBLIC _LINUX_)
    target_link_libraries(arduino-test-project ${CMAKE_THREAD_LIBS_INIT})
endif ()

# Disable string safe functions warning
if (WIN32)
    target_compile_definitions(arduino-test-project PUBLIC _CRT_SECURE_NO_DEPRECATE)
    target_compile_definitions(arduino-test-project PUBLIC _CRT_NONSTDC_NO_DEPRECATE)
endif (WIN32)

target_link_libraries(arduino-test-project gtestd gtest_maind)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
