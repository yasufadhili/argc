import { CompilationResult } from '../types/compiler.types';

// For the MVP, we'll implement a mock compiler service
// In the real implementation, this would use the WebAssembly-based Argon compiler or API to a remote server running argon
export class ArgonCompiler {
  private static instance: ArgonCompiler;
  
  private constructor() {}
  
  public static getInstance(): ArgonCompiler {
    if (!ArgonCompiler.instance) {
      ArgonCompiler.instance = new ArgonCompiler();
    }
    return ArgonCompiler.instance;
  }
  
  public async compile(code: string): Promise<CompilationResult> {
    // This is a mock implementation for the MVP
    // In a future implementation, this would compile the code to WASM or request to Argon server
    
    // Check for simple syntax errors
    const hasOpeningBrace = code.includes('{');
    const hasClosingBrace = code.includes('}');
    
    if (hasOpeningBrace && !hasClosingBrace) {
      return {
        success: false,
        error: 'Syntax error: Missing closing brace',
      };
    }
    
    // Check for main function
    const hasMainFunction = code.includes('fn main()') || code.includes('fn main(');
    
    if (!hasMainFunction) {
      return {
        success: false,
        error: 'Error: No main function found',
      };
    }
    
    // Mock assembly output
    const assembly = `
; Assembly output for Argon code
section .text
    global _start

_start:
    call main
    mov eax, 60
    xor edi, edi
    syscall

main:
    push rbp
    mov rbp, rsp
    
    ; Print "Hello, Argon!"
    mov rax, 1
    mov rdi, 1
    lea rsi, [rel hello_msg]
    mov rdx, 14
    syscall
    
    mov rsp, rbp
    pop rbp
    ret

section .data
hello_msg: db "Hello, Argon!", 10
`;

    // Mock memory map
    const memoryMap = {
      stack: [
        { address: '0x7ffe123456', size: 8, name: 'return_address', value: '0x401234', type: 'pointer' },
        { address: '0x7ffe12345e', size: 8, name: 'saved_rbp', value: '0x7ffe123470', type: 'pointer' },
      ],
      heap: [
        { address: '0x55deadbeef', size: 24, name: 'string_buffer', value: '"Hello, Argon!"', type: 'char[]' },
      ],
      globals: [
        { address: '0x404000', size: 14, name: 'hello_msg', value: '"Hello, Argon!"', type: 'const char[]' },
      ],
    };
    
    // Simulate compilation and execution delay
    await new Promise(resolve => setTimeout(resolve, 1000));
    
    // Extract print statements for mock output
    let output = '';
    const printRegex = /println\(['"](.*?)['"]\)/g;
    let match;
    
    while ((match = printRegex.exec(code)) !== null) {
      output += match[1] + '\n';
    }
    
    if (!output) {
      output = 'Program executed successfully with no output';
    }
    
    return {
      success: true,
      output,
      assembly,
      memoryMap,
    };
  }
}