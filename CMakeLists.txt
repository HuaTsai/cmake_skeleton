cmake_minimum_required(VERSION 3.10)
project(myproject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic)

find_program(CLANG_FORMAT NAMES clang-format) # requires install clang-format
if(CLANG_FORMAT)
  file(GLOB_RECURSE SOURCES *.hpp *.cc)
  add_custom_target(
    format
    COMMAND ${CLANG_FORMAT} -i ${SOURCES}
    COMMENT "Formatting source files with clang-format"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
  message(
    WARNING "clang-format not found! target 'format' will not be available.")
endif()

option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)

if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY NAMES clang-tidy) # requires install clang-tidy
  if(CLANG_TIDY)
    message(STATUS "clang-tidy found: ${CLANG_TIDY}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY}")
  else()
    message(WARNING "clang-tidy not found! static analysis skipped.")
  endif()
else()
  message(STATUS "clang-tidy disabled (use -DENABLE_CLANG_TIDY=ON to enable)")
endif()

# include_directories() and link_libraries(): bad practice, global pollution

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

add_library(math_utils)
#[[
Default is STATIC if BUILD_SHARED_LIBS is OFF, SHARED if ON
STATIC: *.a, good for
  1. not relying on external files
  2. simpler deployment
  3. faster runtime performance
  4. secure from attacks
SHARED: *.so, good for
  1. smaller executable size
  2. shared among executables
  3. faster compile time for modifications
INTERFACE: header-only libraries
  1. no compilation needed, i.e., only target_include_directories() needed
  2. target_link_libraries() with INTERFACE keyword
]]

target_include_directories(math_utils PUBLIC include)
#[[
PUBLIC: downstream targets that link to this target will also include these directories
PRIVATE: only this target will include these directories
INTERFACE: only downstream targets that link to this target will include these directories
]]

target_sources(math_utils PRIVATE src/math_utils.cc)
#[[
PUBLIC: sources used for this target and downstream targets
PRIVATE: sources used only for this target
INTERFACE: sources used only for downstream targets
]]

add_executable(main)
target_sources(main PRIVATE src/main.cc)
target_link_libraries(main PRIVATE math_utils)

include(CTest)

if(BUILD_TESTING) # default is ON
  find_package(GTest REQUIRED) # requires install libgtest-dev
  add_executable(math_utils_test)
  target_sources(math_utils_test PRIVATE tests/math_utils_test.cc)
  target_link_libraries(math_utils_test PRIVATE math_utils GTest::gtest GTest::gtest_main)
  gtest_discover_tests(math_utils_test)
endif()
