cmake_minimum_required(VERSION 3.22)

project(mps_process_traits VERSION 1.0 LANGUAGES CXX)
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
    message(SEND_ERROR "Boost::filesystem target NOT available! Using legacy variables.")
endif()

if(TARGET Boost::system)
    message(STATUS "Boost::system target is available!")
else()
    message(SEND_ERROR "Boost::system target NOT available! Using legacy variables.")
endif()

if(TARGET Boost::program_options)
    message(STATUS "Boost::program_options target is available!")
else()
    message(SEND_ERROR "Boost::program_options target NOT available! Using legacy variables.")
endif()


###############################################################
##############  QT FRAMEWORK    ###############################
###############################################################
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
include_directories(${Qt6Widgets_INCLUDE_DIR})
link_directories(${Qt6Widgets_LIBRARY_DIR})
add_definitions(${Qt6Widgets_DEFINITIONS})

if(TARGET Qt6::Core)
    message(STATUS "Qt6::Core target is available!")
else()
    message(SEND_ERROR "Qt6::Core tartget NOT available! Using legacy variables.")
endif()

if(TARGET Qt6::Gui)
    message(STATUS "Qt6::Gui target is available!")
else()
    message(SEND_ERROR "Qt6::Gui tartget NOT available! Using legacy variables.")
endif()

if(TARGET Qt6::Widgets)
    message(STATUS "Qt6::Widgets target is available!")
else()
    message(SEND_ERROR "Qt6::Widgets tartget NOT available! Using legacy variables.")
endif()


###############################################################
##################  MPS COMMON  ###############################
###############################################################
find_package(mps_common REQUIRED)
if(TARGET mps_common)
    message(STATUS "mps_json_io target is available!")
else()
    message(STATUS "mps_json_io target NOT available. Using legacy variables.")
endif()


#--------------------------------------------------------------#
###################### PROCESS COMPONENTS ######################
#--------------------------------------------------------------#
set(HMPS_PROCESS_COMPONENTS_PATH include/components_traits)
set(SMPS_PROCESS_COMPONENTS_PATH src/components_traits)

######################## BASE FUNCTIONAL COMPONENTS ################
set(BASE_COMPONENTS_HEADERS
    ${HMPS_PROCESS_COMPONENTS_PATH}/base_component/base_functional_component.hpp
    ${HMPS_PROCESS_COMPONENTS_PATH}/base_component/qbase_functional_component.hpp
)

set(BASE_COMPONENTS_SOURCES
    ${SMPS_PROCESS_COMPONENTS_PATH}/base_component/base_functional_component.cpp
    ${SMPS_PROCESS_COMPONENTS_PATH}/base_component/qbase_functional_component.cpp
)

######################## COMPONENTS CONTAINER ################
set(COMPONENTS_CONTAINER_HEADERS
    ${HMPS_PROCESS_COMPONENTS_PATH}/components_container/components_container.hpp
    ${HMPS_PROCESS_COMPONENTS_PATH}/components_container/qcomponents_container.hpp
)

set(COMPONENTS_CONTAINER_SOURCES
    ${SMPS_PROCESS_COMPONENTS_PATH}/components_container/components_container.cpp
    ${SMPS_PROCESS_COMPONENTS_PATH}/components_container/qcomponents_container.cpp
)

#--------------------------------------------------------------#
###################### PROCESS INTERFACES ######################
#--------------------------------------------------------------#
set(HMPS_PROCESS_INTERFACES_PATH include/interfaces_traits)

############################ MESSAGES BUFFERS ##########################
set(BUFFER_MODELS
    ${HMPS_PROCESS_INTERFACES_PATH}/buffer_models/list_buffer_element.hpp
    ${HMPS_PROCESS_INTERFACES_PATH}/buffer_models/list_buffer_model.hpp
    ${HMPS_PROCESS_INTERFACES_PATH}/buffer_models/list_element_optional.hpp
)

set(PROCESS_BUFFER
    ${HMPS_PROCESS_INTERFACES_PATH}/buffer_concept.hpp
    ${HMPS_PROCESS_INTERFACES_PATH}/buffer_description.hpp
    ${BUFFER_MODELS}
)

########################### MESSAGES INTERFACES #######################
set(PROCESS_INTERFACES
    ${PROCESS_BUFFER}
    ${HMPS_PROCESS_INTERFACES_PATH}/interfaces_traits.hpp
    ${HMPS_PROCESS_INTERFACES_PATH}/message_reader_concept.hpp
    ${HMPS_PROCESS_INTERFACES_PATH}/message_writer_concept.hpp
    ${HMPS_PROCESS_INTERFACES_PATH}/sequence_reader.hpp
    ${HMPS_PROCESS_INTERFACES_PATH}/sequence_writer.hpp
)

#--------------------------------------------------------------#
###################### PROCESS MESSAGES ########################
#--------------------------------------------------------------#
set(HMPS_PROCESS_MESSAGES_PATH include/messages_traits)
set(SMPS_PROCESS_MESSAGES_PATH src/messages_traits)

###################### REGISTER MESSAGES CONTAINER ######################
set(MESSAGES_CONTAINER_HEADERS
    ${HMPS_PROCESS_MESSAGES_PATH}/messages_container/register_message_concept.hpp
    ${HMPS_PROCESS_MESSAGES_PATH}/messages_container/register_message_model.hpp
    ${HMPS_PROCESS_MESSAGES_PATH}/messages_container/register_messages_container.hpp
)

set(MESSAGES_CONTAINER_SOURCES
    ${SMPS_PROCESS_MESSAGES_PATH}/messages_container/register_messages_container.cpp
)

###################### MESSAGES TRAITS ################################
set(MESSAGES_TRAITS_HEADERS
    ${HMPS_PROCESS_MESSAGES_PATH}/message_head.hpp
    ${HMPS_PROCESS_MESSAGES_PATH}/messages_traits.hpp
    ${MESSAGES_CONTAINER_HEADERS}
)

set(MESSAGES_TRAITS_SOURCES
    ${SMPS_PROCESS_MESSAGES_PATH}/message_head.cpp
    ${MESSAGES_CONTAINER_SOURCES}
)

#-------------------------------------------------------------------#
###################### PROCESS CONFIGURATION ########################
#-------------------------------------------------------------------#
set(HMPS_PROCESS_CONFIGURATION_PATH include/process_configuration)
set(SMPS_PROCESS_CONFIGURATION_PATH src/process_configuration)

set(PROCESS_CONFIGURATION_HEADERS
    ${HMPS_PROCESS_CONFIGURATION_PATH}/components_configuration/component_configuration.hpp
    ${HMPS_PROCESS_CONFIGURATION_PATH}/components_configuration/components_configuration.hpp
    ${HMPS_PROCESS_CONFIGURATION_PATH}/messages_configuration/message_configuration.hpp
    ${HMPS_PROCESS_CONFIGURATION_PATH}/messages_configuration/messages_configuration.hpp
    ${HMPS_PROCESS_CONFIGURATION_PATH}/connection_configuration/connection_configuration.hpp
    ${HMPS_PROCESS_CONFIGURATION_PATH}/connection_configuration/connections_configuration.hpp
    ${HMPS_PROCESS_CONFIGURATION_PATH}/connection_configuration/managed_memory_connection_configuration.hpp
    ${HMPS_PROCESS_CONFIGURATION_PATH}/connection_configuration/socket_connection_configuration.hpp
    ${HMPS_PROCESS_CONFIGURATION_PATH}/process_configuration.hpp
    ${HMPS_PROCESS_CONFIGURATION_PATH}/sglobal_config.hpp
)

set(PROCESS_CONFIGURATION_SOURCES
    ${SMPS_PROCESS_CONFIGURATION_PATH}/components_configuration/component_configuration.cpp
    ${SMPS_PROCESS_CONFIGURATION_PATH}/components_configuration/components_configuration.cpp
    ${SMPS_PROCESS_CONFIGURATION_PATH}/messages_configuration/message_configuration.cpp
    ${SMPS_PROCESS_CONFIGURATION_PATH}/messages_configuration/messages_configuration.cpp
    ${SMPS_PROCESS_CONFIGURATION_PATH}/connection_configuration/connection_configuration.cpp
    ${SMPS_PROCESS_CONFIGURATION_PATH}/connection_configuration/connections_configuration.cpp
    ${SMPS_PROCESS_CONFIGURATION_PATH}/connection_configuration/managed_memory_connection_configuration.cpp
    ${SMPS_PROCESS_CONFIGURATION_PATH}/connection_configuration/socket_connection_configuration.cpp
    ${SMPS_PROCESS_CONFIGURATION_PATH}/process_configuration.cpp
)

#--------------------------------------------------------------------#
###################### PROCESS TASK_PROCESSOR ########################
#--------------------------------------------------------------------#
set(HMPS_TASK_PROCESSOR_PATH include/task_processor)

set(TASK_PROCESSOR
    ${HMPS_TASK_PROCESSOR_PATH}/base_task_runner.hpp
    ${HMPS_TASK_PROCESSOR_PATH}/task_processor.hpp
    ${HMPS_TASK_PROCESSOR_PATH}/task_runner.hpp
    ${HMPS_TASK_PROCESSOR_PATH}/task_wrapper.hpp
)

#-----------------------------------------------------------#
###################### PROCESS RUNNER #######################
#-----------------------------------------------------------#
set(PROCESS_RUNNER_HEADERS
    include/process_runner.hpp
    include/process_traits.hpp
)

set(PROCESS_RUNNER_SOURCES
    src/process_runner.cpp
)

#-----------------------------------------------------------#
###################### PROJECT SOURCES ######################
#-----------------------------------------------------------#
set(MPS_PROCESS_TRAITS_HEADERS
    ${BASE_COMPONENTS_HEADERS}
    ${COMPONENTS_CONTAINER_HEADERS}
    ${PROCESS_INTERFACES}
    ${MESSAGES_TRAITS_HEADERS}
    ${PROCESS_CONFIGURATION_HEADERS}
    ${TASK_PROCESSOR}
    ${PROCESS_RUNNER_HEADERS}
)

set(MPS_PROCESS_TRAITS_SOURCES
    ${BASE_COMPONENTS_SOURCES}
    ${COMPONENTS_CONTAINER_SOURCES}
    ${MESSAGES_TRAITS_SOURCES}
    ${PROCESS_CONFIGURATION_SOURCES}
    ${PROCESS_RUNNER_SOURCES}
)

set(PROJECT_SOURCES
    ${MPS_PROCESS_TRAITS_HEADERS}
    ${MPS_PROCESS_TRAITS_SOURCES}
)

include_directories(include)

set(CMAKE_INSTALL_PREFIX "/usr/local")

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${Boost_LIBRARIES}
        Qt${QT_VERSION_MAJOR}::Core
        pthread
  	boost_thread
        ${mps_common_LIBRARIES}
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
