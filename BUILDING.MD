# Building the Argon Compiler

The compiler uses CMake (with Ninja) and C++23. It integrates [ANTLR4](https://github.com/antlr/antlr4) as a submodule for grammar-based code generation.

## Prerequisites

- CMake >= 3.15
- Ninja (or another supported generator)
- C++23-compatible compiler (e.g., MSVC, GCC, Clang)
- Java (required for ANTLR4 code generation)
- Git

## Initial Setup

Before building, initialise and update the ANTLR4 submodule:

```bash
git submodule update --init --recursive
```

## Build Instructions

1. **Clone the repository (if you haven't already):**
    ```sh
    git clone https://github.com.yasufadhili/argc.git
    cd argc
    ```

2. **Ensure submodules are initialised:**
    ```sh
    git submodule update --init --recursive
    ```

3. **Configure the project with CMake:**
    ```sh
    cmake -S . -B build -G Ninja
    ```

4. **Build the project:**
    ```sh
    cmake --build build
    ```

   This will:
   - Build the ANTLR4 C++ runtime from the submodule.
   - Generate parser sources from `.g4` grammar files using ANTLR4 (requires Java).
   - Build the main project sources.

## Regenerating ANTLR Sources

To force regeneration of ANTLR4 sources (e.g., after editing grammar files):

```sh
cmake --build build --target generate_antlr
```

## Notes

- The ANTLR4 tool JAR is expected at `tools/antlr-4.13.2-complete.jar`.
- Grammar files should be placed in the `grammar/` directory.
- Generated sources are placed in the build directory under `generated/`.

## Testing

The project uses [GoogleTest (GTest)](https://github.com/google/googletest) for unit testing.

---