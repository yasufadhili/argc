
#  Argon Compiler (argc)

Welcome to the **Argon Compiler**—the beating heart of the Argon programming language! ❤️ This is where your Argon code gets transformed to native executables, ready to run on Linux systems. If you’re curious about how compilers work or want to tinker with Argon’s internals, you’re in the right place.

## 🌟 What is `argc`?

`argc` is the compiler for Argon. It takes `.ar` source files, parses them, performs semantic analysis, and generates object files. 

## 📂 What’s Inside?

Here’s a quick overview of the `argc` directory:

- **`src/`**: The source code for the compiler, written in C++
  
- **`include/`**: Header files for the compiler’s various components.  
  
- **`Makefile`**: Build instructions for compiling the `argc` binary.  
  
- **`README.md`**: (You’re reading it!) Documentation for the Argon Compiler.  

##  Features of `argc`

- 🛠️ **Modular Design**: Supports modular programming with Argon’s module system.  
  
- 📜 **Custom Syntax**: Parses Argon’s unique (and occasionally quirky) syntax.  
- 🖥️ **x86_64 Assembly Output**: Generates assembly code that runs on Linux systems.  
- 🔗 **C/C++ Interoperability**: Link Argon modules with C and C++ static libraries.  
- 🧪 **Experimental Features**: A playground for trying out new ideas in language design.  

## 🧩 How `argc` Works

Here’s a high-level overview of the compilation process:

1. **Lexical Analysis**: Breaks the source code into tokens.  
2. **Parsing**: Converts tokens into an Abstract Syntax Tree (AST).  
3. **Semantic Analysis**: Ensures the code follows Argon’s rules (e.g., type checking).  
4. **Code Generation**: Outputs x86_64 assembly code.  

If you’re interested in the nitty-gritty details, check out the `src/` directory for the implementation of each stage.

---

**Argon Compiler: Because every language needs a heart to beat.** ❤️✨
