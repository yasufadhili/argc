# macros.asm - A reusable assembly macro library

# System call constants
.equ SYS_READ,   0
.equ SYS_WRITE,  1
.equ SYS_OPEN,   2
.equ SYS_CLOSE,  3
.equ SYS_LSEEK,  8
.equ SYS_EXIT,   60

# File access flags
.equ O_RDONLY,   0
.equ O_WRONLY,   1
.equ O_RDWR,     2
.equ O_CREAT,    64
.equ O_TRUNC,    512
.equ O_APPEND,   1024

# File permissions
.equ S_IRUSR,    400
.equ S_IWUSR,    200
.equ S_IXUSR,    100
.equ S_IRGRP,    40
.equ S_IWGRP,    20
.equ S_IXGRP,    10
.equ S_IROTH,    4
.equ S_IWOTH,    2
.equ S_IXOTH,    1

# Basic system call wrapper
.macro SYSCALL nr, arg1=0, arg2=0, arg3=0, arg4=0, arg5=0, arg6=0
    movq $\nr, %rax
    movq \arg1, %rdi
    movq \arg2, %rsi
    movq \arg3, %rdx
    movq \arg4, %r10
    movq \arg5, %r8
    movq \arg6, %r9
    syscall
.endm

# File operations
.macro OPEN file, flags, mode=0
    SYSCALL SYS_OPEN, $\file, $\flags, $\mode
.endm

.macro READ fd, buffer, count
    SYSCALL SYS_READ, \fd, $\buffer, \count
.endm

.macro WRITE fd, buffer, count
    SYSCALL SYS_WRITE, \fd, $\buffer, \count
.endm

.macro CLOSE fd
    SYSCALL SYS_CLOSE, \fd
.endm

.macro LSEEK fd, offset, whence
    SYSCALL SYS_LSEEK, \fd, \offset, \whence
.endm

.macro EXIT code
    SYSCALL SYS_EXIT, \code
.endm

# String operations
.macro STRLEN str, reg
    pushq %rdi
    pushq %rcx
    
    leaq \str(%rip), %rdi
    movq $-1, %rcx
    xorb %al, %al
    repne scasb
    notq %rcx
    decq %rcx
    movq %rcx, \reg
    
    popq %rcx
    popq %rdi
.endm

.macro PRINT str
    STRLEN \str, %r15
    WRITE $1, \str, %r15
.endm

# Memory allocation helpers
.macro PUSH_ALL
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
.endm

.macro POP_ALL
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
.endm

# Control flow macros
.macro IF_EQ val1, val2, label
    cmpq \val2, \val1
    je \label
.endm

.macro IF_NE val1, val2, label
    cmpq \val2, \val1
    jne \label
.endm

.macro IF_LT val1, val2, label
    cmpq \val2, \val1
    jl \label
.endm

.macro IF_GT val1, val2, label
    cmpq \val2, \val1
    jg \label
.endm

# Function prologue and epilogue
.macro FUNC_PROLOG frame_size=0
    pushq %rbp
    movq %rsp, %rbp
    .if \frame_size > 0
        subq $\frame_size, %rsp
    .endif
.endm

.macro FUNC_EPILOG
    movq %rbp, %rsp
    popq %rbp
    ret
.endm
