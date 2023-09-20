PROJECT(DVSProject)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

# add
set(CMAKE_CXX_STANDARD 11)
 
add_definitions(-std=c++11)
 
find_package(Boost COMPONENTS system thread program_options)
find_package(OpenCV REQUIRED) # change 
# find_package(OpenCV QUIET)

# CeleX
set(CeleX ./Samples)
set(CeleX_LIB CeleX)

link_directories(${CeleX}/lib/Linux/x64)
# CeleX


# target
set(LIBRARY_OUTPUT_PATH ../)

# change
set(dso_SOURCE_FILES getEventDataVector.cpp)

 
include_directories(
  ${OpenCV_INCLUDE_DIRS} 
  ${Boost_INCLUDE_DIRS}
  ${CeleX}/include
)
 
add_subdirectory(pybind11)
pybind11_add_module(example_pb ${dso_SOURCE_FILES})

target_link_libraries(example_pb PRIVATE
  ${Boost_LIBRARIES}
  ${OpenCV_LIBS}
  ${CeleX_LIB}
)
