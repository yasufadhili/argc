
# 🌌 The Argon Programming Language

**A Toy Programming Language for x86_64 Linux Systems**  

**Because why not?**

Argon is a playful experiment in programming language design. It compiles to x86_64 assembly and runs on Linux. Think of it as a sandbox for learning and tinkering with compilers, assembly, and language design.

## 🚀 What is Argon?

Argon is a modular programming language where components are separated into modules. These modules can be compiled and linked statically to other programs. It even supports linking with C and C++ static libraries! 🎉

But let’s be honest: Argon is a toy language. It’s not here to compete with the big players. It’s here to have fun, learn, and explore.

## 🏗️ Repository Structure

This repository is divided into several subdirectories, each representing a key part of the Argon ecosystem:

- **[arc](arc/)**: The Argon Compiler. The heart of the language.  
  
- **[arlib](arlib/)**: The Argon Standard Library. A collection of utilities (some better than others).  
  
- **[editors](editors/)**: Text editor/IDE support for Argon.  
  
- **[spec](spec/)**: The Argon Language Specification. For those who love reading specs.  
  
- **[docs](docs/)**: Documentation for Argon, including the website.  
  
- **[play](play/)**: The Argon Online Playground. A web-based editor to try Argon in your browser.  

## 📝 Why Argon Exists

Argon was built for one reason: **to see if I could do it**. It’s a passion project, a learning tool, and a playground for experimentation. 

## ⚠️ Limitations (aka "Why You Shouldn't Use Argon in Production")

Let’s be real. Argon has its quirks:

- 🐢 **Poor Compiler Optimisation**: It’s not going to win any speed contests.  
  
- 📜 **Non-Standard Syntax**: Some parts of the syntax might make you scratch your head.
    
- 📚 **Sparse Documentation**: We’re working on it (kind of).  
  
- 🛠️ **Buggy Standard Library**: Expect some rough edges.  
  
- 🧪 **Limited Tests**: Testing is... minimal.  

But hey, it’s all part of the charm, right?

## 🌟 What Can You Learn from Argon?

Argon is a great way to dive into:

- 🛠️ **Compiler Construction**: Learn how compilers work by exploring the Argon Compiler.  
  
- 💻 **C++ Programming**: The compiler and part of the standard library are written in C++ and some C. 
   
- 🌐 **Web Development**: Check out the Argon Online Playground (but maybe don’t take notes).  

## 📚 Resources

Here are some resources to help you get started:

- **[Argon Language Specification](spec/)**: Dive into the nitty-gritty details of the language.  
- **[Argon Online Playground](play/)**: Try Argon in your browser.  
- **[Compiler Construction Books](https://craftinginterpreters.com/)**: Learn how to build your own language.  
- **[x86 Assembly Guide](https://cs.lmu.edu/~ray/notes/x86assembly/)**: Understand the assembly Argon compiles to.  

## 🛠️ Building and Running Argon

To build the Argon Compiler, clone this repository and follow these steps:

```bash
# Clone the repository
git clone https://github.com/yasufadhili/argon.git
cd argon

# Build the compiler from ./arc
make

# Run the compiler
./arc <source-file>
```

## 💡 Examples

Here’s a simple "Hello, World!" program in Argon:

```c++
module main

import sys

def main() {
    sys::print("Hello, World!")
    sys::exit(0)
}
```

Compile it with the Argon Compiler:

```bash
./arc hello.ar
```

I know *.ar* is for archives, But hey, my language, my rules

## 🧪 Contributing

Want to contribute? Awesome! Here’s how you can help:

- Report bugs 🐛  
- Improve the standard library 📚  
- Write tests 🧪  
- Add documentation ✍️  

## 📜 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

**Argon: Because sometimes, you just want to build a programming language for fun.**
