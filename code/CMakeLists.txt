cmake_minimum_required(VERSION 2.8)
project(fps)

SET(PACKAGE_VERSION "0.0.1")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

LIST( APPEND CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake
  ${CMAKE_SOURCE_DIR}/cmake/Modules
)

INCLUDE( CheckCXXCompilerFlag  )

FILE( GLOB filenames "cmake/Macros/*.cmake"  )
FOREACH( filename ${filenames}  )
  INCLUDE( ${filename}  )
ENDFOREACH()

add_subdirectory(pybind11)
FIND_PACKAGE(PythonInterp 3)
FIND_PACKAGE(PythonLibs   3)
INCLUDE_DIRECTORIES(
    ${PYTHON_INCLUDE_DIRS}
    ${PYBIND11_INCLUDE_DIRS}
)

# Eigen library
find_package (Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Boost
# find_package(Boost REQUIRED)
# include_directories(${Boost_INCLUDE_DIRS})
# link_directories(${Boost_LIBRARY_DIRS})
# add_definitions(${Boost_DEFINITIONS})

if (NOT DEFINED USE_OMP)
  set (USE_OMP false)
endif()
message(STATUS "Use OpemMP: ${USE_OMP}")

## Set compiler optimization flags
set(CMAKE_BUILD_TYPE Release)
if(NOT CXX_COMPILE_FLAGS_M)
  if (USE_OMP)
    set(CXX_COMPILE_FLAGS_M " -O2 -ffast-math -funsafe-math-optimizations -fopenmp -fPIC -Wno-deprecated -Wenum-compare ")
  else()
    set(CXX_COMPILE_FLAGS_M " -O2 -ffast-math -funsafe-math-optimizations -fPIC -Wno-deprecated -Wenum-compare ")
  endif()
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
add_library(${PROJECT_NAME}_farthest_point_sampling SHARED src/${PROJECT_NAME}/fps.cpp)
if (USE_OMP)
  target_compile_definitions(${PROJECT_NAME}_farthest_point_sampling PRIVATE USE_OMP)
endif()

ADD_LIBRARY( ${PROJECT_NAME}_python SHARED src/fps_python.cpp )
target_link_libraries(${PROJECT_NAME}_python
                      ${PROJECT_NAME}_farthest_point_sampling
                     )
SET_TARGET_PROPERTIES( ${PROJECT_NAME}_python
  PROPERTIES OUTPUT_NAME fps
  PREFIX ""
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/fps"
)

# This executable is for testing the shared library
add_executable(${PROJECT_NAME}_example src/example.cpp)
target_link_libraries(${PROJECT_NAME}_example
                      ${PROJECT_NAME}_farthest_point_sampling
                      #${Boost_LIBRARIES}
                      )

# Rename executable.
set_target_properties(${PROJECT_NAME}_example
                      PROPERTIES OUTPUT_NAME example
                      PREFIX "")

CONFIGURE_FILE( fps/__init__.py
  ${CMAKE_CURRENT_BINARY_DIR}/fps/__init__.py
)

CONFIGURE_FILE( fps/setup.py
  ${CMAKE_CURRENT_BINARY_DIR}/fps/setup.py
)
