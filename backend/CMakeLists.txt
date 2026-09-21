cmake_minimum_required(VERSION 3.14)

project(task_distribution LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DVISUALIZATION_PROJECT_PATH="${PROJECT_SOURCE_DIR}")

###############################################################
################  MPS COMMON    ###############################
###############################################################
find_package(mps_common REQUIRED)
if(TARGET mps_common)
    message(STATUS "mps_common target is available!")
else()
    message(STATUS "mps_common target NOT available. Using legacy variables.")
endif()

find_package(mps_process_traits REQUIRED)
if(TARGET mps_process_traits)
    message(STATUS "mps_process_traits target is available!")
else()
    message(STATUS "mps_process_traits target NOT available. Using legacy variables.")
endif()

###############################################################
##############  QT FRAMEWORK    ###############################
###############################################################
find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGLWidgets PrintSupport Widgets)
include_directories(${Qt6Widgets_INCLUDE_DIR})
link_directories(${Qt6Widgets_LIBRARY_DIR})
add_definitions(${Qt6Widgets_DEFINITIONS})

###############################################################
##############  ZERO MQ FRAMEWORK   ###########################
###############################################################
find_path(ZeroMQ_INCLUDE_DIR
  NAMES zmq.hpp
  PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
)

find_library(ZeroMQ_LIBRARY
  NAMES zmq
  PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
)

##############################################################
################    OPENGL LIBRARY   #########################
##############################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGl_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

find_package(GLUT REQUIRED)

##############################################################
################    BOOST LIBRARY   ##########################
##############################################################
find_package(Boost COMPONENTS filesystem system program_options  REQUIRED)
include_directories( ${Boost_INCLUDE_DIRS} )
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(${Boost_DEFINITIONS})

file(GLOB_RECURSE VISUALIZATION_HEADERS "include/*.hpp")
file(GLOB_RECURSE VISUALIZATION_SOURCES "src/*.cpp")


################################################################################################
########################################  PROCESS_SOURCES   ####################################
################################################################################################
set(PROCESS_SOURCES
  ${VISUALIZATION_HEADERS}
  ${VISUALIZATION_SOURCES}
)

add_executable(${PROJECT_NAME}
  main.cpp
  ${PROCESS_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC Qt6::Widgets
  ${Boost_LIBRARIES}
  ${ZeroMQ_LIBRARY}
  Qt6::OpenGL
  Qt6::Core
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARIES}
  pthread
  boost_thread
  ${mps_common_LIBRARIES}
  ${mps_process_traits_LIBRARIES}
)

include_directories(include)
