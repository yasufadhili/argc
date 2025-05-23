# Building Guide {#building}

This guide covers the build process for the Argon compiler in detail.

## Prerequisites

### Required Tools
- CMake 3.15 or higher
- C++23 compatible compiler:
  - GCC 11.0+
  - Clang 13.0+
  - MSVC 19.29+ (Visual Studio 2019 16.9+)
- Java Runtime Environment (for ANTLR4)
- Ninja (recommended) or another CMake generator
- Git

### Optional Tools
- Doxygen (for documentation generation)
- Graphviz (for documentation diagrams)
- Python 3.8+ (for testing tools)

## Build Script

The project includes a convenient build script at `scripts/build.sh` that handles common build scenarios:

```bash
./scripts/build.sh [options]
```

### Build Script Options
- `-h, --help`: Show help message
- `-c, --clean`: Clean build directory before building
- `-t, --build-type`: Build type (Debug|Release|RelWithDebInfo|MinSizeRel)
- `-j, --jobs`: Number of parallel jobs
- `--compiler`: Compiler to use (gcc|clang|msvc)
- `--with-docs`: Build documentation
- `--no-tests`: Skip building tests

### Examples

1. Debug build with GCC:
   ```bash
   ./scripts/build.sh --compiler gcc -t Debug
   ```

2. Release build with Clang and documentation:
   ```bash
   ./scripts/build.sh --compiler clang -t Release --with-docs
   ```

3. Clean rebuild with maximum parallelism:
   ```bash
   ./scripts/build.sh -c -j$(nproc)
   ```

## Manual Build Process

If you prefer not to use the build script, you can build manually:

1. Configure:
   ```bash
   cmake -S . -B build -G Ninja \
     -DCMAKE_BUILD_TYPE=Release \
     -DARGC_BUILD_DOCS=ON
   ```

2. Build:
   ```bash
   cmake --build build
   ```

3. Run tests:
   ```bash
   ctest --test-dir build
   ```

## Platform-Specific Notes

### Linux
- Install build dependencies:
  ```bash
  # Ubuntu/Debian
  sudo apt install build-essential cmake ninja-build
  
  # Fedora
  sudo dnf install gcc-c++ cmake ninja-build
  ```

### Windows
- Install Visual Studio 2019 or newer with C++ workload
- Install CMake and Ninja via Visual Studio Installer
- Use Developer Command Prompt for VS

### macOS
- Install tools via Homebrew:
  ```bash
  brew install cmake ninja llvm
  ```

## Common Issues

### ANTLR4 Issues
- Ensure Java is installed and in PATH
- Verify ANTLR4 jar is present in tools/
- Check generated files in build/generated/

### Compiler Version
- Verify compiler version meets minimum requirements
- Use `--compiler` flag to switch compilers

### Build Performance
- Use `-j` flag to control parallel jobs
- Consider using Ninja generator
- Enable ccache if available