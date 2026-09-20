cmake_minimum_required(VERSION 3.25)

project(mps_containers VERSION 1.0 LANGUAGES CXX)
set(MAIN_PROJECT mps)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} INTERFACE)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

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

target_sources(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${MPS_CONTAINERS_HEADERS}>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<INSTALL_INTERFACE:${include_install_dir}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(DIRECTORY include/ DESTINATION include/${MAIN_PROJECT}/${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Config)
install(EXPORT ${PROJECT_NAME}Config DESTINATION share/${MAIN_PROJECT}/${PROJECT_NAME}/cmake)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION
        ${PROJECT_VERSION}
    COMPATIBILITY
        AnyNewerVersion
)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" DESTINATION share/${MAIN_PROJECT}/${PROJECT_NAME}/cmake)
