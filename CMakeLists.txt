cmake_minimum_required(VERSION 3.14)
project(
  "OStream-Extra"
  VERSION 0.5.0
  DESCRIPTION "Extra overrides of operator<< for common types.")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
# This project only has interface headers.  No translation units.
#
add_library(${PROJECT_NAME} INTERFACE)

file(
  GLOB_RECURSE PUB_FILES
  LIST_DIRECTORIES false
  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/public/*)
target_sources(${PROJECT_NAME} INTERFACE ${PUB_FILES})
# Debug dump of public interface files:
foreach(pub_file ${PUB_FILES})
  message(STATUS "${PROJECT_NAME} pub file: ${pub_file}")
endforeach(pub_file)

#
# The public include directory for this library's clients
#
target_include_directories(${PROJECT_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/public)

#
# Boost
#
if(NOT DEFINED ENV{BOOST_REQUIRED_COMPONENTS})
  set(ENV{BOOST_REQUIRED_COMPONENTS} "boost")
endif()
function(boost_add_component)
  foreach(boost_component ${ARGV})
    set(ENV{BOOST_REQUIRED_COMPONENTS}
        "$ENV{BOOST_REQUIRED_COMPONENTS};${boost_component}")
  endforeach(boost_component)
endfunction()

# Build unit tests only if this project isn't a submodule in a super-project.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  message(STATUS "Building unit tests: ${BUILD_TESTING}")
  if(BUILD_TESTING)
    boost_add_component(filesystem date_time regex unit_test_framework)
  endif(BUILD_TESTING)
endif()

#
# Unit tests
#
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory("Unit-Tests")
endif()
