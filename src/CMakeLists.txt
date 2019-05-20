cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_COMPILER "g++-8")
project (SC CXX)

add_definitions(-std=c++11)
#Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS regex)
if(Boost_FOUND)
include_directories(${Boost_INCLUDE_DIRS})
set(EXTRA_LIBS ${Boost_LIBRARIES} ${EXTRA_LIBS})
endif()
# GSL
find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})
set(EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
# OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
set(CMAKE_EXE_LINKER_FLAGS "-fopenmp")

include_directories(${SC_SOURCE_DIR})

add_library(Tools ${CMAKE_SOURCE_DIR}/Tools.cpp)
set(SC_LIBS ${SC_LIBS} Tools)
set(EXTRA_LIBS ${EXTRA_LIBS} Tools)

add_library(checker ${CMAKE_SOURCE_DIR}/checker.cpp)
set(SC_LIBS ${SC_LIBS} checker)
set(EXTRA_LIBS ${EXTRA_LIBS} checker)

add_library(BooleanNet ${CMAKE_SOURCE_DIR}/BooleanNet.cpp)
set(SC_LIBS ${SC_LIBS} BooleanNet)
set(EXTRA_LIBS ${EXTRA_LIBS} BooleanNet)

add_library(parser.tab ${CMAKE_SOURCE_DIR}/parser.tab.cpp)
set(SC_LIBS ${SC_LIBS} parser.tab)
set(EXTRA_LIBS ${EXTRA_LIBS} parser.tab)

add_library(interface ${CMAKE_SOURCE_DIR}/interface.cpp)
set(SC_LIBS ${SC_LIBS} interface)
set(EXTRA_LIBS ${EXTRA_LIBS} interface)

add_executable(SMC_bl ${CMAKE_SOURCE_DIR}/SMC_bl.cpp)
target_link_libraries(SMC_bl ${EXTRA_LIBS})
target_link_libraries(SMC_bl BooleanNet)