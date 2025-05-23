
if(WIN32)
    set(ARGC_PLATFORM_WINDOWS ON)
elseif(UNIX AND NOT APPLE)
    set(ARGC_PLATFORM_LINUX ON)
elseif(APPLE)
    set(ARGC_PLATFORM_MACOS ON)
endif()



if(MSVC)
    set(ARGC_COMPILER_MSVC ON)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(ARGC_COMPILER_GCC ON)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(ARGC_COMPILER_CLANG ON)
endif()



if(ARGC_PLATFORM_WINDOWS)
    add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)
endif()



configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/config.h
    @ONLY
)


