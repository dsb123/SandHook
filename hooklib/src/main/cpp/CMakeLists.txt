cmake_minimum_required(VERSION 3.4.1)
project(sandhook)

ENABLE_LANGUAGE(ASM)

set(${PROJECT_NAME}_SOURCES
        sandhook.cpp
        trampoline/trampoline.cpp
        trampoline/trampoline_manager.cpp
        utils/dlfcn_nougat.cpp
        utils/hide_api.cpp
        utils/utils.cpp
        utils/offset.cpp
        utils/elf_util.cpp
        casts/cast_art_method.cpp
        casts/cast_compiler_options.cpp
        art/art_method.cpp
        art/art_compiler_options.cpp
        trampoline/arch/arm32.S
        trampoline/arch/arm64.S
        inst/insts_arm32.cpp
        inst/insts_arm64.cpp
        nativehook/native_hook.cpp
        )

add_definitions(-std=c++11)

add_library(${PROJECT_NAME}
        SHARED
        ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} log)