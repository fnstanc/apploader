project (yatl_apploader)
cmake_minimum_required (VERSION 2.8)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

add_definitions(-Wall)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/_out/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/_out/${CMAKE_BUILD_TYPE})

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB HDRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" include/yatl/*.h include/yatl/*.hpp)
file(GLOB SRCS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" src/*.cpp src/*.cc src/*.h src/*.hpp)
file(GLOB RAPIDJSON_SRCS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" include/rapidjson/*.h)

add_executable(apploader ${HDRS} ${SRCS} ${RAPIDJSON_SRCS})
if (LINUX)
    target_link_libraries(apploader dl)
endif()

add_subdirectory(examples)
