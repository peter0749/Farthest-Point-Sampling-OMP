cmake_minimum_required(VERSION 2.8)
project(fps)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Eigen library
message(STATUS ${EIGEN3_INCLUDE_DIR})
find_package (Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Boost
find_package(Boost REQUIRED)
find_package(Boost COMPONENTS thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(${Boost_DEFINITIONS})

## Set compiler optimization flags
set(CMAKE_BUILD_TYPE Release)
if(NOT CXX_COMPILE_FLAGS_M)
  set(CXX_COMPILE_FLAGS_M " -O2 -ffast-math -fopenmp -fPIC -Wno-deprecated -Wenum-compare ")
endif()
if(NOT EXTRA_CXX_FLAG)
  set(EXTRA_CXX_FLAG "")
endif()

string(REPLACE "-O3" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
string(REPLACE "-O3" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

string(REPLACE "-O2" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REPLACE "-O2" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
string(REPLACE "-O2" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")


string(REPLACE "-g" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REPLACE "-g" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
string(REPLACE "-g" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS_M} ${EXTRA_CXX_FLAG}")


# Add the headers from the <include> directory
include_directories(include)

# Other libraries
add_library(${PROJECT_NAME}_farthest_point_sampling src/${PROJECT_NAME}/fps.cpp)


# This executable is for testing the shared library
add_executable(${PROJECT_NAME}_example src/example.cpp)
target_link_libraries(${PROJECT_NAME}_example
                      ${PROJECT_NAME}_farthest_point_sampling
                      ${Boost_LIBRARIES}
                      Boost::thread)

# Rename executable.
set_target_properties(${PROJECT_NAME}_example
                      PROPERTIES OUTPUT_NAME example
                      PREFIX "")

# Set the location for library installation (/usr/lib in this case is not really necessary)
# Use "sudo make install" to apply
install(TARGETS ${PROJECT_NAME}_example
        DESTINATION lib)

install(DIRECTORY include/gpg DESTINATION include)

