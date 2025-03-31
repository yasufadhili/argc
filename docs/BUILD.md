# Argon Build Documentation

This document provides detailed information about the build system for the Argon compiler.

## CMake Build System

The Argon project uses CMake as its build system generator. CMake is a cross-platform build system that can generate build files for various platforms and build tools.

### Build System Structure

The build system is organised as follows:

- `CMakeLists.txt` (root): Main configuration file that sets up the project, compiler flags, and includes subdirectories
- `src/CMakeLists.txt`: Configures the main executable target and its sources
- `examples/CMakeLists.txt`: Sets up example files and targets to run them

### Build Targets

The following targets are available:

- `argon`: The main compiler executable
- `run_examples`: A custom target to run all example Argon programs
- `run_example_<name>`: Custom targets to run specific examples (e.g., `run_example_3_var`)

### Build Directories

When building the project, the following directories are created:

- `build/bin`: Contains the compiled executable
- `build/lib`: Contains any libraries (coming soon)
- `build/examples`: Contains copies of example files

## Compiler Flags

The build system sets the following compiler flags:

- `-Wall -Wextra -Wpedantic`: Enable comprehensive warnings (GCC/Clang)
- `/W4`: Enable comprehensive warnings (MSVC)
- Debug mode: `-g -O0` (GCC/Clang)
- Release mode: `-O3` (GCC/Clang)

## Dependencies

Currently, the Argon compiler has no external dependencies beyond the C++ standard library.

## Build Types

The following build types are supported:

- `Debug` (default): Includes debug symbols and minimal optimization
- `Release`: Optimized for performance
- `RelWithDebInfo`: Optimized with debug information
- `MinSizeRel`: Optimized for size

To specify a build type:

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Installation

The build system includes installation targets that will install:

- The `argon` executable to `${CMAKE_INSTALL_BINDIR}` (typically `/usr/local/bin` on Unix systems)
- Documentation to `${CMAKE_INSTALL_DOCDIR}` (typically `/usr/local/share/doc/argon` on Unix systems)

To install:

```bash
sudo make install
```

## Packaging

The build system includes CPack configuration for creating packages:

- Tarball (TGZ)
- Debian package (DEB)

To create packages:

```bash
make package
```

## Troubleshooting

### Common Issues

1. **CMake version too old**
   
   If you encounter an error about the CMake version being too old, update CMake or modify the `cmake_minimum_required` version in the root CMakeLists.txt.

2. **Compiler doesn't support C++23**
   
   Ensure you have a C++23 compatible compiler. For GCC, version 11 or higher is recommended.

3. **Build fails with "file not found" errors**
   
   Check that all source files listed in the CMakeLists.txt files exist at the specified paths.

### Debugging the Build

To see detailed output during the build process:

```bash
make VERBOSE=1
```

To clean the build directory and start fresh:

```bash
rm -rf build
mkdir build
cd build
cmake ..
make
```
