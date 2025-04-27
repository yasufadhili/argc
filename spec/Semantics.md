# Argon Language Semantics

## Table of Contents
1. [Type System](#type-system)
2. [Memory Model](#memory-model)
3. [Execution Model](#execution-model)
4. [Expression Evaluation](#expression-evaluation)
5. [Control Flow](#control-flow)
6. [Function Semantics](#function-semantics)
7. [Module System](#module-system)
8. [Error Handling](#error-handling)
9. [Concurrency Model](#concurrency-model)
10. [Global Variables](#global-variables)
11. [Assertions](#assertions)

## Type System

### Type Categories

Argon's type system consists of several categories:

1. **Primitive Types**
   - Integer types: `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`
   - Floating-point types: `f32`, `f64`
   - Boolean type: `bool`
   - Character type: `rune`
   - String type: `str`

2. **Composite Types**
   - Arrays: `[N]T` (fixed-size) and `[]T` (compiler-determined size)
   - Structures: `struct { ... }`
   - Enumerations: `enum { ... }`

3. **Reference Types**
   - Pointers: `*T`
   - Function types: `def(T) R`

### Type Rules

1. **Type Inference**
   - Variables must have declared types
   - Function can have no return types or even several
   - Array sizes must be compile-time constants

2. **Type Compatibility**
   - Integer types can be implicitly converted to larger integer types
   - Floating-point types can be implicitly converted to larger floating-point types
   - No implicit conversion between integer and floating-point types
   - No implicit conversion between signed and unsigned types

3. **Type Safety**
   - All type conversions must be explicit
   - Array bounds are checked at runtime
   - Null pointer dereferences are caught at runtime
   - Type assertions are checked at runtime

## Memory Model

### Memory Layout

1. **Stack Allocation**
   - Local variables are allocated on the stack
   - Stack grows downward
   - Stack frames include:
     - Return address
     - Saved registers
     - Local variables
     - Function parameters

2. **Heap Allocation**
   - Dynamic data structures are allocated on the heap
   - Managed by the runtime system
   - Garbage collection is not implemented (manual memory management)

3. **Data Alignment**
   - All data types are naturally aligned
   - i8: 1-byte alignment
   - i16: 2-byte alignment
   - i32: 4-byte alignment
   - i64: 8-byte alignment
   - f32: 4-byte alignment
   - f64: 8-byte alignment

### Memory Operations

1. **Variable Declaration**
   ```argon
   var x i32        // Uninitialized, zero value
   var y i32 = 42   // Initialized
   const Z = 100    // Compile-time constant
   ```

2. **Pointer Operations**
   ```argon
   var x i32 = 42
   var p *i32 = &x  // Address-of operator
   *p = 100         // Dereference
   ```

3. **Array Operations**
   ```argon
   var arr [3]i32
   arr[0] = 1       // Bounds checked
   arr[3] = 4       // Runtime error
   ```

## Execution Model

### Program Initialisation

1. **Entry Point**
   - Program execution begins at the `main` function
   - `main` must be declared in the `main` module
   - Command-line arguments are passed to `main`

2. **Module Initialisation**
   - Modules are initialised in dependency order
   - Package-level variables are initialised before function execution
   - Initialisation is single-threaded

### Runtime Environment

1. **System Calls**
   - System calls are made through the standard library
   - File operations, network operations, etc.
   - Error handling through return values

2. **Signal Handling**
   - SIGSEGV: Memory access violations
   - SIGILL: Illegal instructions
   - SIGFPE: Floating-point exceptions

## Expression Evaluation

### Evaluation Order

1. **Operator Precedence**
   ```argon
   // Highest to lowest
   () [] .          // Parentheses, array indexing, member access
   ! ~ ++ --        // Unary operators
   * / %            // Multiplicative
   + -              // Additive
   << >>            // Bitwise shift
   < <= > >=        // Relational
   == !=            // Equality
   &                // Bitwise AND
   ^                // Bitwise XOR
   |                // Bitwise OR
   &&               // Logical AND
   ||               // Logical OR
   = += -= *= /=    // Assignment
   ```

2. **Short-Circuit Evaluation**
   - Logical AND (`&&`): Second operand not evaluated if first is false
   - Logical OR (`||`): Second operand not evaluated if first is true

### Side Effects

1. **Variable Modification**
   - Assignment operations
   - Increment/decrement operators
   - Function calls with reference parameters

2. **I/O Operations**
   - Console input/output
   - File operations
   - Network operations

## Control Flow

### Branching

1. **If Statement**
   ```argon
   if condition {
       // Executed if condition is true
   } else if condition2 {
       // Executed if condition2 is true
   } else {
       // Executed if all conditions are false
   }
   ```

2. **Match Statement**
   ```argon
   match value {
       case: pattern1 {
           // Executed if value matches pattern1
       }
       case: pattern2 {
           // Executed if value matches pattern2
       }
   }
   ```

### Looping

1. **For Loop**
   ```argon
   var i i8 = 0
   for i < 5 {
       // Loop body
       i++
   }
   ```

2. **Repeat**
    ```
    repeat {
      // Loops forever
    }
    ```

    ```
    repeat x {
      // Repeats x number of times
    }

    ```

3. **Repeat While Loop**
   ```argon
   var x = 1
   repeat {
       // Loop body
       x += 1
   } while x < 5
   ```

## Function Semantics

### Function Calls

1. **Parameter Passing**
   - All parameters are passed by value
   - Pointers can be used for pass-by-reference
   - Return values are passed in registers (System V ABI)

2. **Stack Frame**
   ```argon
   // Function prologue
   push rbp
   mov rbp, rsp
   sub rsp, <local_vars_size>

   // Function body
   ...

   // Function epilogue
   mov rsp, rbp
   pop rbp
   ret
   ```

### Function Types

1. **First-Class Functions**
   ```argon
   type BinaryOp func(i32, i32) i32

   def add(a i32, b i32) i32 {
       return a + b
   }

   var operation BinaryOp = add
   ```

2. **Closures**
   - Not supported (no heap allocation for closures)
   - Functions cannot capture variables from outer scope

## Module System

### Module Resolution

1. **Import Paths**
   - Relative to current module
   - Absolute from workspace root
   - No circular dependencies allowed

2. **Symbol Visibility**
   - Exported symbols start with uppercase letter
   - Unexported symbols start with lowercase letter
   - Module-private symbols are not accessible outside module

### Module Initialisation

1. **Dependency Order**
   - Modules are initialized in topological order
   - Circular dependencies are detected at compile time
   - Initialisation is single-threaded

## Error Handling

### Error Types

1. **Runtime Errors**
   - Division by zero
   - Null pointer dereference
   - Array bounds violation
   - Stack overflow
   - Type assertion failure
   - Assertion failure
   - Invalid type conversion
   - Invalid memory access
   - System call failures

2. **Recoverable Errors**
   ```argon
   // Function that can throw errors
   def canThrow() throws str {
       // Implementation
   }

   // Function with multiple error types
   def complexOperation() throws (str, i32) {
       // Implementation
   }
   ```

### Error Propagation

1. **Error Handling Syntax**
   ```argon
   // Basic error handling
   def handleError() {
       var result, err = canThrow()
       if err != nil {
           // Handle error
       }
   }

   // Pattern matching for errors
   def handleMultipleErrors() {
       match complexOperation() {
           case: (result, nil) {
               // Success case
           }
           case: (nil, err) {
               // Error case
           }
       }
   }
   ```

2. **Error Propagation Rules**
   - Errors must be explicitly handled
   - Unhandled errors cause program termination
   - Error values must be checked before use
   - Error types must match exactly

### Error Recovery

1. **Defer Statements**
   ```argon
   def fileOperation() {
       var file = openFile("data.txt")
       defer file.close()  // Executed when function exits

       // File operations
       if error {
           return  // File still gets closed
       }
   }
   ```

2. **Cleanup Patterns**
   ```argon
   def resourceHandler() {
       var resource = acquireResource()
       defer releaseResource(resource)

       // Use resource
       if error {
           return  // Resource still gets released
       }
   }
   ```

### Error Context

1. **Error Wrapping**
   ```argon
   def operation() throws str {
       var result, err = subOperation()
       if err != nil {
           return "operation failed: " + err
       }
       return result
   }
   ```

2. **Error Stack Traces**
   - Stack traces are automatically generated
   - Include file and line information
   - Show error propagation path
   - Include relevant context

### Error Prevention

1. **Type Safety**
   ```argon
   // Compile-time type checking
   var x i32 = "string"  // Compile error

   // Runtime type checking
   var y any = 42
   var z i32 = y.(i32)  // Runtime type assertion
   ```

2. **Bounds Checking**
   ```argon
   var arr [3]i32
   arr[4] = 42  // Runtime bounds check error
   ```

3. **Null Safety**
   ```argon
   var ptr *i32
   *ptr = 42  // Runtime null check error
   ```

### Error Reporting

1. **Error Messages**
   - Clear and descriptive
   - Include relevant context
   - Suggest possible fixes
   - Follow consistent format

2. **Error Logging**
   ```argon
   def logError(err str) {
       // Log error to appropriate output
       // Include timestamp
       // Include severity level
       // Include stack trace
   }
   ```

### Error Handling Best Practices

1. **Error Checking**
   - Check errors immediately after operations
   - Handle errors at the appropriate level
   - Provide meaningful error messages
   - Use appropriate error types

2. **Resource Management**
   - Use defer for cleanup
   - Handle partial failures
   - Clean up resources in error cases
   - Maintain consistent state

3. **Error Recovery**
   - Implement retry logic where appropriate
   - Provide fallback mechanisms
   - Handle partial success cases
   - Maintain data consistency

### System-Level Error Handling

1. **Signal Handling**
   ```argon
   // SIGSEGV: Memory access violations
   // SIGILL: Illegal instructions
   // SIGFPE: Floating-point exceptions
   // SIGABRT: Program abort
   ```

2. **System Call Errors**
   - File system errors
   - Network errors
   - Process errors
   - Resource errors

3. **Environment Errors**
   - Memory allocation failures
   - Stack overflow
   - Resource exhaustion
   - System call failures

### Error Handling in Modules

1. **Module-Level Error Types**
   ```argon
   module network

   // Base error type for the module
   type NetworkError enum {
       TIMEOUT
       CONNECTION_FAILED
       PROTOCOL_ERROR
       INVALID_STATE
       RESOURCE_EXHAUSTED
   }

   // Error with additional context
   type ConnectionError struct {
       code NetworkError
       message str
       details map[str]any
   }

   // Function that returns module-specific error
   def connect(host str, port i32) throws ConnectionError {
       // Implementation
   }
   ```

2. **Error Type Hierarchy**
   ```argon
   module database

   // Base error type
   type DBError enum {
       CONNECTION_FAILED
       QUERY_FAILED
       TRANSACTION_FAILED
   }

   // Specific error types
   type ConnectionError struct {
       base DBError
       host str
       port i32
   }

   type QueryError struct {
       base DBError
       query str
       params []any
   }
   ```

3. **Error Visibility and Export**
   ```argon
   module http

   // Exported error type (uppercase)
   type HTTPError enum {
       BAD_REQUEST
       UNAUTHORIZED
       FORBIDDEN
       NOT_FOUND
   }

   // Private error type (lowercase)
   type connectionError struct {
       code i32
       message str
   }
   ```

4. **Module Error Handlers**
   ```argon
   module file_system

   // Error handler interface
   type ErrorHandler interface {
       handle(error) bool
   }

   // Default error handler
   type DefaultHandler struct {
       logFile str
   }

   def (h DefaultHandler) handle(err error) bool {
       // Log error to file
       // Return true if error was handled
   }
   ```

5. **Module Error Recovery**
   ```argon
   module transaction

   // Transaction error recovery
   def executeWithRecovery(tx Transaction) throws TransactionError {
       defer {
           if error {
               tx.rollback()
           }
       }
       
       // Transaction operations
   }
   ```

6. **Module Error Context**
   ```argon
   module api

   // Error with context
   type APIError struct {
       code i32
       message str
       context map[str]any
       stackTrace []str
   }

   def newAPIError(code i32, message str) APIError {
       return APIError{
           code: code,
           message: message,
           context: make(map[str]any),
           stackTrace: getStackTrace()
       }
   }
   ```

7. **Module Error Utilities**
   ```argon
   module error_utils

   // Error wrapping
   def wrapError(err error, context str) error {
       return Error{
           cause: err,
           context: context,
           timestamp: time.now()
       }
   }

   // Error classification
   def isRecoverable(err error) bool {
       match err {
           case: NetworkError {
               return true
           }
           case: DBError {
               return false
           }
       }
   }
   ```

8. **Module Error Documentation**
   ```argon
   module documentation

   // Error documentation
   type ErrorDoc struct {
       code i32
       description str
       possibleCauses []str
       suggestedFixes []str
   }

   // Generate error documentation
   def documentError(err error) ErrorDoc {
       // Implementation
   }
   ```

9. **Module Error Testing**
   ```argon
   module error_test

   // Error test cases
   def testErrorHandling() {
       // Test error creation
       var err = newError("test")
       assert err != nil

       // Test error wrapping
       var wrapped = wrapError(err, "context")
       assert wrapped.context == "context"

       // Test error recovery
       var recovered = recoverFromError(err)
       assert recovered == true
   }
   ```

10. **Module Error Metrics**
    ```argon
    module metrics

    // Error metrics collection
    type ErrorMetrics struct {
        count i32
        types map[str]i32
        recoveryRate f64
    }

    def collectErrorMetrics() ErrorMetrics {
       // Implementation
    }
    ```

### Module Error Handling Best Practices

1. **Error Type Design**
   - Use enums for simple error codes
   - Use structs for errors with additional context
   - Implement error interfaces for flexibility
   - Provide clear error messages

2. **Error Handling Patterns**
   - Use defer for cleanup
   - Implement error recovery strategies
   - Provide error context
   - Log errors appropriately

3. **Error Documentation**
   - Document all error types
   - Provide usage examples
   - Include recovery strategies
   - Document error codes

4. **Error Testing**
   - Test error creation
   - Test error handling
   - Test error recovery
   - Test error propagation

5. **Error Monitoring**
   - Collect error metrics
   - Monitor error rates
   - Track error patterns
   - Implement alerts

## Concurrency Model

### Threading

1. **Thread Creation**
   - Not supported in current version
   - Future versions may add threading support

2. **Synchronisation**
   - Not supported in current version
   - Future versions may add synchronisation primitives

### Atomic Operations

1. **Atomic Types**
   - Not supported in current version
   - Future versions may add atomic operations

## Global Variables

### Initialization Semantics

1. **Initialization Order**
   - Global variables are initialized before any function calls
   - Initialization follows declaration order
   - Circular dependencies between globals are detected at compile time

2. **Initialization Rules**
   ```argon
   global var x i32 = 42        // Valid
   global var y i32            // Invalid: must be initialized
   
   global {
       var a = 10              // Type inferred
       var b i32 = 20          // Explicit type
   }
   ```

### Memory Semantics

1. **Storage Duration**
   - Global variables have static storage duration
   - Memory is allocated at program startup
   - Memory is deallocated at program termination

2. **Thread Safety**
   - Global variables are not thread-safe by default
   - Access to global variables requires explicit synchronization
   - Global constants are thread-safe

3. **Visibility Rules**
   - Global variables are module-scoped by default
   - Exported globals (uppercase) are accessible from other modules
   - Private globals (lowercase) are module-private

## Assertions

### Compile-time Semantics

1. **Constant Folding**
   ```argon
   const x = 10
   assert x > 0                // Evaluated at compile time
   ```

2. **Type Checking**
   - Assertion condition must be boolean
   - Assertion message must be string
   - Compile-time assertions must be constant expressions

### Runtime Semantics

1. **Assertion Evaluation**
   ```argon
   var x = getUserInput()
   assert x > 0, "Input must be positive"  // Evaluated at runtime
   ```

2. **Error Handling**
   - Failed assertions terminate the program
   - Error message is displayed to stderr
   - Stack trace is included in error output

3. **Performance Impact**
   - Assertions are removed in release builds
   - Runtime assertions have minimal overhead
   - Compile-time assertions have no runtime cost

### Assertion Contexts

1. **Function Preconditions**
   ```argon
   def sqrt(x f64) f64 {
       assert x >= 0, "Cannot take square root of negative number"
       // Implementation
   }
   ```

2. **Loop Invariants**
   ```argon
   var i = 0
   while i < 10 {
       assert i >= 0 && i < 10
       // Loop body
       i++
   }
   ```

3. **Module Initialization**
   ```argon
   global {
       const MAX_SIZE = 100
       assert MAX_SIZE > 0
   }
   ```

## Notes on x86_64 Implementation

### Register Usage

1. **General Purpose Registers**
   - RAX: Return value
   - RBX: Callee-saved
   - RCX: 4th argument
   - RDX: 3rd argument
   - RSI: 2nd argument
   - RDI: 1st argument
   - RSP: Stack pointer
   - RBP: Frame pointer

2. **Floating-Point Registers**
   - XMM0-XMM7: Arguments and return values
   - XMM8-XMM15: Temporary storage

### Calling Convention

1. **System V AMD64 ABI**
   - First 6 integer arguments in registers
   - First 8 floating-point arguments in XMM registers
   - Additional arguments on stack
   - Return values in RAX/XMM0

2. **Stack Frame**
   - 16-byte aligned
   - Red zone (128 bytes below RSP)
   - Shadow space for register arguments
