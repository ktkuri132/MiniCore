
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/CORE)
include_directories(${CMAKE_SOURCE_DIR}/BSP)
include_directories(${CMAKE_SOURCE_DIR}/HARDWARE)


# 得到当前文件名
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_library(${DIR_NAME}_LIB ${${DIR_NAME}_Source})

file(GLOB_RECURSE ${DIR_NAME}_Source 
    ./*.c
    command_system/*.c
    Storage_components/*.c
    )

target_link_libraries(${DIR_NAME}_LIB BSP_LIB CORE_LIB HARDWARE_LIB)