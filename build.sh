#!/bin/bash

# Build script for Argon compiler
# This script simplifies the build process and provides common operations

set -e  # Exit on error

# Default build type
BUILD_TYPE="Debug"
BUILD_DIR="build"
CLEAN=0
RUN_EXAMPLES=0
INSTALL=0
PACKAGE=0

# Parse command line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    --release)
      BUILD_TYPE="Release"
      shift
      ;;
    --clean)
      CLEAN=1
      shift
      ;;
    --examples)
      RUN_EXAMPLES=1
      shift
      ;;
    --install)
      INSTALL=1
      shift
      ;;
    --package)
      PACKAGE=1
      shift
      ;;
    --build-dir)
      BUILD_DIR="$2"
      shift 2
      ;;
    --help)
      echo "Usage: $0 [options]"
      echo "Options:"
      echo "  --release       Build in Release mode (default is Debug)"
      echo "  --clean         Clean build directory before building"
      echo "  --examples      Build and run examples after building"
      echo "  --install       Install after building"
      echo "  --package       Create packages after building"
      echo "  --build-dir DIR Set build directory (default is 'build')"
      echo "  --help          Show this help message"
      exit 0
      ;;
    *)
      echo "Unknown option: $1"
      echo "Use --help for usage information"
      exit 1
      ;;
  esac
done

# Clean build directory if requested
if [ $CLEAN -eq 1 ]; then
  echo "Cleaning build directory: $BUILD_DIR"
  rm -rf "$BUILD_DIR"
fi

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
  echo "Creating build directory: $BUILD_DIR"
  mkdir -p "$BUILD_DIR"
fi

# Configure and build
echo "Configuring Argon with build type: $BUILD_TYPE"
cd "$BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..

echo "Building Argon"
cmake --build . -- -j$(nproc)

# Run examples if requested
if [ $RUN_EXAMPLES -eq 1 ]; then
  echo "Running examples"
  make run_examples
fi

# Install if requested
if [ $INSTALL -eq 1 ]; then
  echo "Installing Argon"
  sudo make install
fi

# Create packages if requested
if [ $PACKAGE -eq 1 ]; then
  echo "Creating packages"
  make package
fi

echo "Build completed successfully!"
echo "Executable location: $BUILD_DIR/bin/argon"
