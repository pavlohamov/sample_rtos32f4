cmake_minimum_required(VERSION 3.13)

include(toolchain.cmake NO_POLICY_SCOPE)

if(NOT VERSION)
	set(VERSION 0.0.1)
endif()

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE MINSIZEREL)
	set(CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE} "-Os -ggdb3")
	set(CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE} "-Os -ggdb3")
endif()
# DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL

project(f4rtos VERSION ${VERSION} LANGUAGES C CXX ASM)

if(NOT OUTPUT_NAME)
	set(OUTPUT_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
endif()

string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M:%S UTC" UTC)
configure_file(buildConfig.h.in buildConfig.h)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})



add_compile_definitions(
	STM32F401xE
	HSE_VALUE=25000000
	DEBUG
	TRACE
	OS_INTEGER_TRACE_PRINTF_TMP_ARRAY_SIZE=1024
)

include_directories(${CMAKE_PROJECT_NAME} PRIVATE
	inc
	src

	${PROJECT_BINARY_DIR}
	sdk/
	sdk/cmsis
	sdk/cube
	sdk/cube/Legacy
	sdk/dev
	sdk/system
	sdk/system/include/
	sdk/system/include/arm
	sdk/system/include/cmsis
	sdk/system/include/cortexm
	sdk/system/include/diag
)

add_subdirectory(ThirdParty)


set(CFLAGS "\
	${CMFLAGS} \
	-include buildConfig.h \
	-ggdb3 \
	-ffunction-sections \
	-fdata-sections \
	\
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

set(CFLAGS "\
	${CFLAGS} \
	-mcpu=cortex-m4 \
	-mthumb \
	-mabi=aapcs \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16 \
")

set(CCFLAGS "\
	-Wno-pointer-to-int-cast \
	-Wno-pointer-sign \
	-std=gnu99 \
")

set(CXXFLAGS "\
	-std=c++14 \
	-fno-exceptions \
	-fno-rtti \
	-Wno-conversion-null \
")

set (LDFLAGS "\
	-ggdb3 \
	-Wl,-eReset_Handler \
	-mcpu=cortex-m4 \
	-mthumb \
	-fmessage-length=0 \
	-T mem.ld -T sections.ld \
	-nostartfiles \
	-Xlinker --gc-sections \
	-L\"${CMAKE_SOURCE_DIR}/ldscripts\" \
	-Wl,-g \
	-Wl,-Map,\"${CMAKE_PROJECT_NAME}.map\" \
	--specs=nano.specs \
	-u _printf_float \
")

add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_SIZE} --format=berkeley ${CMAKE_PROJECT_NAME}
	COMMAND ${CMAKE_OBJCOPY} -O ihex ${CMAKE_PROJECT_NAME} ${OUTPUT_NAME}.hex
	COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_PROJECT_NAME} ${OUTPUT_NAME}.bin
	COMMAND ln -fs ${CMAKE_PROJECT_NAME} fw.elf
	VERBATIM
)

add_custom_target(flash
	DEPENDS ${CMAKE_PROJECT_NAME}
	COMMAND openocd -f ../openocd.cfg -c init -c targets -c 'reset halt' -c 'flash write_image erase ${OUTPUT_NAME}.hex' -c 'reset halt' -c 'verify_image ${OUTPUT_NAME}.hex' -c 'reset run' -c shutdown
	USES_TERMINAL
)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS} ${CCFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS} ${CXXFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}")

FILE(GLOB SRC src/*.c*)
FILE(GLOB SDK sdk/cube/src/*.c* sdk/cube/src/Legacy/*.c*)
FILE(GLOB SDK_SYSTEM 
	sdk/system/src/arm/*.c*
	sdk/system/src/cmsis/*.c*
	sdk/system/src/cortexm/*.c*
	sdk/system/src/diag/*.c*
	sdk/system/src/newlib/*.c*
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
	${SRC}
	${SDK}
	${SDK_SYSTEM}
)
