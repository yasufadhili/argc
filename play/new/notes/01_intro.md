

## Language Processors

- A compiler is a program that can read a program in one language | the source language | and translate it into an equivalent program in  another language | the target language
- An interpreter is another common kind of language processor. Instead of
  producing a target program as a translation, an interpreter appears to directly
  execute the operations specified in the source program on inputs supplied by
  the user


## Terms

- Compiler
- Interpreter
- Translator
- Preprocessor
- Generator
- Reader
- Loader
- Assembler

```mermaid



```


Source --> Preprocessor -> Compiler -> Assembler -> Linker/Loader --> Machine Code


## Compiler

Mapping:
- Analysis
- Synthesis

Input -> Lexical Analyser -> Syntax Analyser -> Semantic Analyser -> Intermediate Code Generator -> Machine-Independent Code Optimiser -> Code Generator -> Machine-Dependant Code Optimiser
(Char Stream) -> (Token Stream) -> (Syntax Tree) -> (Syntax Tree) -> IR -> IR -> Target Machine Code -> Target Machine Code

Symbol table is used at all phases

### Lexical Analysis / Scanning

Lexical Analyser Reads the source input stream and groups the characters into meaningful sequences called ``lexemes``. For each
lexeme, the lexical analyser produces as output a token of the form 

<token-name, attribute-value>

Token 
    - Token Name
    - Attribute Value

### Syntax Analysis / Parsing

- Represents in a Syntax Tree

### Semantic Analysis

- Type checking

### Intermediate Code Generation
- Three-address code

### Symbol Table Management

The symbol table is a data structure containing a record for each variable  name, with fields for the attributes of the name.
The data structure should be designed to allow the compiler to find the record for each name quickly and to store or retrieve data from that record quickly


## Concepts
- Finite State Machines
- Regular Expressions
- Context Free Grammars
- Trees
