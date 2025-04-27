# Argon Module System

## Table of Contents
1. [Module Basics](#module-basics)
2. [Module Structure](#module-structure)
3. [Module Dependencies](#module-dependencies)
4. [Module Initialisation](#module-initialisation)
5. [Module Visibility](#module-visibility)
6. [Module Resolution](#module-resolution)
7. [Module Compilation](#module-compilation)
8. [Module Linking](#module-linking)

## Module Basics

### Module Declaration

A module in Argon is declared using the `module` keyword:

```argon
module math

// Module contents
```

### Module Naming

1. **Rules**
   - Module names must be valid identifiers
   - Module names are case-sensitive
   - Module names should be lowercase
   - Module names can contain underscores

2. **Examples**
   ```argon
   module math
   module string_utils
   module network_http
   ```

### Module Scope

1. **File Scope**
   - Each `.ar` file must declare exactly one module
   - The module declaration must be the first non-comment line
   - All code in the file belongs to that module

2. **Directory Scope**
   - Modules can span multiple files in the same directory
   - All files in a directory must declare the same module
   - Subdirectories can contain different modules

## Module Structure

### Module Components

1. **Imports**
   ```argon
   module math
   
   import std::io
   import std::math
   ```

2. **Type Declarations**
   ```argon
   type Vector struct {
       x f64
       y f64
   }
   ```

3. **Function Declarations**
   ```argon
   def add(a i32, b i32) i32 {
       return a + b
   }
   ```

4. **Constant Declarations**
   ```argon
   const PI = 3.14159
   ```

### Module Organisation

1. **File Structure**
   ```
   module_name/
   ├── module_name.ar    # Main module file
   ├── types.ar          # Type definitions
   ├── utils.ar          # Utility functions
   └── tests.ar          # Test cases
   ```

2. **Module Hierarchy**
   ```
   project/
   ├── main.ar           # Main module
   ├── math/             # Math module
   │   ├── math.ar
   │   └── vector.ar
   └── utils/            # Utils module
       ├── utils.ar
       └── string.ar
   ```

## Module Dependencies

### Import Syntax

1. **Basic Import**
   ```argon
   import std::io
   ```

2. **Renamed Import**
   ```argon
   import std::io as myio
   ```

3. **Selective Import**
   ```argon
   import std::math::{sin, cos, tan}
   ```

### Dependency Rules

1. **No Circular Dependencies**
   - Module A cannot import Module B if Module B imports Module A
   - Circular dependencies are detected at compile time

2. **Transitive Dependencies**
   - If Module A imports Module B, and Module B imports Module C
   - Module A has access to Module C's public symbols through Module B

3. **Dependency Graph**
   ```
   main
   ├── math
   │   └── utils
   └── network
       └── utils
   ```

## Module Initialisation

### Initialisation Order

1. **Dependency-Based**
   - Modules are initialised in dependency order
   - Leaf modules (no dependencies) are initialised first
   - Root modules (most dependencies) are initialised last

2. **Initialisation Steps**
   ```argon
   // 1. Static initialisation
   const X = 42
   
   // 2. Global variable initialisation
   var global = 100
   
   // 3. Module initialisation function (if present)
   def init() {
       // Module setup code
   }
   ```

### Initialisation Guarantees

1. **Single Initialisation**
   - Each module is initialised exactly once
   - Initialisation is thread-safe
   - Initialisation is atomic

2. **Dependency Guarantees**
   - All dependencies are fully initialised before a module is initialised
   - Initialisation failures propagate up the dependency chain

## Module Visibility

### Symbol Visibility

1. **Public Symbols**
   - Start with uppercase letter
   - Accessible from other modules
   - Example: `def PublicFunction()`

2. **Private Symbols**
   - Start with lowercase letter
   - Only accessible within the module
   - Example: `def privateFunction()`

3. **Module-Local Symbols**
   - Prefixed with underscore
   - Only accessible within the file
   - Example: `def _localFunction()`

### Visibility Rules

1. **Type Visibility**
   ```argon
   type PublicType struct {    // Exported
       PublicField i32        // Exported
       privateField i32       // Private
   }
   ```

2. **Function Visibility**
   ```argon
   def PublicFunction() {     // Exported
       _localFunction()       // File-local
   }
   
   def privateFunction() {    // Module-private
   }
   ```

## Module Resolution

### Search Paths

1. **Standard Library**
   - `std::` prefix for standard library modules
   - Example: `import std::io`

2. **Local Modules**
   - Relative to current module
   - Example: `import ../utils`

3. **Workspace Modules**
   - Absolute from workspace root
   - Example: `import /project/utils`

### Resolution Rules

1. **Path Resolution**
   - Relative paths are resolved from the importing file's location
   - Absolute paths are resolved from the workspace root
   - Module names must match directory names

2. **Conflict Resolution**
   - Local modules take precedence over standard library
   - Explicit imports take precedence over implicit ones
   - Renamed imports resolve naming conflicts

## Module Compilation

### Compilation Units

1. **Single File Compilation**
   ```bash
   arc module.ar
   ```

2. **Module Compilation**
   ```bash
   arc -m module_name
   ```

3. **Project Compilation**
   ```bash
   arc -p project_name
   ```

### Compilation Output

1. **Object Files**
   - `.o` files for each module
   - Position-independent code
   - Debug information included

2. **Module Metadata**
   - Module dependencies
   - Symbol table
   - Type information

## Module Linking

### Linkage Types

1. **Static Linking**
   - Modules are linked at compile time
   - All symbols resolved at compile time
   - Produces standalone executable

2. **Dynamic Linking**
   - Modules are linked at runtime
   - Symbols resolved at load time
   - Supports plugin architecture

### Linkage Rules

1. **Symbol Resolution**
   - Public symbols must be unique across modules
   - Private symbols can be duplicated
   - Weak symbols can be overridden

2. **Linkage Attributes**
   ```argon
   // Strong linkage (default)
   def function() { ... }
   
   // Weak linkage
   def weak function() { ... }
   
   // Hidden linkage
   def hidden function() { ... }
   ```

## Module Best Practices

### Organisation

1. **Module Size**
   - Keep modules focused and cohesive
   - Split large modules into smaller ones
   - Group related functionality

2. **Dependency Management**
   - Minimise dependencies
   - Avoid circular dependencies
   - Use interface modules for abstraction

### Documentation

1. **Module Documentation**
   ```argon
   // Module: math
   // Description: Mathematical operations and types
   // Dependencies: std::io
   module math
   ```

2. **Public Interface**
   - Document all public symbols
   - Provide usage examples
   - Document error conditions

### Testing

1. **Test Modules**
   ```argon
   module math_test
   
   import math
   
   def testAdd() {
       assert(math::add(1, 2) == 3)
   }
   ```

2. **Test Organisation**
   - Separate test modules
   - Test both public and private interfaces
   - Include edge cases
