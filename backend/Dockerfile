# ==========================================
# ЭТАП 1: Сборка (Builder)
# ==========================================
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Устанавливаем системные зависимости для сборки
RUN sed -i 's|http://archive.ubuntu.com/ubuntu|http://mirror.yandex.ru/ubuntu|g' /etc/apt/sources.list && \
    sed -i 's|http://security.ubuntu.com/ubuntu|http://mirror.yandex.ru/ubuntu|g' /etc/apt/sources.list && \
    echo 'Acquire::Retries "5";' > /etc/apt/apt.conf.d/80-retries && \
    apt-get update && apt-get install -y --fix-missing \
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    ca-certificates \
    qt6-base-dev \
    libgl1-mesa-dev \
    libboost-all-dev \
    libssl-dev \
    libsqlite3-dev \
    libzmq3-dev \
    libglpk-dev \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Копируем ВСЕ файлы проекта (включая libs/vroom и libs/mps_*)
COPY . .

RUN rm -rf libs/mps_common/build \
            libs/mps_container/build \
            libs/mps_process_traits/build \
            libs/vroom/build \
            build

# ==========================================
# Сборка и установка локальных библиотек mps
# ==========================================
# Создаем папку для установки локальных библиотек
RUN mkdir -p /app/install

# Удаляем find_package для mps и добавляем add_subdirectory
RUN sed -i '/find_package(mps_common/d' CMakeLists.txt && \
    sed -i '/find_package(mps_containers/d' CMakeLists.txt && \
    sed -i '/find_package(mps_process_traits/d' CMakeLists.txt && \
    sed -i '/find_package(mps)/d' CMakeLists.txt && \
    echo 'add_subdirectory(libs/mps_common)' >> CMakeLists.txt && \
    echo 'add_subdirectory(libs/mps_containers)' >> CMakeLists.txt && \
    echo 'add_subdirectory(libs/mps_process_traits)' >> CMakeLists.txt

# Собираем mps_common
#RUN cmake -B libs/mps_common/build -S libs/mps_common \
#    -DCMAKE_BUILD_TYPE=Release \
#    -DCMAKE_INSTALL_PREFIX=/app/install && \
#    cmake --build libs/mps_common/build -j$(nproc) && \
#    cmake --install libs/mps_common/build

# Собираем mps_containers
#RUN cmake -B libs/mps_containers/build -S libs/mps_containers \
#    -DCMAKE_BUILD_TYPE=Release \
#    -DCMAKE_PREFIX_PATH=/app/install \
#    -DCMAKE_INSTALL_PREFIX=/app/install && \
#    cmake --build libs/mps_containers/build -j$(nproc) && \
#    cmake --install libs/mps_containers/build

# Собираем mps_process_traits
#RUN cmake -B libs/mps_process_traits/build -S libs/mps_process_traits \
#    -DCMAKE_BUILD_TYPE=Release \
#    -DCMAKE_PREFIX_PATH=/app/install \
#    -DCMAKE_INSTALL_PREFIX=/app/install && \
#    cmake --build libs/mps_process_traits/build -j$(nproc) && \
#    cmake --install libs/mps_process_traits/build

# ==========================================
# Создаем файлы конфигурации для find_package
# ==========================================
# Это нужно, потому что сами библиотеки mps не генерируют *Config.cmake файлы
RUN mkdir -p /app/install/lib/cmake/mps_common && \
    cat << 'EOF' > /app/install/lib/cmake/mps_common/mps_commonConfig.cmake
if(NOT TARGET mps::mps_common)
    add_library(mps::mps_common STATIC IMPORTED)
    set_target_properties(mps::mps_common PROPERTIES
        IMPORTED_LOCATION "/app/install/lib/libmps_common.a"
        INTERFACE_INCLUDE_DIRECTORIES "/app/install/include"
    )
endif()
EOF

RUN mkdir -p /app/install/lib/cmake/mps_containers && \
    cat << 'EOF' > /app/install/lib/cmake/mps_containers/mps_containersConfig.cmake
if(NOT TARGET mps::mps_containers)
    add_library(mps::mps_containers STATIC IMPORTED)
    set_target_properties(mps::mps_containers PROPERTIES
        IMPORTED_LOCATION "/app/install/lib/libmps_containers.a"
        INTERFACE_INCLUDE_DIRECTORIES "/app/install/include"
    )
endif()
EOF

RUN mkdir -p /app/install/lib/cmake/mps_process_traits && \
    cat << 'EOF' > /app/install/lib/cmake/mps_process_traits/mps_process_traitsConfig.cmake
if(NOT TARGET mps::mps_process_traits)
    add_library(mps::mps_process_traits STATIC IMPORTED)
    set_target_properties(mps::mps_process_traits PROPERTIES
        IMPORTED_LOCATION "/app/install/lib/libmps_process_traits.a"
        INTERFACE_INCLUDE_DIRECTORIES "/app/install/include"
    )
endif()
EOF

# ==========================================
# Сборка VROOM
# ==========================================
RUN cd libs/vroom/src && make USE_ROUTING=true

# Создаем симлинк, чтобы компилятор нашел mps по пути mps/...
RUN mkdir -p /app/include && ln -s /app/libs /app/include/mps

RUN ln -s /app/libs /app/mps

# ==========================================
# Сборка основного проекта
# ==========================================
# Обратите внимание: мы передаем CMAKE_PREFIX_PATH=/app/install,
# чтобы find_package мог найти mps библиотеки.
# Замените task_distribution на реальное имя вашего бинарника из add_executable
RUN cmake -B build -S . -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="/app/install" \
    -DVROOM_ROOT=/app/libs/vroom \
    -DCMAKE_CXX_FLAGS="-I/app/libs" && \
    cmake --build build

# ==========================================
# ЭТАП 2: Финальный образ (Runtime)
# ==========================================
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Устанавливаем только runtime-зависимости
RUN apt-get update && apt-get install -y \
    libqt6core6 \
    libqt6widgets6 \
    libboost-system1.74.0 \
    libboost-filesystem1.74.0 \
    libboost-program-options1.74.0 \
    libboost-thread1.74.0 \
    libssl3 \
    libsqlite3-0 \
    libzmq5 \
    libglpk40 \
    libgl1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Копируем собранный исполняемый файл
# Замените task_distribution на имя вашего бинарника
COPY --from=builder /app/build/task_distribution .

# Точка входа
CMD ["./task_distribution"]
