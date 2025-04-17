
# 📚 Argon Standard Library (arlib)

Welcome to the **Argon Standard Library**—the toolbox for all your Argon programming needs! 🛠️✨ This is where you’ll find reusable utilities, modules, and functions that make coding in Argon a little easier (and sometimes a little weirder). Whether you’re building something serious (I'll keep saying it till it happens) or just tinkering, `arlib` has your back (sort of).

## 🌟 What is the Argon Standard Library?

The Argon Standard Library (or `arlib`) is a collection of pre-built modules and utilities designed to extend the functionality of the Argon programming language. Think of it as the "batteries included" part of Argon—except some of the batteries might be a little leaky. 🔋

## 📂 What’s Inside?

Here’s a quick overview of the modules you’ll find in `arlib`:

- **`sys`**: System-level utilities for interacting with the operating system (e.g., printing to the console, exiting programs).  
  
- **`math`**: Basic mathematical functions and constants (because who doesn’t love math?).  
  
- **`string`**: String manipulation utilities for slicing, dicing, and concatenating text.  
  
- **`collections`**: Data structures like lists, maps, and sets (still a work in progress).  
  
- **`io`**: Input/output utilities for reading and writing files.
    
- **`time`**: Functions for working with dates, times, and delays.  
  
- **`experimental`**: A playground for new, unstable features (use at your own risk!).  

## 🚀 Getting Started

To use the Argon Standard Library in your code, simply import the modules you need. For example:

```c++
module main

import sys
import math

def main() {
    sys::print("Welcome to Argon! 🎉")
    var result i16 = math::add(2, 3)
    sys::print("2 + 3 = " + result)
}
```

## 🛠️ How to Build and Use `arlib`

The Standard Library is included as part of the Argon repository. To build and use it:

1. **Build the Compiler**:  
   Make sure you’ve built the Argon Compiler (`arc`) by following the instructions in the main repository.  

2. **Link the Standard Library**:  
   The Standard Library modules are automatically linked when you compile Argon programs. Just import the modules you need, and you’re good to go!  

3. **Explore the Code**:  
   Dive into the `arlib/` directory to see how the library is implemented. Feel free to modify or extend it to suit your needs.  

## 🌟 Highlights of `arlib`

- **Lightweight and Modular**: Import only what you need—no unnecessary bloat.  
  
- **Cross-Compatible**: Works seamlessly with all Argon programs.
    
- **Open for Contributions**: Got a great idea for a new module? I’d love to see it!  

## 📚 Module Documentation

Here’s a closer look at some of the key modules:

### `sys`
System-level utilities for interacting with the OS.  

**Functions**:  
- `sys::print(message: []char)`: Prints a message to the console.  
- `sys::exit(code: i32)`: Exits the program with the given status code.  

### `math`
Basic mathematical operations and constants.  

**Functions**:  
- `math::add(a: int, b: int) -> int`: Returns the sum of two integers.  
- `math::pow(base: int, exp: int) -> int`: Raises a number to a power.  

### `string`
Utilities for working with strings.  

**Functions**:  
- `string::length(s: string) -> int`: Returns the length of a string.  
- `string::concat(a: string, b: string) -> string`: Concatenates two strings.  

### `collections`
Data structures for managing collections of data.  

**Features**:  
- Lists, maps, and sets (coming soon!).  

### `io`
Input/output utilities for file handling.  

**Functions**:  
- `io::read_file(path: string) -> string`: Reads the contents of a file.  
- `io::write_file(path: string, content: string)`: Writes content to a file.  

### `time`
Functions for working with time.  
**Functions**:  
- `time::now() -> string`: Returns the current date and time.  
- `time::sleep(ms: int)`: Pauses execution for the given number of milliseconds.  

### `experimental`
A playground for unstable or in-progress features. Use with caution!  

## 🧑‍💻 Contributing to `arlib`

I'm always looking to expand and improve the Argon Standard Library! Here’s how you can contribute:

1. Fork the repository.  
   
2. Add or improve a module in the `arlib/` directory.  
3. Write tests for your changes (please!).  
4. Submit a pull request with a clear description of your updates.  

## 🌈 Join the Fun!

The Argon Standard Library is all about making Argon more powerful and fun to use. Whether you’re fixing bugs, adding features, or just exploring, I'm glad to have you here. Happy coding! 🎉

---

**Argon Standard Library: Because every language needs a little help sometimes.** 📚✨
