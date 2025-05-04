
---

#  Building the Argon Compiler (`argc`)

## Requirements

Before you begin, ensure you have the following dependencies installed on your system:

- **C++ Compiler**: `g++` with C++23 support
- **Flex**: For lexical analysis (`flex`)
- **Bison**: For parser generation (`bison`)
- **GNU Make**: For build automation (`make`)
- **bear**: For generating `compile_commands.json` (useful for code analysis tools)
- **GAS**: For assembling the generated x86_64 assembly to run Argon programs (pre-installed on Linux)
- **ld**: The GNU linker to link assembled object files (Also pre-installed on Linux)
- **Linux**: The compiler targets x86_64 Linux systems


## Cloning the Repository

Clone the main Argon repository and navigate to the `argc` directory:

```bash
git clone https://github.com/yasufadhili/argon.git
cd argon/argc
```

**All build commands below should be run from within the `argc` directory.**

## Building with `bear` and Make

The project uses `bear` to generate a `compile_commands.json` file, which is helpful for code analysis tools and IDE integration. `bear` wraps the build process and records all compilation commands.

### To build the compiler and generate `compile_commands.json`:

```bash
bear -- make
```

This will:
- Generate the parser and lexer using Bison and Flex
- Compile all C++ source files
- Link the final binary to `build/bin/argc`
- Create or update `compile_commands.json` in the directory

### To install the compiler system-wide (optional):

```bash
sudo bear -- make install
```

This will copy the `argc` binary to `/usr/local/bin/argc` (or another prefix if you set `INSTALL_PREFIX`).

### To uninstall:

```bash
sudo make uninstall
```

This removes the installed binary from your system.

### To clean build artefacts:

```bash
make clean
```
Removes all object files, generated sources, and the binary, but keeps build directories.

### To remove all build directories and generated files:

```bash
make distclean
```
This is a more thorough clean, removing everything produced by the build.

### To rebuild from scratch:

```bash
make rebuild
```
Equivalent to running `make clean` followed by `make`.

### To generate dependency information:

```bash
make depend
```
Creates a `.depend` file with header dependencies for the project.

### To run the compiler (after building):

```bash
make run
```
This will execute the built `argc` binary.

## Example: Compiling and Running an Argon Program

Suppose you have a file called `hello.ar`:

```argon
module main;

fn main {

  ret;
}
```

Compile it with:

```bash
./build/bin/argc hello.ar
```

This will produce `hello.asm`. Assemble and link it:

```bash
as -o hello.o hello.asm 
ld hello.o -o hello
./hello
```

## Directory Structure

- `src/`: C++ source files, including the main compiler logic, lexer, and parser
- `include/`: Header files for the compiler
- `build/`: Build artefacts (object files, binaries)
- `tests/`: Unit tests and example Argon programs
- `Makefile`: Build instructions
- `BUILDING.md`: This documentation
- `README.md` : Project readme

## Notes

- Always run build commands from the `argc` directory.
- If you modify the parser or lexer definitions (`parser.yy` or `lexer.l`), a rebuild will regenerate the corresponding C++ files.
- The `bear` tool is only needed for generating `compile_commands.json`. If you do not need this file, you can use `make` directly.

## Troubleshooting

- If you encounter missing dependencies, ensure all requirements are installed.
- If you see permission errors during install/uninstall, use `sudo`.
- For issues with the build, try `make distclean` and rebuild.

---
