section .data

; Constants
EXIT_SUCCESS      equ      0 ; successful operation
SYS_EXIT          equ      0 ; Call code for terminate

; Byte (8-bit) variable declarations
bVar1             db       17
bVar2             db       9
bResult           db       0

; Word (16-bit) variable declarations
wVar1             dw        17000
wVar2             dw        9000
wResult           dw        0

; Duoble-word (32-bit) variable declarations
dVar1             dd        17000000
dVar2             dd        9000000
dResult           dd        0

; Quadword (64-bit) variable declarations
qVar1             dq        170000000
qVar2             dq        90000000
qResult           dq        0

section .text
global _start

_start:
  ; Byte
  ; bResult = bVar1 + bVar2
  mov al, byte [bVar1]
  add al, byte [bVar2]
  mov byte [bResult], al

  ; Word
  ; wResult = wVar1 + wVar2
  mov ax, word [wVar1]
  add ax, word [wVar2]
  mov word [wResult], ax

  ; Double Word
  ; dResult = dVar1 + dVar2
  mov eax, dword [dVar1]
  add eax, dword [dVar2]
  mov dword [dResult], eax

  ; Quad Word
  ; qResult = qVar1 + qVar2
  mov rax, qword [qVar1]
  add rax, qword [qVar2]
  mov qword [qResult], rax

last:
  mov rax, SYS_EXIT
  mov rdi, EXIT_SUCCESS
  syscall