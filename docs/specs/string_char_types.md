# String and Character Types Specification

## Character Type (Rune)

### UTF-8 Encoding
- Characters in Argon are UTF-8 encoded Unicode code points
- Type name: `rune`
- Size: 4 bytes (32-bit)
- Range: 0 to 0x10FFFF (all valid Unicode code points)

### Character Literals
```argon
'a'              // ASCII character
'げ'              // Hiragana
'🦀'              // Emoji
'\n'             // Newline
'\r'             // Carriage return
'\t'             // Tab
'\\'             // Backslash
'\''             // Single quote
'\u0041'         // Unicode escape (exactly 4 hex digits)
'\U0001F600'     // Unicode escape (exactly 8 hex digits)
```

### Character Operations
- Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
- Conversion to integer: `char as i32`
- Conversion from integer: `65 as rune`

## String Type

### Properties
- Type name: `str`
- UTF-8 encoded sequence of bytes
- Immutable by default
- Length measured in:
  - Bytes: `len_bytes()`
  - Code points: `len_runes()`
  - Grapheme clusters: `len_graphemes()`

### String Literals
```argon
// Regular string literal
"Hello, World!"

// Raw string literal (no escape processing)
`Line 1
Line 2
Special chars: \n \t`

// Unicode string literal
"Hello, 世界"

// Escape sequences
"Quotes: \"Hello\"\nNew line"
```

### String Operations

#### Concatenation
```argon
var s1 str = "Hello"
var s2 str = "World"
var s3 str = s1 + " " + s2    // "Hello World"
```

#### Slicing
```argon
var s str = "Hello"
var slice str = s[1:4]        // "ell"
var char rune = s[0]          // 'H'
```

#### Iteration
```argon
// Iterate over code points
for c in str {
    // c is rune type
}

// Iterate over bytes
for b in str.bytes() {
    // b is u8 type
}

// Iterate over graphemes
for g in str.graphemes() {
    // g is str type
}
```

### String Methods

#### Basic Operations
- `len_bytes()`: Length in bytes
- `len_runes()`: Length in Unicode code points
- `len_graphemes()`: Length in grapheme clusters
- `is_empty()`: Check if string is empty
- `clone()`: Create a deep copy
- `as_bytes()`: Get byte slice view
- `as_runes()`: Get code point slice view

#### Search and Compare
- `contains(substr str) bool`
- `starts_with(prefix str) bool`
- `ends_with(suffix str) bool`
- `index_of(substr str) i64`
- `last_index_of(substr str) i64`
- `compare(other str) i8`

#### Transformation
- `to_upper()`: Convert to uppercase
- `to_lower()`: Convert to lowercase
- `trim()`: Remove whitespace from both ends
- `trim_start()`: Remove whitespace from start
- `trim_end()`: Remove whitespace from end
- `replace(old str, new str) str`
- `split(sep str) []str`
- `join(strs []str, sep str) str`

### String Formatting
```argon
// Using string interpolation
var name str = "World"
var greeting str = f"Hello, {name}!"

// Using format function
var x i32 = 42
var s str = format("Value: {:d}", x)
```

## Error Handling

### Runtime Checks
1. Invalid UTF-8 sequences
2. String index out of bounds
3. Invalid Unicode code points
4. Invalid string slicing ranges

### Compile-time Checks
1. String literal validation
2. Unicode escape sequence validation
3. String concatenation type checking

## Memory Management

### String Internals
- Strings are reference counted
- Copy-on-write semantics
- UTF-8 validation on creation
- Cached length and hash values

### Performance Considerations
1. Small String Optimisation (SSO)
   - Strings under 24 bytes stored inline
   - No heap allocation for short strings
   
2. Memory Layout
   ```
   String {
       ptr *u8,        // Pointer to data
       len usize,      // Length in bytes
       capacity usize  // Allocated capacity
   }
   ```

3. Optimisation Tips
   - Preallocate for large string operations
   - Use raw strings for large literals
   - Use string builder for multiple concatenations
