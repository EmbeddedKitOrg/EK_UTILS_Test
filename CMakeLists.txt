cmake_minimum_required(VERSION 3.20)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/gd32f470zgt6_flash.ld)

project(ek_utils_test LANGUAGES C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ek_utils 子模块
set(EK_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc/ek_conf.h" CACHE STRING "Path to ek_conf.h" FORCE)
add_subdirectory(ek_utils)

# GD32头文件
set(GD_Include_Dir
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Driver/CMSIS/Device/GD/GD32F4xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Driver/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Bsp/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Test/Inc
)

# 收集源文件
file(GLOB GD_Start_S ${CMAKE_CURRENT_SOURCE_DIR}/*.s)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Core/Src GD_Core_Src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Driver/Src GD_Std_Src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Driver/CMSIS/Device/GD/GD32F4xx/Source GD_CMSIS_Src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Bsp/Src GD_Bsp_Src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Test/Src GD_Test_Src)

# 创建可执行文件目标
add_executable(${CMAKE_PROJECT_NAME}
    ${GD_Start_S}
    ${GD_Core_Src}
    ${GD_Std_Src}
    ${GD_CMSIS_Src}
    ${GD_Bsp_Src}
    ${GD_Test_Src}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${GD_Include_Dir}
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    GD32F470
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -T ${LINKER_SCRIPT}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ek_utils)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -fdiagnostics-color=always
    -Wall
    -Wextra
)
