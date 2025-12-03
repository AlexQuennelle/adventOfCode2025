cmake_minimum_required(VERSION 3.30.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

if("${CMAKE_GENERATOR}" STREQUAL "Ninja")
  set(WINDOWS TRUE)
  message("Ninja")
else() # set(CMAKE_CXX_FLAGS "-Wall -Wextra")

endif()

set(CMAKE_CXX_FLAGS
    "-Wall -Wextra -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused\
    -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion\
    -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wduplicated-cond\
    -Wduplicated-branches -Wlogical-op -Wuseless-cast")
if(NOT WINDOWS)
  add_compile_options("-fsanitize=undefined")
endif()

project(
  adventOfCode2025
  LANGUAGES CXX C
  VERSION 0.1)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

message("Build type: ${CMAKE_BUILD_TYPE}")

add_executable("${CMAKE_PROJECT_NAME}" src/day03.cpp)

if(NOT WINDOWS)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
  # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

# file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS
# "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

target_compile_definitions(
  ${CMAKE_PROJECT_NAME} PUBLIC $<$<CONFIG:Release>:PRODUCTION_BUILD>
                               NAME="${CMAKE_PROJECT_NAME}")

set_target_properties(
  "${CMAKE_PROJECT_NAME}"
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin"
             RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/\
${CMAKE_PROJECT_NAME}_${CMAKE_PROJECT_VERSION}")

set(RELEASE_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/\
${CMAKE_PROJECT_NAME}_${CMAKE_PROJECT_VERSION}/resources/")
set(DEBUG_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(
  ${CMAKE_PROJECT_NAME}
  PUBLIC
    RESOURCES_PATH=$<IF:$<CONFIG:Release>,"./resources/","${DEBUG_RESOURCE_DIR}">
)

target_compile_features("${CMAKE_PROJECT_NAME}" PRIVATE cxx_std_23)

# target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES})

target_include_directories("${CMAKE_PROJECT_NAME}"
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories("${CMAKE_PROJECT_NAME}"
                           PUBLIC SYSTEM "${PROJECT_BINARY_DIR}")

add_custom_command(
  TARGET ${CMAKE_PROJECT_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
    $<IF:$<CONFIG:Release>,"${RELEASE_RESOURCE_DIR}","${DEBUG_RESOURCE_DIR}">)
