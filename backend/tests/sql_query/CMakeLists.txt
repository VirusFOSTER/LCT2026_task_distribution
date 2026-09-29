cmake_minimum_required(VERSION 3.14)

project(sql_query LANGUAGES C CXX)

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

##############################################################
################    BOOST LIBRARY   ##########################
##############################################################
find_package(Boost COMPONENTS filesystem system program_options  REQUIRED)
include_directories( ${Boost_INCLUDE_DIRS} )
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(${Boost_DEFINITIONS})


##############################################################
#####################    SQLITE 3   ##########################
##############################################################
find_package (SQLite3)
if (SQLITE3_FOUND)
  include_directories(${SQLite3_INCLUDE_DIRS})
  link_directories(${SQLite3_LIBRARY_DIRS})
  add_definitions(${SQLite3_DEFINITIONS})
endif (SQLITE3_FOUND)

add_executable (sql_query
main.cpp
)

target_link_libraries (sql_query
  mps_common
)
