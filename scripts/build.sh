#!/usr/bin/env bash
set -e

# Directory containing this script
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Default values
BUILD_TYPE="Debug"
BUILD_DIR="build"
COMPILER="gcc"
CLEAN=0
DOCS=0
TESTS=1
JOBS=$(nproc)


usage() {
    echo "Usage: $0 [options]"
    echo "Options:"
    echo "  -h, --help             Show this help message"
    echo "  -c, --clean           Clean build directory before building"
    echo "  -t, --build-type      Build type (Debug|Release|RelWithDebInfo|MinSizeRel)"
    echo "  -j, --jobs            Number of parallel jobs (default: number of CPU cores)"
    echo "  --compiler            Compiler to use (gcc|clang|msvc)"
    echo "  --with-docs           Build documentation"
    echo "  --no-tests           Skip building tests"
    exit 1
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            usage
            ;;
        -c|--clean)
            CLEAN=1
            shift
            ;;
        -t|--build-type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        --compiler)
            COMPILER="$2"
            shift 2
            ;;
        --with-docs)
            DOCS=1
            shift
            ;;
        --no-tests)
            TESTS=0
            shift
            ;;
        *)
            echo "Unknown option: $1"
            usage
            ;;
    esac
done


# Validate build type
case $BUILD_TYPE in
    Debug|Release|RelWithDebInfo|MinSizeRel) ;;
    *)
        echo "Invalid build type: $BUILD_TYPE"
        usage
        ;;
esac


# Clean build directory if requested
if [ $CLEAN -eq 1 ]; then
    echo "Cleaning build directory..."
    rm -rf "$PROJECT_ROOT/$BUILD_DIR"
fi


# Create build directory
mkdir -p "$PROJECT_ROOT/$BUILD_DIR"


# Configure toolchain file based on compiler
TOOLCHAIN_ARG=""
case $COMPILER in
    gcc)
        TOOLCHAIN_ARG="-DCMAKE_TOOLCHAIN_FILE=$PROJECT_ROOT/cmake/toolchains/gcc-toolchain.cmake"
        ;;
    clang)
        TOOLCHAIN_ARG="-DCMAKE_TOOLCHAIN_FILE=$PROJECT_ROOT/cmake/toolchains/clang-toolchain.cmake"
        ;;
    msvc)
        # MSVC doesn't need a toolchain file
        ;;
    *)
        echo "Invalid compiler: $COMPILER"
        usage
        ;;
esac


# Configure CMake
echo "Configuring CMake..."
cmake -S "$PROJECT_ROOT" -B "$PROJECT_ROOT/$BUILD_DIR" \
    -G Ninja \
    $TOOLCHAIN_ARG \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DARGC_BUILD_DOCS=$DOCS \
    -DARGC_BUILD_TESTS=$TESTS

# Build
echo "Building..."
cmake --build "$PROJECT_ROOT/$BUILD_DIR" --parallel $JOBS

# Run tests if enabled
if [ $TESTS -eq 1 ]; then
    echo "Running tests..."
    ctest --test-dir "$PROJECT_ROOT/$BUILD_DIR" --output-on-failure
fi

# Build documentation if enabled
if [ $DOCS -eq 1 ]; then
    echo "Building documentation..."
    cmake --build "$PROJECT_ROOT/$BUILD_DIR" --target docs
fi

echo "Build completed successfully!"
