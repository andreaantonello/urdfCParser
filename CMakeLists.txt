cmake_minimum_required(VERSION 3.15) # Requirement driven by libxml2
project(urdf_parser)

# Set the project name as a variable
enable_language(C)
enable_language(CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Check if /build directory exists, if not create it
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/build)
    message(STATUS "Creating build directory: ${CMAKE_CURRENT_SOURCE_DIR}/build")
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
endif()

# Set the build folder to /build
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

### GOOGLE TESTING ###
# Enable testing for the project
enable_testing()

# 'gtest' is the folder with Google Test sources
set(GTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/lib/gtest)

# Check if the folder exists
if(NOT EXISTS ${GTEST_ROOT})
    message(STATUS "Google Test framework not found. Downloading...")
    execute_process(
            COMMAND git clone https://github.com/google/googletest.git ${GTEST_ROOT}
            RESULT_VARIABLE GIT_RESULT
    )
    if(NOT GIT_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to download Google Test framework. Aborting...")
    endif()
endif()

# Include the Google Test framework
add_subdirectory(${GTEST_ROOT})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})




find_package(LibXml2)


## 'libxml2' is the folder with libxml2 sources
#set(LIBXML2_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/libxml2)
#
## Check if the folder exists
#if(NOT EXISTS ${LIBXML2_INSTALL_DIR})
#    message(STATUS "libxml2 library not found. Downloading...")
#    execute_process(
#            COMMAND git clone https://github.com/GNOME/libxml2.git ${LIBXML2_INSTALL_DIR}
#            RESULT_VARIABLE GIT_RESULT
#    )
#    if(NOT GIT_RESULT EQUAL 0)
#        message(FATAL_ERROR "Failed to download libxml2 library. Aborting...")
#    endif()
#endif()
#
## Configure and build libxml2
#add_subdirectory(${LIBXML2_INSTALL_DIR})
#include_directories(${LIBXML2_INSTALL_DIR}/include/libxml2 ${LIBXML2_INSTALL_DIR}/include ${LIBXML2_INSTALL_DIR})

# Add subdirectories for the different components
add_subdirectory(src)

# Add the main executable and link it with the necessary files
add_executable(${PROJECT_NAME}_executable main.c)
target_link_libraries(${PROJECT_NAME}_executable PUBLIC src ${LIBXML2_LIBRARIES})
