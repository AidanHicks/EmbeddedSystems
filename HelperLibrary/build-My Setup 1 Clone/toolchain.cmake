set(CORE_NAME "P18" CACHE STRING "" FORCE)
set(HAS_MIKROBUS "true" CACHE STRING "" FORCE)
set(MCU_NAME "PIC18F45K22" CACHE STRING "" FORCE)
set(XC8_SUPPORTED "TRUE" CACHE STRING "" FORCE)
set(_MSDK_BOARD_NAME_ "BOARD_EASYPIC_V7" CACHE STRING "" FORCE)
set(_MSDK_DIP_SOCKET_TYPE_ "DIP40" CACHE STRING "" FORCE)
set(_MSDK_ETH_PHY_CHIP_ "NULL" CACHE STRING "" FORCE)
set(_MSDK_HAL_LOW_LEVEL_TARGET_ "mikroe" CACHE STRING "" FORCE)
set(_MSDK_PACKAGE_ID_ "" CACHE STRING "" FORCE)
set(_MSDK_PACKAGE_NAME_ "" CACHE STRING "" FORCE)
set(_MSDK_PACKAGE_PIN_COUNT_ "40" CACHE STRING "" FORCE)
set(TOOLCHAIN_ID "mikrocpic" CACHE STRING "" FORCE)
set(OSC "32" CACHE STRING "" FORCE)


set(COMPILER_FLAGS -C -MF -O11111114   "-DBG")
set(LINKER_FLAGS -C -MF -O11111114   "-DBG")

string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)
if (build_type STREQUAL debug)
    list(APPEND COMPILER_FLAGS "-UICD")
    list(APPEND LINKER_FLAGS "-UICD")
endif()

file(TO_CMAKE_PATH "C:/users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/.meproject/setup/Debug/PIC18F45K22.jcfg" JCFG_FILE_VAL)
set(JCFG_FILE  ${JCFG_FILE_VAL} CACHE STRING "" FORCE)

file(TO_CMAKE_PATH "C:/users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/.meproject/setup/Debug/lib/lib_core.a" CORE_LIB_VAL)
set(CORE_LIB ${CORE_LIB_VAL} CACHE STRING "" FORCE)

set(SEARCH_PATHS "C:/users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/build-My Setup 1 Clone;C:/Users/767905/AppData/Local/MIKROE/NECTOStudio7/packages/core/PIC/mikroC/pic_mikroc_pic18/def;C:/users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary" CACHE STRING "" FORCE)

set(CMAKE_MikroC_COMPILER "C:/Users/767905/AppData/Local/MIKROE/NECTOStudio7/packages/compilers/mikroc/pic/mikroc/mikroCPIC1618.exe" CACHE INTERNAL "")
set(CMAKE_MikroC_COMPILER_ID "mikroC" CACHE INTERNAL "")

set(CMAKE_MikroC_OUTPUT_EXTENSION_REPLACE 1)
set(CMAKE_MikroC_OUTPUT_EXTENSION ".mcl")

# set CMAKE_SYSTEM_NAME to define build as CMAKE_CROSSCOMPILING
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

SET(CMAKE_MikroC_COMPILER_WORKS TRUE CACHE INTERNAL "")
SET(CMAKE_MikroC_COMPILER_FORCED TRUE CACHE INTERNAL "")
SET(CMAKE_MikroC_COMPILER_ID_RUN TRUE CACHE INTERNAL "")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# add compiler option flags
add_compile_options(${COMPILER_FLAGS})
message(INFO ": ${COMPILER_FLAGS}")
# add link option flags
add_link_options(${LINKER_FLAGS})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

file(TO_CMAKE_PATH "C:/users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/.meproject/setup/Debug/lib/cmake" MIKROSDK_PATH)
file(TO_CMAKE_PATH "C:/users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/.meproject/setup/Debug/lib/cmake" MIKROC_CORE_PATH)


set(MIKROSDK_ROOT_PATH ${MIKROSDK_PATH})
set(MIKROC_CORE_ROOT_PATH ${MIKROC_CORE_PATH})
#append to cmake_prefix_path
list(APPEND CMAKE_PREFIX_PATH ${MIKROSDK_ROOT_PATH})
list(APPEND CMAKE_PREFIX_PATH ${MIKROC_CORE_ROOT_PATH})

list(APPEND CMAKE_MODULE_PATH "C:/Users/767905/AppData/Local/MIKROE/NECTOStudio7/cmake;C:/Users/767905/AppData/Local/MIKROE/NECTOStudio7/packages/core/PIC/mikroC/pic_mikroc_pic18/cmake;C:/Users/767905/AppData/Local/MIKROE/NECTOStudio7/packages/mikroe_utils_common;C:/users/767905/OneDrive - hull.ac.uk/MIKROE/Projects/HelperLibrary/.meproject/setup/Debug/lib/cmake;")


