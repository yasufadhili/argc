
# 🛠️ Argon Compiler (argc)

Welcome to the **Argon Compiler**—the beating heart of the Argon programming language! ❤️ This is where your Argon code gets transformed into x86_64 assembly, ready to run on Linux systems. If you’re curious about how compilers work or want to tinker with Argon’s internals, you’re in the right place.

## 🌟 What is `argc`?

`argc` is the official compiler for the Argon programming language. It takes your `.ar` source files, parses them, performs semantic analysis, and generates x86_64 assembly code. Think of it as the magical translator that turns your high-level ideas into low-level machine instructions. ✨

## 📂 What’s Inside?

Here’s a quick overview of the `argc` directory:

- **`src/`**: The source code for the compiler, written in C++ (with a sprinkle of C).  
  
- **`include/`**: Header files for the compiler’s various components.  
  
- **`tests/`**: Unit tests and example programs to ensure the compiler works as expected.  
  
- **`Makefile`**: Build instructions for compiling the `argc` binary.  
  
- **`README.md`**: (You’re reading it!) Documentation for the Argon Compiler.  

## 🚀 Features of `argc`

- 🛠️ **Modular Design**: Supports modular programming with Argon’s module system.  
  
- 📜 **Custom Syntax**: Parses Argon’s unique (and occasionally quirky) syntax.  
- 🖥️ **x86_64 Assembly Output**: Generates assembly code that runs on Linux systems.  
- 🔗 **C/C++ Interoperability**: Link Argon modules with C and C++ static libraries.  
- 🧪 **Experimental Features**: A playground for trying out new ideas in language design.  

## 🛠️ Building the Compiler

To build the Argon Compiler, follow these steps:

1. **Clone the Repository**:  
   
   ```bash
   git clone https://github.com/yasufadhili/argon.git
   cd argon/argc
   ```

2. **Build the Compiler**:  
   
   Use the provided `Makefile` to compile the `argc` binary:  
   ```bash
   make
   ```

3. **Run the Compiler**:  
   
   Once built, you can use the `argc` binary to compile Argon source files:  
   ```bash
   ./argc <source-file>
   ```

## 💡 Example Usage

Here’s a simple example to get you started:

### Input (`hello.ar`)
```c++
module main

def main() {
    sys::print("Hello, Argon!")
}
```

### Compile and Run
```bash
./argc hello.ar
nasm -f elf64 hello.asm -o hello.o
ld hello.o -o hello
./hello
```

And voilà! Your Argon program is now running. 🎉

## 🧩 How `argc` Works

Here’s a high-level overview of the compilation process:

1. **Lexical Analysis**: Breaks the source code into tokens.  
2. **Parsing**: Converts tokens into an Abstract Syntax Tree (AST).  
3. **Semantic Analysis**: Ensures the code follows Argon’s rules (e.g., type checking).  
4. **Code Generation**: Outputs x86_64 assembly code.  

If you’re interested in the nitty-gritty details, check out the `src/` directory for the implementation of each stage.

## 🧑‍💻 Contributing to `argc`

Want to improve the Argon Compiler? Awesome! Here’s how you can contribute:

1. Fork the repository.  
   
2. Make your changes in the `argc/` directory.  
3. Write tests for your changes (please!).  
4. Submit a pull request with a clear description of your updates.  

## 🌟 Future Plans

Here are some features I’d love to add to `argc`:

- 🔄 **Better Optimisation**: Improve the generated assembly for faster execution.  
  
- 🧪 **More Tests**: Expand the test suite to cover edge cases.  
- 🌐 **Cross-Platform Support**: Extend support beyond Linux systems (Just kidding).  
- 🛡️ **Error Reporting**: Make compiler errors more user-friendly.  

## 📚 Resources

If you’re new to compilers or want to learn more, here are some great resources:

- **[Crafting Interpreters](https://craftinginterpreters.com/)**: A beginner-friendly guide to building languages.  
- **[The Dragon Book](https://en.wikipedia.org/wiki/Compilers:_Principles,_Techniques,_and_Tools)**: The classic textbook on compilers.  
- **[x86 Assembly Guide](https://cs.lmu.edu/~ray/notes/x86assembly/)**: Learn the assembly language Argon compiles to.  

## 🌈 Join the Community

The Argon Compiler is a labour of love, and I’d love to have you join on this journey. Whether you’re fixing bugs, adding features, or just exploring, I'm glad to have you here. Happy coding! 🎉

---

**Argon Compiler: Because every language needs a heart to beat.** ❤️✨
