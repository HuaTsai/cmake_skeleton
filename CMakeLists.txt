cmake_minimum_required(VERSION 3.10)
project(
  myproject
  VERSION 1.0.0
  DESCRIPTION "A C++ project skeleton with CMake"
  HOMEPAGE_URL "https://github.com/HuaTsai/cmake_skeleton"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Format and lint
message(CHECK_START "Find clang-format")
find_program(CLANG_FORMAT NAMES clang-format) # requires install clang-format
if(CLANG_FORMAT)
  message(CHECK_PASS "found")
  file(GLOB_RECURSE SOURCES *.hpp *.cc)
  add_custom_target(
    format
    COMMAND ${CLANG_FORMAT} -i ${SOURCES}
    COMMENT "Formatting source files with clang-format"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
  message(CHECK_FAIL "not found, target 'format' will not be available")
endif()

option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" ON)

if(ENABLE_CLANG_TIDY)
  message(CHECK_START "Find clang-tidy")
  find_program(CLANG_TIDY NAMES clang-tidy) # requires install clang-tidy
  if(CLANG_TIDY)
    message(CHECK_PASS "found")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY}")
  else()
    message(CHECK_FAIL "not found, static analysis skipped.")
  endif()
else()
  message(STATUS "clang-tidy disabled (use -DENABLE_CLANG_TIDY=ON to enable)")
endif()

# Build include_directories() and link_libraries(): bad practice, global
# pollution
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

target_include_directories(
  math_utils PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>)
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

# Testing
include(CTest) # calls enable_testing() internally

if(BUILD_TESTING) # after include(CTest), the default is ON
  find_package(GTest REQUIRED) # requires install libgtest-dev
  add_executable(math_utils_test)
  target_sources(math_utils_test PRIVATE tests/math_utils_test.cc)
  target_link_libraries(math_utils_test PRIVATE math_utils GTest::gtest
                                                GTest::gtest_main)
  gtest_discover_tests(math_utils_test)
endif()

# Install
install(DIRECTORY include/ DESTINATION include) # includes

install(
  TARGETS math_utils main
  EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib # shared libraries *.so
  ARCHIVE DESTINATION lib # static libraries *.a
  RUNTIME DESTINATION bin # executables
)

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME})

include(CMakePackageConfigHelpers)

configure_package_config_file(
  cmake/${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME})

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION lib/cmake/${PROJECT_NAME})

# CPack
set(CPACK_GENERATOR "DEB;TGZ")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "HuaTsai <huatsai42@gmail.com>")
set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_DESCRIPTION
"A comprehensive C++ project template featuring CMake build system, \
unit testing with GTest, code formatting with clang-format, \
static analysis with clang-tidy, and packaging support")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "HuaTsai <huatsai42@gmail.com>")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${PROJECT_HOMEPAGE_URL}")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional") # most common priority
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack) # include after all variables are set

# Uninstall
configure_file("${CMAKE_SOURCE_DIR}/cmake/uninstall.cmake.in"
               "${CMAKE_BINARY_DIR}/cmake/uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(
  uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake/uninstall.cmake
  COMMENT "Uninstall ${PROJECT_NAME}")
