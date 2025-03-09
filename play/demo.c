/*
 * C Language Features Demonstration
 * 
 * This program demonstrates key features of the C programming language
 * including data types, control structures, functions, pointers,
 * memory management, structures, file I/O, and more.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>  // C99 feature for boolean type
#include <stdint.h>   // For fixed-width integer types
#include <time.h>     // For time functions
#include <assert.h>   // For assertions
#include <math.h>     // For mathematical functions

// Macro definitions
#define PI 3.14159
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define PRINT_VAR(var) printf(#var " = %d\n", var)

// Enumeration
enum Weekday {
    MONDAY = 1,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
};

// Structure definition
struct Person {
    char name[50];
    int age;
    float height;
};

// Union example
union Data {
    int i;
    float f;
    char str[20];
};

// Typedef example
typedef struct {
    int x;
    int y;
} Point;

// Function prototypes
void demonstrate_variables(void);
void demonstrate_operators(void);
void demonstrate_control_flow(void);
int factorial(int n);
int fibonacci(int n);
void demonstrate_arrays(void);
void demonstrate_strings(void);
void demonstrate_pointers(void);
void swap(int *a, int *b);
void demonstrate_dynamic_memory(void);
void demonstrate_structures(void);
void demonstrate_file_operations(void);
void demonstrate_function_pointers(void);
void demonstrate_preprocessor(void);
void callback_function(int value);
void demonstrate_variadic_functions(const char *format, ...);
void error_handling_demo(void);

// Global variable
int global_var = 100;

int main(void) {
    printf("==== C LANGUAGE FEATURES DEMONSTRATION ====\n\n");
    
    // Demonstrate basic variables and data types
    demonstrate_variables();
    
    // Demonstrate operators
    demonstrate_operators();
    
    // Demonstrate control flow structures
    demonstrate_control_flow();
    
    // Demonstrate recursion with factorial function
    printf("\n==== RECURSION DEMONSTRATION ====\n");
    printf("Factorial of 5: %d\n", factorial(5));
    printf("Fibonacci(10): %d\n", fibonacci(10));
    
    // Demonstrate arrays
    demonstrate_arrays();
    
    // Demonstrate strings
    demonstrate_strings();
    
    // Demonstrate pointers
    demonstrate_pointers();
    
    // Demonstrate dynamic memory allocation
    demonstrate_dynamic_memory();
    
    // Demonstrate structures and unions
    demonstrate_structures();
    
    // Demonstrate file I/O
    demonstrate_file_operations();
    
    // Demonstrate function pointers
    demonstrate_function_pointers();
    
    // Demonstrate preprocessor directives
    demonstrate_preprocessor();
    
    // Demonstrate error handling
    error_handling_demo();
    
    printf("\n==== END OF DEMONSTRATION ====\n");
    return 0;
}

// Function implementations

void demonstrate_variables(void) {
    printf("\n==== VARIABLES AND DATA TYPES ====\n");
    
    // Basic data types
    char character = 'A';
    int integer = 42;
    float floating_point = 3.14f;
    double double_precision = 3.14159265359;
    _Bool boolean = 1;  // C99 boolean
    bool modern_boolean = true;  // Using stdbool.h
    
    // Fixed-width integer types from stdint.h
    int8_t fixed_int8 = 127;
    uint8_t fixed_uint8 = 255;
    int16_t fixed_int16 = 32767;
    uint16_t fixed_uint16 = 65535;
    int32_t fixed_int32 = 2147483647;
    uint32_t fixed_uint32 = 4294967295u;
    
    // Variable scope demonstration
    {
        int block_scope_var = 50;
        printf("Block scope variable: %d\n", block_scope_var);
    }
    // block_scope_var is not accessible here
    
    // Constants
    const int constant_integer = 100;
    
    // Display all variables
    printf("character: %c (ASCII: %d)\n", character, character);
    printf("integer: %d\n", integer);
    printf("floating_point: %f\n", floating_point);
    printf("double_precision: %.10lf\n", double_precision);
    printf("boolean: %d\n", boolean);
    printf("modern_boolean: %d\n", modern_boolean);
    printf("fixed_int8: %d\n", fixed_int8);
    printf("fixed_uint8: %u\n", fixed_uint8);
    printf("fixed_int16: %d\n", fixed_int16);
    printf("fixed_uint16: %u\n", fixed_uint16);
    printf("fixed_int32: %d\n", fixed_int32);
    printf("fixed_uint32: %u\n", fixed_uint32);
    printf("constant_integer: %d\n", constant_integer);
    printf("global_var: %d\n", global_var);
    
    // Type conversion (casting)
    int int_value = 65;
    char char_value = (char)int_value;
    float float_value = 3.99f;
    int truncated = (int)float_value;
    
    printf("int_value as character: %c\n", char_value);
    printf("float_value truncated to int: %d\n", truncated);
}

void demonstrate_operators(void) {
    printf("\n==== OPERATORS ====\n");
    
    // Arithmetic operators
    int a = 10, b = 3;
    printf("a + b = %d\n", a + b);
    printf("a - b = %d\n", a - b);
    printf("a * b = %d\n", a * b);
    printf("a / b = %d\n", a / b);  // Integer division
    printf("a %% b = %d\n", a % b);  // Modulus/remainder
    
    // Increment and decrement
    int c = 5;
    printf("c = %d\n", c);
    printf("++c = %d\n", ++c);  // Pre-increment
    printf("c++ = %d\n", c++);  // Post-increment
    printf("c = %d\n", c);
    printf("--c = %d\n", --c);  // Pre-decrement
    printf("c-- = %d\n", c--);  // Post-decrement
    printf("c = %d\n", c);
    
    // Relational operators
    printf("a == b: %d\n", a == b);
    printf("a != b: %d\n", a != b);
    printf("a > b: %d\n", a > b);
    printf("a < b: %d\n", a < b);
    printf("a >= b: %d\n", a >= b);
    printf("a <= b: %d\n", a <= b);
    
    // Logical operators
    bool x = true, y = false;
    printf("x && y (AND): %d\n", x && y);
    printf("x || y (OR): %d\n", x || y);
    printf("!x (NOT): %d\n", !x);
    
    // Bitwise operators
    unsigned int d = 60;  // 0011 1100
    unsigned int e = 13;  // 0000 1101
    
    printf("d & e (AND): %u\n", d & e);   // 0000 1100 = 12
    printf("d | e (OR): %u\n", d | e);    // 0011 1101 = 61
    printf("d ^ e (XOR): %u\n", d ^ e);   // 0011 0001 = 49
    printf("~d (NOT): %u\n", ~d);         // 1100 0011
    printf("d << 2 (Left Shift): %u\n", d << 2);  // 1111 0000 = 240
    printf("d >> 2 (Right Shift): %u\n", d >> 2); // 0000 1111 = 15
    
    // Assignment operators
    int f = 10;
    f += 5;  // f = f + 5
    printf("f += 5: %d\n", f);
    f -= 3;  // f = f - 3
    printf("f -= 3: %d\n", f);
    f *= 2;  // f = f * 2
    printf("f *= 2: %d\n", f);
    f /= 4;  // f = f / 4
    printf("f /= 4: %d\n", f);
    f %= 3;  // f = f % 3
    printf("f %%= 3: %d\n", f);
    
    // Conditional (ternary) operator
    int max = (a > b) ? a : b;
    printf("max of a and b: %d\n", max);
    
    // Comma operator
    int g = (f = 3, f + 2);
    printf("g = (f = 3, f + 2): %d\n", g);
    
    // Sizeof operator
    printf("Size of int: %zu bytes\n", sizeof(int));
    printf("Size of float: %zu bytes\n", sizeof(float));
    printf("Size of double: %zu bytes\n", sizeof(double));
    printf("Size of char: %zu bytes\n", sizeof(char));
}

void demonstrate_control_flow(void) {
    printf("\n==== CONTROL FLOW ====\n");
    
    // if-else statement
    int num = 10;
    
    printf("if-else demo with num = %d:\n", num);
    if (num > 0) {
        printf("  Number is positive\n");
    } else if (num < 0) {
        printf("  Number is negative\n");
    } else {
        printf("  Number is zero\n");
    }
    
    // switch statement
    char grade = 'B';
    
    printf("switch demo with grade = %c:\n", grade);
    switch (grade) {
        case 'A':
            printf("  Excellent!\n");
            break;
        case 'B':
            printf("  Good job!\n");
            break;
        case 'C':
            printf("  Satisfactory\n");
            break;
        case 'D':
            printf("  Needs improvement\n");
            break;
        case 'F':
            printf("  Failed\n");
            break;
        default:
            printf("  Invalid grade\n");
    }
    
    // for loop
    printf("for loop demo:\n");
    for (int i = 0; i < 5; i++) {
        printf("  i = %d\n", i);
    }
    
    // while loop
    printf("while loop demo:\n");
    int j = 0;
    while (j < 5) {
        printf("  j = %d\n", j);
        j++;
    }
    
    // do-while loop
    printf("do-while loop demo:\n");
    int k = 0;
    do {
        printf("  k = %d\n", k);
        k++;
    } while (k < 5);
    
    // break and continue
    printf("break and continue demo:\n");
    for (int i = 0; i < 10; i++) {
        if (i == 3) {
            printf("  Skipping 3 (continue)\n");
            continue;
        }
        if (i == 7) {
            printf("  Breaking at 7 (break)\n");
            break;
        }
        printf("  i = %d\n", i);
    }
    
    // goto statement (generally discouraged but included for completeness)
    printf("goto demo:\n");
    int counter = 0;
start:
    printf("  counter = %d\n", counter);
    counter++;
    if (counter < 3) {
        goto start;
    }
}

int factorial(int n) {
    // Base case
    if (n <= 1) {
        return 1;
    }
    // Recursive case
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void demonstrate_arrays(void) {
    printf("\n==== ARRAYS ====\n");
    
    // 1D array declaration and initialization
    int numbers[5] = {10, 20, 30, 40, 50};
    
    // Array traversal
    printf("Array elements:\n");
    for (int i = 0; i < 5; i++) {
        printf("  numbers[%d] = %d\n", i, numbers[i]);
    }
    
    // Array modification
    numbers[2] = 35;
    printf("Modified array:\n");
    for (int i = 0; i < 5; i++) {
        printf("  numbers[%d] = %d\n", i, numbers[i]);
    }
    
    // Multi-dimensional arrays
    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    
    printf("2D array (matrix):\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("  matrix[%d][%d] = %d\n", i, j, matrix[i][j]);
        }
    }
    
    // Array of structures
    Point points[3] = {{1, 2}, {3, 4}, {5, 6}};
    
    printf("Array of structures:\n");
    for (int i = 0; i < 3; i++) {
        printf("  points[%d] = (%d, %d)\n", i, points[i].x, points[i].y);
    }
    
    // Variable-length array (C99 feature)
    int size = 3;
    int vla[size];
    for (int i = 0; i < size; i++) {
        vla[i] = i * 10;
    }
    
    printf("Variable-length array:\n");
    for (int i = 0; i < size; i++) {
        printf("  vla[%d] = %d\n", i, vla[i]);
    }
}

void demonstrate_strings(void) {
    printf("\n==== STRINGS ====\n");
    
    // String declaration and initialization
    char greeting[] = "Hello, World!";
    char name[20] = "John";
    
    printf("greeting: %s\n", greeting);
    printf("name: %s\n", name);
    
    // String input and output
    // printf("Enter your name: ");
    // scanf("%19s", name);  // Limited to 19 chars to avoid buffer overflow
    // printf("Hello, %s!\n", name);
    
    // String functions from string.h
    printf("strlen(greeting): %zu\n", strlen(greeting));
    
    char destination[50];
    strcpy(destination, greeting);
    printf("strcpy result: %s\n", destination);
    
    strcat(destination, " Welcome!");
    printf("strcat result: %s\n", destination);
    
    printf("strcmp(\"apple\", \"banana\"): %d\n", strcmp("apple", "banana"));
    
    char *substring = strchr(greeting, 'W');
    if (substring) {
        printf("strchr found 'W' at position: %ld\n", substring - greeting);
    }
    
    char sentence[] = "This is a sample sentence";
    char *token = strtok(sentence, " ");
    printf("strtok tokenization:\n");
    while (token != NULL) {
        printf("  Token: %s\n", token);
        token = strtok(NULL, " ");
    }
    
    // String to number conversion
    char number_str[] = "12345";
    int number = atoi(number_str);
    printf("atoi(%s) = %d\n", number_str, number);
    
    char float_str[] = "123.45";
    float float_val = atof(float_str);
    printf("atof(%s) = %f\n", float_str, float_val);
    
    // Safer string functions (C11)
    #ifdef __STDC_LIB_EXT1__
    printf("C11 safer string functions are available\n");
    #else
    printf("C11 safer string functions not available\n");
    #endif
}

void demonstrate_pointers(void) {
    printf("\n==== POINTERS ====\n");
    
    // Basic pointer declaration and initialization
    int num = 42;
    int *ptr = &num;
    
    printf("num: %d\n", num);
    printf("&num (address): %p\n", (void*)&num);
    printf("ptr (holds address): %p\n", (void*)ptr);
    printf("*ptr (dereferenced value): %d\n", *ptr);
    
    // Changing value through pointer
    *ptr = 100;
    printf("After *ptr = 100, num: %d\n", num);
    
    // Pointer arithmetic
    int array[5] = {10, 20, 30, 40, 50};
    int *array_ptr = array;
    
    printf("Array pointer demonstration:\n");
    for (int i = 0; i < 5; i++) {
        printf("  *(array_ptr + %d) = %d\n", i, *(array_ptr + i));
    }
    
    // Pointer to pointer
    int value = 5;
    int *value_ptr = &value;
    int **ptr_to_ptr = &value_ptr;
    
    printf("Pointer to pointer:\n");
    printf("  value: %d\n", value);
    printf("  *value_ptr: %d\n", *value_ptr);
    printf("  **ptr_to_ptr: %d\n", **ptr_to_ptr);
    
    // NULL pointer
    int *null_ptr = NULL;
    printf("null_ptr: %p\n", (void*)null_ptr);
    
    // Void pointer
    void *void_ptr = &num;
    printf("void_ptr: %p\n", void_ptr);
    printf("*(int*)void_ptr: %d\n", *(int*)void_ptr);
    
    // Pointer to function (will be demonstrated later)
    
    // Swap two numbers using pointers
    int a = 5, b = 10;
    printf("Before swap: a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("After swap: a = %d, b = %d\n", a, b);
    
    // Pointer to constant and constant pointer
    const int c_val = 200;
    const int *ptr_to_const = &c_val;  // Pointer to constant (value cannot be changed)
    // *ptr_to_const = 300;  // Error: cannot modify a const value
    
    int non_const = 100;
    int * const const_ptr = &non_const;  // Constant pointer (pointer cannot be redirected)
    *const_ptr = 200;  // OK: value can be changed
    // const_ptr = &c_val;  // Error: cannot change pointer
    
    printf("Pointer to constant and constant pointer:\n");
    printf("  *ptr_to_const: %d\n", *ptr_to_const);
    printf("  *const_ptr: %d\n", *const_ptr);
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void demonstrate_dynamic_memory(void) {
    printf("\n==== DYNAMIC MEMORY ALLOCATION ====\n");
    
    // malloc - Allocates memory of specified size
    int *int_ptr = (int*)malloc(sizeof(int));
    if (int_ptr == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    *int_ptr = 42;
    printf("malloc demonstration:\n");
    printf("  *int_ptr: %d\n", *int_ptr);
    
    // calloc - Allocates memory and initializes to zero
    int *arr_ptr = (int*)calloc(5, sizeof(int));
    if (arr_ptr == NULL) {
        printf("Memory allocation failed\n");
        free(int_ptr);
        return;
    }
    
    printf("calloc demonstration (initialized to 0):\n");
    for (int i = 0; i < 5; i++) {
        printf("  arr_ptr[%d]: %d\n", i, arr_ptr[i]);
    }
    
    // Assigning values to allocated array
    for (int i = 0; i < 5; i++) {
        arr_ptr[i] = i * 10;
    }
    
    printf("After assignment:\n");
    for (int i = 0; i < 5; i++) {
        printf("  arr_ptr[%d]: %d\n", i, arr_ptr[i]);
    }
    
    // realloc - Reallocates memory to new size
    int *new_arr_ptr = (int*)realloc(arr_ptr, 10 * sizeof(int));
    if (new_arr_ptr == NULL) {
        printf("Memory reallocation failed\n");
        free(int_ptr);
        free(arr_ptr);
        return;
    }
    
    // Original data is preserved
    printf("realloc demonstration (resized to 10 elements):\n");
    for (int i = 0; i < 5; i++) {
        printf("  new_arr_ptr[%d]: %d\n", i, new_arr_ptr[i]);
    }
    
    // Add data to expanded section
    for (int i = 5; i < 10; i++) {
        new_arr_ptr[i] = i * 10;
    }
    
    printf("After adding data to expanded section:\n");
    for (int i = 5; i < 10; i++) {
        printf("  new_arr_ptr[%d]: %d\n", i, new_arr_ptr[i]);
    }
    
    // Dynamic 2D array allocation
    int rows = 3;
    int cols = 4;
    
    // Allocate memory for rows
    int **matrix = (int**)malloc(rows * sizeof(int*));
    if (matrix == NULL) {
        printf("Memory allocation failed\n");
        free(int_ptr);
        free(new_arr_ptr);
        return;
    }
    
    // Allocate memory for each row
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            printf("Memory allocation failed\n");
            
            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            free(int_ptr);
            free(new_arr_ptr);
            return;
        }
    }
    
    // Initialize 2D array
    printf("Dynamic 2D array demonstration:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j;
            printf("  matrix[%d][%d] = %d\n", i, j, matrix[i][j]);
        }
    }
    
    // Free all allocated memory
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(int_ptr);
    free(new_arr_ptr);
    
    printf("All dynamically allocated memory has been freed\n");
}

void demonstrate_structures(void) {
    printf("\n==== STRUCTURES, UNIONS, AND ENUMS ====\n");
    
    // Basic structure usage
    struct Person person1;
    strcpy(person1.name, "John Doe");
    person1.age = 30;
    person1.height = 1.75f;
    
    printf("Structure demo:\n");
    printf("  Person: %s, %d years old, %.2f meters tall\n", 
           person1.name, person1.age, person1.height);
    
    // Structure initialization
    struct Person person2 = {"Jane Smith", 25, 1.65f};
    printf("  Person: %s, %d years old, %.2f meters tall\n", 
           person2.name, person2.age, person2.height);
    
    // Nested structures
    typedef struct {
        struct Person person;
        int employee_id;
        float salary;
    } Employee;
    
    Employee employee = {{"Bob Johnson", 35, 1.80f}, 12345, 50000.0f};
    
    printf("Nested structure demo:\n");
    printf("  Employee: %s (ID: %d), %d years old, %.2f meters tall, $%.2f salary\n",
           employee.person.name, employee.employee_id, 
           employee.person.age, employee.person.height, employee.salary);
    
    // Union demo
    union Data data;
    
    data.i = 10;
    printf("Union demo:\n");
    printf("  data.i: %d\n", data.i);
    
    data.f = 220.5f;
    printf("  data.f: %.1f\n", data.f);
    
    strcpy(data.str, "C Programming");
    printf("  data.str: %s\n", data.str);
    
    // Enumeration example
    enum Weekday today = WEDNESDAY;
    
    printf("Enumeration demo:\n");
    printf("  Today is day %d of the week\n", today);
    
    // Switch with enum
    switch (today) {
        case MONDAY:
            printf("  It's Monday\n");
            break;
        case TUESDAY:
            printf("  It's Tuesday\n");
            break;
        case WEDNESDAY:
            printf("  It's Wednesday\n");
            break;
        case THURSDAY:
            printf("  It's Thursday\n");
            break;
        case FRIDAY:
            printf("  It's Friday\n");
            break;
        case SATURDAY:
        case SUNDAY:
            printf("  It's the weekend\n");
            break;
        default:
            printf("  Invalid day\n");
    }
    
    // Bit fields
    typedef struct {
        unsigned int is_active : 1;    // 1 bit
        unsigned int permission : 3;   // 3 bits (0-7)
        unsigned int user_level : 4;   // 4 bits (0-15)
    } Flags;
    
    Flags flags = {1, 5, 10};
    
    printf("Bit fields demo:\n");
    printf("  is_active: %u\n", flags.is_active);
    printf("  permission: %u\n", flags.permission);
    printf("  user_level: %u\n", flags.user_level);
    printf("  Size of Flags struct: %zu bytes\n", sizeof(Flags));
}

void demonstrate_file_operations(void) {
    printf("\n==== FILE I/O OPERATIONS ====\n");
    
    // File writing
    FILE *file_ptr = fopen("demo.txt", "w");
    if (file_ptr == NULL) {
        printf("Failed to open file for writing\n");
        return;
    }
    
    fprintf(file_ptr, "Hello, File I/O!\n");
    fprintf(file_ptr, "This is a demonstration of file operations in C.\n");
    fprintf(file_ptr, "Numbers: %d, %f\n", 42, 3.14);
    
    fclose(file_ptr);
    printf("File written successfully\n");
    
    // File reading
    file_ptr = fopen("demo.txt", "r");
    if (file_ptr == NULL) {
        printf("Failed to open file for reading\n");
        return;
    }
    
    printf("File contents:\n");
    
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file_ptr) != NULL) {
        printf("  %s", buffer);
    }
    
    fclose(file_ptr);
    
    // Binary file operations
    typedef struct {
        int id;
        char name[50];
        float score;
    } Record;
    
    Record records[3] = {
        {1, "Alice", 95.5f},
        {2, "Bob", 89.0f},
        {3, "Charlie", 78.5f}
    };
    
    // Write binary data
    file_ptr = fopen("records.bin", "wb");
    if (file_ptr == NULL) {
        printf("Failed to open binary file for writing\n");
        return;
    }
    
    fwrite(records, sizeof(Record), 3, file_ptr);
    fclose(file_ptr);
    printf("Binary data written successfully\n");
    
    // Read binary data
    Record read_records[3];
    file_ptr = fopen("records.bin", "rb");
    if (file_ptr == NULL) {
        printf("Failed to open binary file for reading\n");
        return;
    }
    
    fread(read_records, sizeof(Record), 3, file_ptr);
    fclose(file_ptr);
    
    printf("Binary data read:\n");
    for (int i = 0; i < 3; i++) {
        printf("  Record %d: %s, Score: %.1f\n", 
               read_records[i].id, read_records[i].name, read_records[i].score);
    }
    
    // Random access
    file_ptr = fopen("records.bin", "rb");
    if (file_ptr == NULL) {
        printf("Failed to open binary file for random access\n");
        return;
    }
    
    // Seek to the second record
    fseek(file_ptr, sizeof(Record), SEEK_SET);
    
    Record second_record;
    fread(&second_record, sizeof(Record), 1, file_ptr);
    
    printf("Random access - Second record:\n");
    printf("  ID: %d, Name: %s, Score: %.1f\n", 
           second_record.id, second_record.name, second_record.score);
    
    fclose(file_ptr);
    
    // Remove temporary files
    remove("demo.txt");
    remove("records.bin");
    printf("Temporary files removed\n");
}

// Function for function pointer demonstration
void callback_function(int value) {
    printf("  Callback called with value: %d\n", value);
}

void demonstrate_function_pointers(void) {
    printf("\n==== FUNCTION POINTERS ====\n");
    
    // Declare a function pointer
    void (*func_ptr)(int);
    
    // Assign address of function to pointer
    func_ptr = callback_function;
    
    // Call function through pointer
    printf("Calling function through pointer:\n");
    func_ptr(42);
    
    // Array of function pointers
    void (*func_array[3])(int);
    for (int i = 0; i < 3; i++) {
        func_array[i] = callback_function;
    }
    
    printf("Calling functions from array of function pointers:\n");
    for (int i = 0; i < 3; i++) {
        func_array[i](i * 10);
    }
    
    // Function pointers as arguments to functions
    int numbers[] = {5, 2, 8, 1, 9, 3};
    int size = sizeof(numbers) / sizeof(numbers[0]);
    
    printf("Original array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // Implementing a simple sorting algorithm using function pointers
    // for comparison (this is similar to how qsort works)
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            // Ascending order comparison
            if (numbers[j] > numbers[j + 1]) {
                // Swap
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }
    
    printf("Sorted array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // Using standard library qsort
    // This would require implementing a comparison function
    // int compare(const void *a, const void *b) {
    //     return (*(int*)a - *(int*)b);
    // }
    // qsort(numbers, size, sizeof(int), compare);
}

void demonstrate_preprocessor(void) {
    printf("\n==== PREPROCESSOR DIRECTIVES ====\n");
    
    // Using macros
    printf("Macro PI: %.5f\n", PI);
    printf("SQUARE(5): %d\n", SQUARE(5));
    printf("MAX(10, 20): %d\n", MAX(10, 20));
    
    int x = 42;
    PRINT_VAR(x);
    
    // Conditional compilation
    #ifdef DEBUG
        printf("Debug mode is ON\n");
    #else
        printf("Debug mode is OFF\n");
    #endif
    
    // Checking standard version
    #if __STDC_VERSION__ >= 201112L
        printf("Using C11 or later\n");
    #elif __STDC_VERSION__ >= 199901L
        printf("Using C99\n");
    #else
        printf("Using C89/C90 or earlier\n");
    #endif
    
    // The __FILE__ and __LINE__ predefined macros
    printf("Current file: %s\n", __FILE__);
    printf("Current line: %d\n", __LINE__);
    printf("Compilation date: %s\n", __DATE__);
    printf("Compilation time: %s\n", __TIME__);
    
    // Stringizing operator (#)
    #define STR(x) #x
    printf("Stringizing operator: %s\n", STR(Hello World));
    
    // Token-pasting operator (##)
    #define CONCAT(x, y) x##y
    int var12 = 100;
    printf("Token-pasting: %d\n", CONCAT(var, 12));
    
    // #undef example
    #define TEMP_VALUE 100
    printf("TEMP_VALUE: %d\n", TEMP_VALUE);
    
    #undef TEMP_VALUE
    // printf("TEMP_VALUE: %d\n", TEMP_VALUE);  // Would cause an error
    
    // #error directive (commented out to allow compilation)
    // #if __STDC_VERSION__ < 199901L
    //     #error This program requires C99 or later
    // #endif
}

#include <stdarg.h>
void demonstrate_variadic_functions(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    printf("Variadic function demonstration:\n");
    printf("  Format string: %s\n", format);
    
    // Process format string
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            switch (format[i+1]) {
                case 'd': {
                    int value = va_arg(args, int);
                    printf("  Integer argument: %d\n", value);
                    break;
                }
                case 'f': {
                    double value = va_arg(args, double);
                    printf("  Float argument: %.2f\n", value);
                    break;
                }
                case 's': {
                    char *value = va_arg(args, char*);
                    printf("  String argument: %s\n", value);
                    break;
                }
            }
            i++; // Skip the format specifier
        }
    }
    
    va_end(args);
}

/** 
void error_handling_demo(void) {
    printf("\n==== ERROR HANDLING ====\n");
    
    // Returning error codes
    FILE *file = fopen("nonexistent.txt", "r");
    if (file == NULL) {
        //printf("Error opening file: %s\n", strerror(errno));
        printf("Error opening file: ");
    }
    
    // Using assert
    printf("Assert demonstration:\n");
    int x = 5;
    assert(x > 0);  // This will pass
    printf("  Assert passed: x > 0\n");
    
    // perror example
    printf("perror demonstration:\n");
    perror("Error message");
    
    // setjmp/longjmp for exception-like behaviour
    printf("setjmp/longjmp demonstration:\n");
    //jmp_buf jump_buffer;
    //int jump_value = setjmp(jump_buffer);
    
    //if (jump_value == 0) {
    //    printf("  First time through\n");
    //    // Simulate an error condition
    //    longjmp(jump_buffer, 1);
    //    printf("  This line will not be executed\n");
    //} else {
    //    printf("  Jumped back with value: %d\n", jump_value);
    }
    
    // signal handling
    printf("Signal handling demonstration:\n");
    printf("  (SIGINT handler would be set up here in a real program)\n");
    
    // try/catch is not available in standard C
    printf("  Note: C does not have try/catch exception handling like C++\n");
}
**/
