cmake_minimum_required(VERSION 3.13)

set(CMAKE_BUILD_TYPE MINSIZEREL)
set(CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE} "-Os -ggdb")
set(CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE} "-Os -ggdb")

add_subdirectory(FreeRTOS-Kernel)
target_link_libraries(${CMAKE_PROJECT_NAME}
    FreeRTOS-Kernel
)

