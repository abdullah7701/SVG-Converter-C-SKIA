cmake_minimum_required(VERSION 3.10)

project(SVGConverter)

set(CMAKE_CXX_STANDARD 11)

# Set paths to Skia and JsonCpp
set(SKIA_INCLUDE_DIRS skia/headers)
set(SKIA_LIB_DIRS skia/libraries)
set(JSONCPP_INCLUDE_DIRS jsoncpp/headers)
set(JSONCPP_LIB_DIRS jsoncpp/libraries)

include_directories(${SKIA_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})
link_directories(${SKIA_LIB_DIRS} ${JSONCPP_LIB_DIRS})

# Add your source files
set(SOURCE_FILES main.cpp SVGElement.cpp SVGRect.cpp SVGCircle.cpp)

# Create the executable
add_executable(SVGConverter ${SOURCE_FILES})

# Link Skia and JsonCpp libraries
target_link_libraries(SVGConverter PRIVATE skia jsoncpp)
