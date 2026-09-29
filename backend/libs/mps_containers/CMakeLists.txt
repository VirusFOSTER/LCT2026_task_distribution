cmake_minimum_required(VERSION 3.22)

project(mps_containers VERSION 1.0 LANGUAGES CXX)
set(MAIN_PROJECT mps)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} INTERFACE)

include_directories(include)

set(MPS_MEMORY_BUFFERS_HEADERS
  include/memory/list_memory_buffer.hpp
  include/memory/tp_list_element.hpp
)

set(MPS_CONTAINER_LIST_HEADERS
    include/list/tp_list_element.hpp
    include/list/list_buffer.hpp
)

set(MPS_CONTAINER_AVL_TREE_HEADERS
    include/avl_tree/tp_tree_element.hpp
    include/avl_tree/avl_tree_buffer.hpp
)

set(MPS_CONTAINERS_HEADERS
    ${MPS_MEMORY_BUFFERS_HEADERS}
    ${MPS_CONTAINER_LIST_HEADERS}
    ${MPS_CONTAINER_AVL_TREE_HEADERS}
)

set(CMAKE_INSTALL_PREFIX "/usr/local")

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
