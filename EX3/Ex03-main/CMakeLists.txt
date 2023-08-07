cmake_minimum_required(VERSION 3.20)
project(Ex03)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(Headers)

# Add the executable target
add_executable(flightsService "Cpp files/flightsService.cpp") 
add_executable(dbService "Cpp files/dbService.cpp") 

# Add the library target
add_library(Utility SHARED Library/libUtility.cpp)

# Find the libzip library
find_library(LIBZIP_LIBRARY zip)

# Link the executable with the libraries
target_link_libraries(flightsService Utility )
target_link_libraries(dbService Utility ${LIBZIP_LIBRARY})

