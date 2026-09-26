cmake_minimum_required(VERSION 3.25)

project(mps_common VERSION 1.0 LANGUAGES CXX)
set(MAIN_PROJECT mps)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

##############################################################
################    BOOST LIBRARY   ##########################
##############################################################
find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} )
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(${Boost_DEFINITIONS})

if(TARGET Boost::filesystem)
    message(STATUS "Boost::filesystem target is available!")
else()
    message(STATUS "Boost::filesystem target NOT available. Using legacy variables.")
endif()

if(TARGET Boost::system)
    message(STATUS "Boost::system target is available!")
else()
    message(STATUS "Boost::system target NOT available. Using legacy variables.")
endif()

if(TARGET Boost::program_options)
    message(STATUS "Boost::program_options target is available!")
else()
    message(STATUS "Boost::program_options target NOT available. Using legacy variables.")
endif()

#-----------------------------------------------------------#
###################### TYPE_TRAITS ##########################
#-----------------------------------------------------------#
set(MPS_TYPES_PROPERTY_PATH include/type_traits/types/properties)

set(MPS_TYPE_TRAITS_HEADERS
    ${MPS_TYPES_PROPERTY_PATH}/color.hpp
    ${MPS_TYPES_PROPERTY_PATH}/del_copy_constructor.hpp
    ${MPS_TYPES_PROPERTY_PATH}/member_detector.hpp
    ${MPS_TYPES_PROPERTY_PATH}/member_type_detector.hpp
    ${MPS_TYPES_PROPERTY_PATH}/point_dimension.hpp
    ${MPS_TYPES_PROPERTY_PATH}/reference_counter.hpp
    ${MPS_TYPES_PROPERTY_PATH}/type_detector.hpp
)

set(MPS_TYPE_TRAITS
    ${MPS_TYPE_TRAITS_HEADERS}
)

#-----------------------------------------------------------#
######################## UTILS ##############################
#-----------------------------------------------------------#
set(HMPS_UTILS_PATH include/utils)
set(SMPS_UTILS_PATH src/utils)

set(HMPS_JSON_IO_PATH ${HMPS_UTILS_PATH}/json_io)
set(SMPS_JSON_IO_PATH ${SMPS_UTILS_PATH}/json_io)

####################### JSON_IO ############################
set(MPS_JSON_IO_HEADERS
    ${HMPS_JSON_IO_PATH}/json.hpp
    ${HMPS_JSON_IO_PATH}/json_array.hpp
    ${HMPS_JSON_IO_PATH}/json_common.hpp
    ${HMPS_JSON_IO_PATH}/json_loader.hpp
    ${HMPS_JSON_IO_PATH}/json_object.hpp
    ${HMPS_JSON_IO_PATH}/json_output.hpp
)

set(MPS_JSON_IO_SOURCES
    ${SMPS_JSON_IO_PATH}/json_array.cpp
    ${SMPS_JSON_IO_PATH}/json_common.cpp
    ${SMPS_JSON_IO_PATH}/json_loader.cpp
    ${SMPS_JSON_IO_PATH}/json_object.cpp
    ${SMPS_JSON_IO_PATH}/json_output.cpp
)

set(MPS_JSON_IO
    ${MPS_JSON_IO_HEADERS}
    ${MPS_JSON_IO_SOURCES}
)

################### PARSE ARGUMENTS #########################
set(MPS_PARSE_PROCESS_ARGUMENTS_HEADERS
    ${HMPS_UTILS_PATH}/parse_process_arguments/parse_process_arguments.hpp
)

set(MPS_PARSE_PROCESS_ARGUMENTS_SOURCES
    ${SMPS_UTILS_PATH}/parse_process_arguments/parse_process_arguments.cpp
)

set(MPS_PARSE_PROCESS_ARGUMENTS
    ${MPS_PARSE_PROCESS_ARGUMENTS_HEADERS}
    ${MPS_PARSE_PROCESS_ARGUMENTS_SOURCES}
)

################# STRING HEX CONVERTER ######################
set(HMPS_SH_CONVERTER_PATH include/utils/string_hex_converter)
set(SMPS_SH_CONVERTER_PATH src/utils/string_hex_converter)

set(MPS_STRING_HEX_CONVERTER_HEADERS
    ${HMPS_SH_CONVERTER_PATH}/string_hex_converter.hpp
)

set(MPS_STRING_HEX_CONVERTER_SOURCES
    ${SMPS_SH_CONVERTER_PATH}/string_hex_converter.cpp
)

set(MPS_STRING_HEX_CONVERTER
    ${MPS_STRING_HEX_CONVERTER_HEADERS}
    ${MPS_STRING_HEX_CONVERTER_SOURCES}
)

########################### MATH #########################
########################### MATH OPTIMIZATION #########################
set(MPS_MATH_OPTIMIZATION_ROUND_VALUES
    ${HMPS_UTILS_PATH}/math/optimization/round_values.hpp
)

set(MPS_MATH_OPTIMIZATION
    ${MPS_MATH_OPTIMIZATION_ROUND_VALUES}
)

########################### MATH STATISTIC #########################
set(MPS_MATH_STATISTIC
    ${HMPS_UTILS_PATH}/math/statistic/math_expected_value_wrapper.hpp
    ${HMPS_UTILS_PATH}/math/statistic/mean_value_wrapper.hpp
    ${HMPS_UTILS_PATH}/math/statistic/variance_value_wrapper.hpp
)

set(MPS_MATH_HEADERS
    ${MPS_MATH_OPTIMIZATION}
    ${MPS_MATH_STATISTIC}
)


########################### UTILS #########################
set(MPS_UTILS_HEADERS
    ${MPS_JSON_IO_HEADERS}
    ${MPS_PARSE_PROCESS_ARGUMENTS_HEADERS}
    ${MPS_STRING_HEX_CONVERTER_HEADERS}
    ${MPS_MATH_HEADERS}
)

set(MPS_UTILS_SOURCES
    ${MPS_JSON_IO_SOURCES}
    ${MPS_PARSE_PROCESS_ARGUMENTS_SOURCES}
    ${MPS_STRING_HEX_CONVERTER_SOURCES}
)

set(MPS_UTILS
    ${MPS_UTILS_HEADERS}
    ${MPS_UTILS_SOURCES}
)

#-----------------------------------------------------------#
###################### PROJECT SOURCES ######################
#-----------------------------------------------------------#
set(MPS_COMMON_SOURCES
    ${MPS_TYPE_TRAITS}
    ${MPS_UTILS}
)

set(CMAKE_INSTALL_PREFIX "/usr/local")

add_library(${PROJECT_NAME} STATIC ${MPS_COMMON_SOURCES})
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${Boost_LIBRARIES}
)

# Определяем путь установки для заголовков
set(include_install_dir "include/${MAIN_PROJECT}/${PROJECT_NAME}")

target_sources(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${include_install_dir}>
)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${include_install_dir}>
)

# Установка заголовков
install(DIRECTORY include/ DESTINATION ${include_install_dir})

# Экспорт цели с пространством имен
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE mps::
    DESTINATION share/${MAIN_PROJECT}/${PROJECT_NAME}/cmake
)

include(CMakePackageConfigHelpers)

# Генерация файла версии
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Генерация основного Config-файла
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION share/${MAIN_PROJECT}/${PROJECT_NAME}/cmake
)

# Установка Config-файлов
install(FILES
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION share/${MAIN_PROJECT}/${PROJECT_NAME}/cmake
)

include_directories(include)
