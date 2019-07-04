cmake_minimum_required(VERSION 3.0.0)
project(PathBuilder)

set(CMAKE_CXX_STANDARD 14)

# We need thread support
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

#-------------------------GTEST-------------------------------#

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

#-------------------------LIBRARIES-------------------------------#

find_library(LIBEVENT2_CORE event_core)
add_subdirectory(include/SimpleWeb)

#-------------------------EXECUTABLES-------------------------------#

include_directories(include)
file(GLOB_RECURSE SRCS
        "src/pbuilder/*.h"
        "src/pbuilder/*.cpp")
#set(SRCS src/pbuilder/Data/Place.cpp
#        src/pbuilder/Data/TimePoint.cpp
#        src/pbuilder/Structures/Mat.cpp
#        src/pbuilder/Parser/Impl1.cpp
#        src/pbuilder/PathBuilder/Impl1.cpp
#        src/pbuilder/PathChecker/Impl1.cpp
#)
add_executable(PathBuilder ${SRCS} src/main.cpp)
target_link_libraries(PathBuilder  ${LIBEVENT2_CORE} simple-web-server)

#-------------------------EXECUTABLES/TESTS-------------------------------#

file(GLOB_RECURSE TESTS
        "test/*.h"
        "test/*.cpp"
)

add_executable(tests ${SRCS} ${TESTS})

target_link_libraries(tests
        gtest_main
        ${LIBEVENT2_CORE}
)

add_custom_target(run
        COMMAND test
        DEPENDS tests
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

#-------------------------EXECUTABLES/TESTS-------------------------------#

install(TARGETS PathBuilder tests DESTINATION bin)