In **Argon**, modules are the primary unit of code organisation, encapsulation, and compilation. The module system is intentionally kept **minimalist and explicit** to suit its purpose as a toy language that compiles to **x86-64 assembly**.

Here's an explanation of how modules are to work and are treated in the language:

---

## 📦 **Module System in Argon**

---

### 🧱 1. **Modules are File-Based Units**

Each **`.ar`** source file defines exactly one module. The module name must not match the file path.

---

### 📥 2. **Importing Modules**

Modules are imported using the `import` keyword:

```Argon
import math.core;
import utils.conversion;
```

Imported symbols **must be explicitly marked `export`** in their respective modules to be visible. Importing a module does **not** import all of its contents unless re-exported.

There is **no implicit glob importing**, no `import *`, and no import side effects.

---

### 🚪 3. **Exporting Symbols**

Only functions and constants can be exported:

```Argon
export define add_i32(a i32, b i32) i32 {
    ret a + b
}

export const PI f64 = 3.1415926535
```

* Types are primitive and global, so there’s nothing else to export.
* There are no nested modules.

---

### ⚙️ 4. **Module Compilation Model**

Argon uses a **separate compilation model** with a linker:

* Each module is compiled independently into its own `.o` or `.asm` file.
* The compiler tracks dependencies and symbol exports to generate a symbol table.
* Only used, imported, and reachable functions are included during linking.

The compilation process is roughly:

1. Parse & typecheck individual module.
2. Emit IR (intermediate representation).
3. Translate IR to x86-64 assembly.
4. Link compiled modules together.

---

### 📚 5. **Dependency Resolution**

* Modules are resolved using file paths rooted from the project directory.
* No package manager is included by default; dependencies are tracked locally.
* Future extensions can support remote or binary modules.

---

### 🔒 6. **Encapsulation**

Modules enforce symbol privacy:

* Functions and constants are **private by default**.
* `export` is required to make them accessible.
* There is **no public/private keyword**—just export or not.

There are no global symbols; everything must reside within a module scope.

---

### 🧪 7. **Example**

File: `src/math/core.ar`

```Argon
module math.core;

export define add_i32(a i32, b i32) i32 {
    ret a + b
}

define internal_helper(x i32) i32 {
    ret x * 2
}
```

File: `src/main.ar`

```Argon
module main;
import math.core;

define main {
    var sum i32 = add_i32(10, 20)
    ret sum
}
```

`internal_helper` is **not visible** outside `math.core` and will not be compiled if unused.

---

### 🚫 8. **What Modules Don't Do**

* **No runtime module loading** – All modules are static and resolved at compile time.
* **No macros or generics in modules**
* **No circular imports allowed**
* **No module-level side effects** – Modules are just containers for symbols.

---

## 🧩 Summary

| Feature           | Description                                                               |
| ----------------- | ------------------------------------------------------------------------- |
| Declaration       | Each `.ar` file starts with `module <name>`                               |
| Scope             | Private by default; must `export`                                         |
| Imports           | Always explicit (`import math.core`)                                      |
| Symbols           | Only `define` and `const` can be exported                                 |
| Compilation Model | Separate per-module compilation, linked statically                        |
| No Hidden Magic   | No macros, implicit imports, circular dependencies, or runtime evaluation |

---
