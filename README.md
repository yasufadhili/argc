
# The Argon Programming Language

**A Toy Programming Language for x86_64 Linux Systems**  

**Because why not?**

Argon is a playful experiment in programming language design. It compiles to x86_64 assembly and runs on Linux. Think of it as a sandbox for learning and tinkering with compilers, assembly, and language design.

## What is Argon?

Argon is a modular programming language where components are separated into modules. These modules can be compiled and linked statically to other programs. It should even support linking with C and C++ static libraries! 

But let’s be honest: Argon is a toy language. It’s not here to compete with the big players. It’s here to have fun, learn, and explore.

## Repository Structure

This repository is divided into several subdirectories, each representing a key part of the Argon ecosystem:

- **[argc](argc/)**: The Argon Compiler. The heart of the language.  
  
- **[arlib](arlib/)**: The Argon Standard Library. (Not yet).  
  
- **[spec](spec/)**: The Argon Language Specification. For those who love reading specs (Work in progress).  
  
## Why Argon Exists

Argon is being built for one reason: **to see if I can do it**. It’s a passion project, a learning tool, and a playground for experimentation. 

## What Can You Learn from Argon?

Argon is a great way to dive into:

-  **Compiler Construction**: Learn how compilers work by exploring the Argon Compiler.  
  
-  **C++ Programming**: The compiler and soon part of the standard library is written in C++. 
   
##  Resources

Here are some resources to help you get started:

- **[Argon Language Specification](spec/)**: Dive into the nitty-gritty details of the language.  

- **[Compiler Construction Books](https://craftinginterpreters.com/)**: Learn how to build your own language. 

- **[x86 Assembly Guide](https://cs.lmu.edu/~ray/notes/x86assembly/)**: Understand the assembly Argon compiles to.  


##  License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

**Argon: Because sometimes, you just want to build a programming language for fun.**
