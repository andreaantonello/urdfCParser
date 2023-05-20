cmake_minimum_required(VERSION 3.5)
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
set(GTEST_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/gtest)

# Check if the folder exists
if(NOT EXISTS ${GTEST_INSTALL_DIR})
    message(STATUS "Google Test framework not found. Downloading...")
    execute_process(
            COMMAND git clone https://github.com/google/googletest.git ${GTEST_INSTALL_DIR}
            RESULT_VARIABLE GIT_RESULT
    )
    if(NOT GIT_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to download Google Test framework. Aborting...")
    endif()
endif()

# Include the Google Test framework
add_subdirectory(${GTEST_INSTALL_DIR})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})



# 'liblzma' is the folder where LibLZMA sources will be downloaded and built
set(LIBLZMA_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/liblzma)

# Check if the folder exists
if(NOT EXISTS ${LIBLZMA_INSTALL_DIR})
    message(STATUS "LibLZMA library not found. Downloading and building...")

    include(ExternalProject)

    # Define the ExternalProject
    ExternalProject_Add(liblzma
            GIT_REPOSITORY https://git.tukaani.org/xz.git
            GIT_TAG v5.2.5  # Replace with the desired version of LibLZMA
            SOURCE_DIR ${LIBLZMA_INSTALL_DIR}
            CONFIGURE_COMMAND ${LIBLZMA_INSTALL_DIR}/autogen.sh && ${LIBLZMA_INSTALL_DIR}/configure --prefix=${LIBLZMA_INSTALL_DIR}
            BUILD_COMMAND make
            INSTALL_COMMAND make install
            )

    # Ensure that LibLZMA is built before it is used
    add_dependencies(liblzma ${PROJECT_NAME})
endif()

# Set the necessary variables for LibLZMA
set(LIBLZMA_INCLUDE_DIR ${LIBLZMA_INSTALL_DIR}/include)
set(LIBLZMA_LIBRARIES ${LIBLZMA_INSTALL_DIR}/lib/liblzma.a)

# Add the include directory and link against the LibLZMA library
include_directories(${LIBLZMA_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} ${LIBLZMA_LIBRARIES})








# 'libxml2' is the folder with libxml2 sources
set(LIBXML2_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/libxml2)

# Check if the folder exists
if(NOT EXISTS ${LIBXML2_INSTALL_DIR})
    message(STATUS "libxml2 library not found. Downloading...")
    execute_process(
            COMMAND git clone https://github.com/GNOME/libxml2.git ${LIBXML2_INSTALL_DIR}
            RESULT_VARIABLE GIT_RESULT
    )
    if(NOT GIT_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to download libxml2 library. Aborting...")
    endif()

    # Build libxml2 using the 'libxml2' folder
    message(STATUS "Building libxml2...")
    execute_process(
            COMMAND ${CMAKE_COMMAND} -S ${LIBXML2_INSTALL_DIR} -B ${LIBXML2_INSTALL_DIR}/build
            RESULT_VARIABLE CMAKE_RESULT
    )
    if(NOT CMAKE_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to configure libxml2. Aborting...")
    endif()

    execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${LIBXML2_INSTALL_DIR}/build --target install
            RESULT_VARIABLE BUILD_RESULT
    )
    if(NOT BUILD_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to build libxml2. Aborting...")
    endif()
endif()

# Specify the include directory for libxml2
include_directories(${LIBXML2_INSTALL_DIR}/include)

# Link against the libxml2 library
link_directories(${LIBXML2_INSTALL_DIR}/lib)





# Add subdirectories for the different components
add_subdirectory(src)

# Add the main executable and link it with the necessary files
add_executable(${PROJECT_NAME}_executable main.c)
target_link_libraries(${PROJECT_NAME}_executable PUBLIC src)