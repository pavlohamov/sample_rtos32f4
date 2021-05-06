cmake_minimum_required(VERSION 3.13)

add_library(FreeRTOS-Kernel STATIC)

set(CFLAGS "\
    -ggdb \
    -mcpu=cortex-m4 \
    -mthumb \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16 \
    -ffunction-sections \
    -fdata-sections \
    
    -std=c99 \
    \
    -Wno-pointer-to-int-cast \
    -Wno-pointer-sign \
    -Wall \
    -Werror \
    -Wfatal-errors \
    \
    -Wno-sign-compare \
    -Wno-unused-variable \
    -Wno-unused-but-set-variable \
    -Wno-parentheses \
    -Wno-format \
")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS} ${CCFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS} ${CXXFLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} ${LDFLAGS}")


target_include_directories(FreeRTOS-Kernel PRIVATE
    include
    portable/${TOOL_U}/${ARCH}
)

target_include_directories(FreeRTOS-Kernel INTERFACE
    include
    portable/${TOOL_U}/${ARCH}

    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(FreeRTOS-Kernel PRIVATE
    #   portable/Common/mpu_wrappers.c
    portable/MemMang/heap_4.c
    portable/${TOOL_U}/${ARCH}/port.c
    croutine.c
    event_groups.c
    list.c
    queue.c
    stream_buffer.c
    tasks.c
    timers.c
)

