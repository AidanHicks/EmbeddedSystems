cmake_minimum_required(VERSION 3.19)

if (${TOOLCHAIN_LANGUAGE} MATCHES "MikroC")
    project(HelperLibrary LANGUAGES MikroC)
else()
    project(HelperLibrary LANGUAGES C ASM)
endif()

set(PROJECT_TYPE "mikrosdk" CACHE STRING "" FORCE)

add_library(HelperLibrary
STATIC
led.c
button.c
lib.h
seven_seg.c
)
############################ HelperLibrary GENERATED CODE START ###########################
############################ HelperLibrary GENERATED CODE END ###########################

target_link_libraries(HelperLibrary
    PUBLIC
        MikroC.Core
)

target_include_directories(HelperLibrary PUBLIC Headers)

if (${MIKROSDK_TYPE} STREQUAL "legacy")
    target_link_libraries(HelperLibrary
        PUBLIC
            MikroC.System
    )
endif()
