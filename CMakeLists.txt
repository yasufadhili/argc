cmake_minimum_required(VERSION 3.20)
project(
    argon
    VERSION 0.0.1
    DESCRIPTION "Compiler for Argon Programming Language"
    HOMEPAGE_URL "https://github.com/yasufadhili/argon"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
    message(STATUS "Setting build type to Debug as none was specified.")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Options
option(ARGON_BUILD_TESTS "Build tests" OFF)
option(ARGON_BUILD_EXAMPLES "Build examples" ON)
option(ARGON_INSTALL "Generate installation target" ON)

# Dependencies


# Main source code
add_subdirectory(src)

# Examples
if(ARGON_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests (if we add them in the future)
if(ARGON_BUILD_TESTS)
    enable_testing()
    # add_subdirectory(tests)
endif()

# Installation
if(ARGON_INSTALL)
    include(GNUInstallDirs)
    
    install(TARGETS argon
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    
    # Install headers if we decide to make this a library in the future
    # install(DIRECTORY src/
    #     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/argon
    #     FILES_MATCHING PATTERN "*.h*"
    # )
    
    # Install documentation
    install(FILES README.md LICENSE
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )
endif()

# Package generation
set(CPACK_PACKAGE_NAME "argon")
set(CPACK_PACKAGE_VENDOR "Yasu Fadhili")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Compiler for Argon Programming Language")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "yasufadhili@gmail.com")
set(CPACK_GENERATOR "TGZ;DEB")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")

include(CPack)
