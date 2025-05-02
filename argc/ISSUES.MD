# Current Issues and Improvements for the Argon Compiler

- **Un uniform error handling:**
 Currently, there exist different error handlers for the include handler, lexer, parser, and the visitors.
 I makes little sense although, errors that arise even before the file is opened, like invalid options, filepaths, and so on do seem to work moderately ok.
  
  However, we could need to have a uniform file handler once the lexing begins, All these errors should be reported to the error handler, then handled appropriately as follows.
    - Lexical Errors: Terminate compilation and report the error
    - Invalid Include paths, terminate compilation
    - Syntax errors, Attempt recovery
    - Logical errors, report them and fallback to a more suitable expression (This will cause furhter errors where this expression or it's result is needed)
    - Type mismatch : Attempt recovery and continue

  All errors should be logged to the console either after all stages are finished, a fatal error occurs, or any of the errors that cause continuing not to be the best option

- **File Names in error reporting:**
Currently the include handler reports the filename too, but nothing other error handler don't

- **Should add snippets for the location of errors:**

