	.file	"demo.c"
	.text
	.globl	global_var
	.data
	.align 4
	.type	global_var, @object
	.size	global_var, 4
global_var:
	.long	100
	.section	.rodata
	.align 8
.LC0:
	.string	"==== C LANGUAGE FEATURES DEMONSTRATION ====\n"
	.align 8
.LC1:
	.string	"\n==== RECURSION DEMONSTRATION ===="
.LC2:
	.string	"Factorial of 5: %d\n"
.LC3:
	.string	"Fibonacci(10): %d\n"
	.align 8
.LC4:
	.string	"\n==== END OF DEMONSTRATION ===="
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	call	demonstrate_variables
	call	demonstrate_operators
	call	demonstrate_control_flow
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$5, %edi
	call	factorial
	movl	%eax, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$10, %edi
	call	fibonacci
	movl	%eax, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	call	demonstrate_arrays
	call	demonstrate_strings
	call	demonstrate_pointers
	call	demonstrate_dynamic_memory
	call	demonstrate_structures
	call	demonstrate_file_operations
	call	demonstrate_function_pointers
	call	demonstrate_preprocessor
	call	error_handling_demo@PLT
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC5:
	.string	"\n==== VARIABLES AND DATA TYPES ===="
.LC8:
	.string	"Block scope variable: %d\n"
.LC9:
	.string	"character: %c (ASCII: %d)\n"
.LC10:
	.string	"integer: %d\n"
.LC11:
	.string	"floating_point: %f\n"
.LC12:
	.string	"double_precision: %.10lf\n"
.LC13:
	.string	"boolean: %d\n"
.LC14:
	.string	"modern_boolean: %d\n"
.LC15:
	.string	"fixed_int8: %d\n"
.LC16:
	.string	"fixed_uint8: %u\n"
.LC17:
	.string	"fixed_int16: %d\n"
.LC18:
	.string	"fixed_uint16: %u\n"
.LC19:
	.string	"fixed_int32: %d\n"
.LC20:
	.string	"fixed_uint32: %u\n"
.LC21:
	.string	"constant_integer: %d\n"
.LC22:
	.string	"global_var: %d\n"
.LC24:
	.string	"int_value as character: %c\n"
	.align 8
.LC25:
	.string	"float_value truncated to int: %d\n"
	.text
	.globl	demonstrate_variables
	.type	demonstrate_variables, @function
demonstrate_variables:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movb	$65, -54(%rbp)
	movl	$42, -44(%rbp)
	movss	.LC6(%rip), %xmm0
	movss	%xmm0, -40(%rbp)
	movsd	.LC7(%rip), %xmm0
	movsd	%xmm0, -8(%rbp)
	movb	$1, -53(%rbp)
	movb	$1, -52(%rbp)
	movb	$127, -51(%rbp)
	movb	$-1, -50(%rbp)
	movw	$32767, -48(%rbp)
	movw	$-1, -46(%rbp)
	movl	$2147483647, -36(%rbp)
	movl	$-1, -32(%rbp)
	movl	$50, -28(%rbp)
	movl	-28(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC8(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$100, -24(%rbp)
	movsbl	-54(%rbp), %edx
	movsbl	-54(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC9(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-44(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC10(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	pxor	%xmm1, %xmm1
	cvtss2sd	-40(%rbp), %xmm1
	movq	%xmm1, %rax
	movq	%rax, %xmm0
	leaq	.LC11(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	movq	-8(%rbp), %rax
	movq	%rax, %xmm0
	leaq	.LC12(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	movzbl	-53(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC13(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movzbl	-52(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC14(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movsbl	-51(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC15(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movzbl	-50(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC16(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movswl	-48(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC17(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movzwl	-46(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC18(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-36(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC19(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-32(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC20(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-24(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC21(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	global_var(%rip), %eax
	movl	%eax, %esi
	leaq	.LC22(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$65, -20(%rbp)
	movl	-20(%rbp), %eax
	movb	%al, -49(%rbp)
	movss	.LC23(%rip), %xmm0
	movss	%xmm0, -16(%rbp)
	movss	-16(%rbp), %xmm0
	cvttss2sil	%xmm0, %eax
	movl	%eax, -12(%rbp)
	movsbl	-49(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC24(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-12(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC25(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	demonstrate_variables, .-demonstrate_variables
	.section	.rodata
.LC26:
	.string	"\n==== OPERATORS ===="
.LC27:
	.string	"a + b = %d\n"
.LC28:
	.string	"a - b = %d\n"
.LC29:
	.string	"a * b = %d\n"
.LC30:
	.string	"a / b = %d\n"
.LC31:
	.string	"a %% b = %d\n"
.LC32:
	.string	"c = %d\n"
.LC33:
	.string	"++c = %d\n"
.LC34:
	.string	"c++ = %d\n"
.LC35:
	.string	"--c = %d\n"
.LC36:
	.string	"c-- = %d\n"
.LC37:
	.string	"a == b: %d\n"
.LC38:
	.string	"a != b: %d\n"
.LC39:
	.string	"a > b: %d\n"
.LC40:
	.string	"a < b: %d\n"
.LC41:
	.string	"a >= b: %d\n"
.LC42:
	.string	"a <= b: %d\n"
.LC43:
	.string	"x && y (AND): %d\n"
.LC44:
	.string	"x || y (OR): %d\n"
.LC45:
	.string	"!x (NOT): %d\n"
.LC46:
	.string	"d & e (AND): %u\n"
.LC47:
	.string	"d | e (OR): %u\n"
.LC48:
	.string	"d ^ e (XOR): %u\n"
.LC49:
	.string	"~d (NOT): %u\n"
.LC50:
	.string	"d << 2 (Left Shift): %u\n"
.LC51:
	.string	"d >> 2 (Right Shift): %u\n"
.LC52:
	.string	"f += 5: %d\n"
.LC53:
	.string	"f -= 3: %d\n"
.LC54:
	.string	"f *= 2: %d\n"
.LC55:
	.string	"f /= 4: %d\n"
.LC56:
	.string	"f %%= 3: %d\n"
.LC57:
	.string	"max of a and b: %d\n"
.LC58:
	.string	"g = (f = 3, f + 2): %d\n"
.LC59:
	.string	"Size of int: %zu bytes\n"
.LC60:
	.string	"Size of float: %zu bytes\n"
.LC61:
	.string	"Size of double: %zu bytes\n"
.LC62:
	.string	"Size of char: %zu bytes\n"
	.text
	.globl	demonstrate_operators
	.type	demonstrate_operators, @function
demonstrate_operators:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	leaq	.LC26(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$10, -32(%rbp)
	movl	$3, -28(%rbp)
	movl	-32(%rbp), %edx
	movl	-28(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, %esi
	leaq	.LC27(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-32(%rbp), %eax
	subl	-28(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC28(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-32(%rbp), %eax
	imull	-28(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC29(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-32(%rbp), %eax
	cltd
	idivl	-28(%rbp)
	movl	%eax, %esi
	leaq	.LC30(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-32(%rbp), %eax
	cltd
	idivl	-28(%rbp)
	movl	%edx, %eax
	movl	%eax, %esi
	leaq	.LC31(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$5, -24(%rbp)
	movl	-24(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC32(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -24(%rbp)
	movl	-24(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC33(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-24(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -24(%rbp)
	movl	%eax, %esi
	leaq	.LC34(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-24(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC32(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	subl	$1, -24(%rbp)
	movl	-24(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC35(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-24(%rbp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -24(%rbp)
	movl	%eax, %esi
	leaq	.LC36(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-24(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC32(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-32(%rbp), %eax
	cmpl	-28(%rbp), %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC37(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-32(%rbp), %eax
	cmpl	-28(%rbp), %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC38(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-32(%rbp), %eax
	cmpl	-28(%rbp), %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC39(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-32(%rbp), %eax
	cmpl	-28(%rbp), %eax
	setl	%al
	movzbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC40(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-32(%rbp), %eax
	cmpl	-28(%rbp), %eax
	setge	%al
	movzbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC41(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-32(%rbp), %eax
	cmpl	-28(%rbp), %eax
	setle	%al
	movzbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC42(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movb	$1, -34(%rbp)
	movb	$0, -33(%rbp)
	cmpb	$0, -34(%rbp)
	je	.L5
	cmpb	$0, -33(%rbp)
	je	.L5
	movl	$1, %eax
	jmp	.L6
.L5:
	movl	$0, %eax
.L6:
	movl	%eax, %esi
	leaq	.LC43(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	cmpb	$0, -34(%rbp)
	jne	.L7
	cmpb	$0, -33(%rbp)
	je	.L8
.L7:
	movl	$1, %eax
	jmp	.L9
.L8:
	movl	$0, %eax
.L9:
	movl	%eax, %esi
	leaq	.LC44(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movzbl	-34(%rbp), %eax
	xorl	$1, %eax
	movzbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC45(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$60, -20(%rbp)
	movl	$13, -16(%rbp)
	movl	-20(%rbp), %eax
	andl	-16(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC46(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-20(%rbp), %eax
	orl	-16(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC47(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-20(%rbp), %eax
	xorl	-16(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC48(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-20(%rbp), %eax
	notl	%eax
	movl	%eax, %esi
	leaq	.LC49(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-20(%rbp), %eax
	sall	$2, %eax
	movl	%eax, %esi
	leaq	.LC50(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-20(%rbp), %eax
	shrl	$2, %eax
	movl	%eax, %esi
	leaq	.LC51(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$10, -12(%rbp)
	addl	$5, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC52(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	subl	$3, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC53(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	sall	-12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC54(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-12(%rbp), %eax
	leal	3(%rax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$2, %eax
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC55(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-12(%rbp), %edx
	movslq	%edx, %rax
	imulq	$1431655766, %rax, %rax
	shrq	$32, %rax
	movq	%rax, %rcx
	movl	%edx, %eax
	sarl	$31, %eax
	subl	%eax, %ecx
	movl	%ecx, %eax
	addl	%eax, %eax
	addl	%ecx, %eax
	subl	%eax, %edx
	movl	%edx, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC56(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-28(%rbp), %edx
	movl	-32(%rbp), %eax
	cmpl	%eax, %edx
	cmovge	%edx, %eax
	movl	%eax, -8(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC57(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$3, -12(%rbp)
	movl	-12(%rbp), %eax
	addl	$2, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC58(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$4, %esi
	leaq	.LC59(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$4, %esi
	leaq	.LC60(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$8, %esi
	leaq	.LC61(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, %esi
	leaq	.LC62(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	demonstrate_operators, .-demonstrate_operators
	.section	.rodata
.LC63:
	.string	"\n==== CONTROL FLOW ===="
.LC64:
	.string	"if-else demo with num = %d:\n"
.LC65:
	.string	"  Number is positive"
.LC66:
	.string	"  Number is negative"
.LC67:
	.string	"  Number is zero"
.LC68:
	.string	"switch demo with grade = %c:\n"
.LC69:
	.string	"  Excellent!"
.LC70:
	.string	"  Good job!"
.LC71:
	.string	"  Satisfactory"
.LC72:
	.string	"  Needs improvement"
.LC73:
	.string	"  Failed"
.LC74:
	.string	"  Invalid grade"
.LC75:
	.string	"for loop demo:"
.LC76:
	.string	"  i = %d\n"
.LC77:
	.string	"while loop demo:"
.LC78:
	.string	"  j = %d\n"
.LC79:
	.string	"do-while loop demo:"
.LC80:
	.string	"  k = %d\n"
.LC81:
	.string	"break and continue demo:"
.LC82:
	.string	"  Skipping 3 (continue)"
.LC83:
	.string	"  Breaking at 7 (break)"
.LC84:
	.string	"goto demo:"
.LC85:
	.string	"  counter = %d\n"
	.text
	.globl	demonstrate_control_flow
	.type	demonstrate_control_flow, @function
demonstrate_control_flow:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	leaq	.LC63(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$10, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC64(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	cmpl	$0, -4(%rbp)
	jle	.L11
	leaq	.LC65(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L12
.L11:
	cmpl	$0, -4(%rbp)
	jns	.L13
	leaq	.LC66(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L12
.L13:
	leaq	.LC67(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
.L12:
	movb	$66, -25(%rbp)
	movsbl	-25(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC68(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movsbl	-25(%rbp), %eax
	subl	$65, %eax
	cmpl	$5, %eax
	ja	.L14
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L16(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L16(%rip), %rdx
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L16:
	.long	.L20-.L16
	.long	.L19-.L16
	.long	.L18-.L16
	.long	.L17-.L16
	.long	.L14-.L16
	.long	.L15-.L16
	.text
.L20:
	leaq	.LC69(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L21
.L19:
	leaq	.LC70(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L21
.L18:
	leaq	.LC71(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L21
.L17:
	leaq	.LC72(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L21
.L15:
	leaq	.LC73(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L21
.L14:
	leaq	.LC74(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
.L21:
	leaq	.LC75(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -24(%rbp)
	jmp	.L22
.L23:
	movl	-24(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC76(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -24(%rbp)
.L22:
	cmpl	$4, -24(%rbp)
	jle	.L23
	leaq	.LC77(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -20(%rbp)
	jmp	.L24
.L25:
	movl	-20(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC78(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -20(%rbp)
.L24:
	cmpl	$4, -20(%rbp)
	jle	.L25
	leaq	.LC79(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -16(%rbp)
.L26:
	movl	-16(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC80(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -16(%rbp)
	cmpl	$4, -16(%rbp)
	jle	.L26
	leaq	.LC81(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -12(%rbp)
	jmp	.L27
.L32:
	cmpl	$3, -12(%rbp)
	jne	.L28
	leaq	.LC82(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L29
.L28:
	cmpl	$7, -12(%rbp)
	jne	.L30
	leaq	.LC83(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L31
.L30:
	movl	-12(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC76(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L29:
	addl	$1, -12(%rbp)
.L27:
	cmpl	$9, -12(%rbp)
	jle	.L32
.L31:
	leaq	.LC84(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -8(%rbp)
.L33:
	movl	-8(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC85(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -8(%rbp)
	cmpl	$2, -8(%rbp)
	jg	.L35
	jmp	.L33
.L35:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	demonstrate_control_flow, .-demonstrate_control_flow
	.globl	factorial
	.type	factorial, @function
factorial:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	cmpl	$1, -4(%rbp)
	jg	.L37
	movl	$1, %eax
	jmp	.L38
.L37:
	movl	-4(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %edi
	call	factorial
	imull	-4(%rbp), %eax
.L38:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	factorial, .-factorial
	.globl	fibonacci
	.type	fibonacci, @function
fibonacci:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset 3, -24
	movl	%edi, -20(%rbp)
	cmpl	$1, -20(%rbp)
	jg	.L40
	movl	-20(%rbp), %eax
	jmp	.L41
.L40:
	movl	-20(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %edi
	call	fibonacci
	movl	%eax, %ebx
	movl	-20(%rbp), %eax
	subl	$2, %eax
	movl	%eax, %edi
	call	fibonacci
	addl	%ebx, %eax
.L41:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	fibonacci, .-fibonacci
	.section	.rodata
.LC86:
	.string	"\n==== ARRAYS ===="
.LC87:
	.string	"Array elements:"
.LC88:
	.string	"  numbers[%d] = %d\n"
.LC89:
	.string	"Modified array:"
.LC90:
	.string	"2D array (matrix):"
.LC91:
	.string	"  matrix[%d][%d] = %d\n"
.LC92:
	.string	"Array of structures:"
.LC93:
	.string	"  points[%d] = (%d, %d)\n"
.LC94:
	.string	"Variable-length array:"
.LC95:
	.string	"  vla[%d] = %d\n"
	.text
	.globl	demonstrate_arrays
	.type	demonstrate_arrays, @function
demonstrate_arrays:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$168, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movq	%rsp, %rax
	movq	%rax, %rbx
	leaq	.LC86(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$10, -128(%rbp)
	movl	$20, -124(%rbp)
	movl	$30, -120(%rbp)
	movl	$40, -116(%rbp)
	movl	$50, -112(%rbp)
	leaq	.LC87(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -176(%rbp)
	jmp	.L43
.L44:
	movl	-176(%rbp), %eax
	cltq
	movl	-128(%rbp,%rax,4), %edx
	movl	-176(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC88(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -176(%rbp)
.L43:
	cmpl	$4, -176(%rbp)
	jle	.L44
	movl	$35, -120(%rbp)
	leaq	.LC89(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -172(%rbp)
	jmp	.L45
.L46:
	movl	-172(%rbp), %eax
	cltq
	movl	-128(%rbp,%rax,4), %edx
	movl	-172(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC88(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -172(%rbp)
.L45:
	cmpl	$4, -172(%rbp)
	jle	.L46
	movl	$1, -64(%rbp)
	movl	$2, -60(%rbp)
	movl	$3, -56(%rbp)
	movl	$4, -52(%rbp)
	movl	$5, -48(%rbp)
	movl	$6, -44(%rbp)
	movl	$7, -40(%rbp)
	movl	$8, -36(%rbp)
	movl	$9, -32(%rbp)
	leaq	.LC90(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -168(%rbp)
	jmp	.L47
.L50:
	movl	$0, -164(%rbp)
	jmp	.L48
.L49:
	movl	-164(%rbp), %eax
	movslq	%eax, %rcx
	movl	-168(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	addq	%rcx, %rax
	movl	-64(%rbp,%rax,4), %ecx
	movl	-164(%rbp), %edx
	movl	-168(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC91(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -164(%rbp)
.L48:
	cmpl	$2, -164(%rbp)
	jle	.L49
	addl	$1, -168(%rbp)
.L47:
	cmpl	$2, -168(%rbp)
	jle	.L50
	movl	$1, -96(%rbp)
	movl	$2, -92(%rbp)
	movl	$3, -88(%rbp)
	movl	$4, -84(%rbp)
	movl	$5, -80(%rbp)
	movl	$6, -76(%rbp)
	leaq	.LC92(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -160(%rbp)
	jmp	.L51
.L52:
	movl	-160(%rbp), %eax
	cltq
	movl	-92(%rbp,%rax,8), %ecx
	movl	-160(%rbp), %eax
	cltq
	movl	-96(%rbp,%rax,8), %edx
	movl	-160(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC93(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -160(%rbp)
.L51:
	cmpl	$2, -160(%rbp)
	jle	.L52
	movl	$3, -148(%rbp)
	movl	-148(%rbp), %eax
	movslq	%eax, %rdx
	subq	$1, %rdx
	movq	%rdx, -144(%rbp)
	cltq
	leaq	0(,%rax,4), %rdx
	movl	$16, %eax
	subq	$1, %rax
	addq	%rdx, %rax
	movl	$16, %esi
	movl	$0, %edx
	divq	%rsi
	imulq	$16, %rax, %rax
	subq	%rax, %rsp
	movq	%rsp, %rax
	addq	$3, %rax
	shrq	$2, %rax
	salq	$2, %rax
	movq	%rax, -136(%rbp)
	movl	$0, -156(%rbp)
	jmp	.L53
.L54:
	movl	-156(%rbp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	movl	%eax, %ecx
	movq	-136(%rbp), %rax
	movl	-156(%rbp), %edx
	movslq	%edx, %rdx
	movl	%ecx, (%rax,%rdx,4)
	addl	$1, -156(%rbp)
.L53:
	movl	-156(%rbp), %eax
	cmpl	-148(%rbp), %eax
	jl	.L54
	leaq	.LC94(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -152(%rbp)
	jmp	.L55
.L56:
	movq	-136(%rbp), %rax
	movl	-152(%rbp), %edx
	movslq	%edx, %rdx
	movl	(%rax,%rdx,4), %edx
	movl	-152(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC95(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -152(%rbp)
.L55:
	movl	-152(%rbp), %eax
	cmpl	-148(%rbp), %eax
	jl	.L56
	movq	%rbx, %rsp
	nop
	movq	-24(%rbp), %rax
	subq	%fs:40, %rax
	je	.L57
	call	__stack_chk_fail@PLT
.L57:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	demonstrate_arrays, .-demonstrate_arrays
	.section	.rodata
.LC96:
	.string	"\n==== STRINGS ===="
.LC97:
	.string	"greeting: %s\n"
.LC98:
	.string	"name: %s\n"
.LC99:
	.string	"strlen(greeting): %zu\n"
.LC100:
	.string	"strcpy result: %s\n"
.LC101:
	.string	"strcat result: %s\n"
	.align 8
.LC102:
	.string	"strcmp(\"apple\", \"banana\"): %d\n"
	.align 8
.LC103:
	.string	"strchr found 'W' at position: %ld\n"
.LC104:
	.string	" "
.LC105:
	.string	"strtok tokenization:"
.LC106:
	.string	"  Token: %s\n"
.LC107:
	.string	"atoi(%s) = %d\n"
.LC108:
	.string	"atof(%s) = %f\n"
	.align 8
.LC109:
	.string	"C11 safer string functions not available"
	.text
	.globl	demonstrate_strings
	.type	demonstrate_strings, @function
demonstrate_strings:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$192, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC96(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movabsq	$6278066737626506568, %rax
	movq	%rax, -142(%rbp)
	movabsq	$9399091170596640, %rax
	movq	%rax, -136(%rbp)
	movq	$1852337994, -128(%rbp)
	movq	$0, -120(%rbp)
	movl	$0, -112(%rbp)
	leaq	-142(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC97(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-128(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC98(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-142(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movq	%rax, %rsi
	leaq	.LC99(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-142(%rbp), %rdx
	leaq	-64(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	leaq	-64(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC100(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movq	%rax, %rdx
	leaq	-64(%rbp), %rax
	addq	%rdx, %rax
	movabsq	$7308620243103209248, %rcx
	movq	%rcx, (%rax)
	movw	$33, 8(%rax)
	leaq	-64(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC101(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$-1, %esi
	leaq	.LC102(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-142(%rbp), %rax
	movl	$87, %esi
	movq	%rax, %rdi
	call	strchr@PLT
	movq	%rax, -168(%rbp)
	cmpq	$0, -168(%rbp)
	je	.L59
	leaq	-142(%rbp), %rdx
	movq	-168(%rbp), %rax
	subq	%rdx, %rax
	movq	%rax, %rsi
	leaq	.LC103(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L59:
	movabsq	$2338328219631577172, %rax
	movabsq	$7308339910404153441, %rdx
	movq	%rax, -96(%rbp)
	movq	%rdx, -88(%rbp)
	movabsq	$8295742030033543539, %rax
	movabsq	$28538298447457893, %rdx
	movq	%rax, -86(%rbp)
	movq	%rdx, -78(%rbp)
	leaq	-96(%rbp), %rax
	leaq	.LC104(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strtok@PLT
	movq	%rax, -176(%rbp)
	leaq	.LC105(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L60
.L61:
	movq	-176(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC106(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC104(%rip), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	strtok@PLT
	movq	%rax, -176(%rbp)
.L60:
	cmpq	$0, -176(%rbp)
	jne	.L61
	movl	$875770417, -155(%rbp)
	movw	$53, -151(%rbp)
	leaq	-155(%rbp), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -184(%rbp)
	movl	-184(%rbp), %edx
	leaq	-155(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC107(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$775107121, -149(%rbp)
	movl	$3486766, -146(%rbp)
	leaq	-149(%rbp), %rax
	movq	%rax, %rdi
	call	atof@PLT
	cvtsd2ss	%xmm0, %xmm0
	movss	%xmm0, -180(%rbp)
	pxor	%xmm1, %xmm1
	cvtss2sd	-180(%rbp), %xmm1
	movq	%xmm1, %rdx
	leaq	-149(%rbp), %rax
	movq	%rdx, %xmm0
	movq	%rax, %rsi
	leaq	.LC108(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	leaq	.LC109(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	nop
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L62
	call	__stack_chk_fail@PLT
.L62:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	demonstrate_strings, .-demonstrate_strings
	.section	.rodata
.LC110:
	.string	"\n==== POINTERS ===="
.LC111:
	.string	"num: %d\n"
.LC112:
	.string	"&num (address): %p\n"
.LC113:
	.string	"ptr (holds address): %p\n"
	.align 8
.LC114:
	.string	"*ptr (dereferenced value): %d\n"
.LC115:
	.string	"After *ptr = 100, num: %d\n"
.LC116:
	.string	"Array pointer demonstration:"
.LC117:
	.string	"  *(array_ptr + %d) = %d\n"
.LC118:
	.string	"Pointer to pointer:"
.LC119:
	.string	"  value: %d\n"
.LC120:
	.string	"  *value_ptr: %d\n"
.LC121:
	.string	"  **ptr_to_ptr: %d\n"
.LC122:
	.string	"null_ptr: %p\n"
.LC123:
	.string	"void_ptr: %p\n"
.LC124:
	.string	"*(int*)void_ptr: %d\n"
.LC125:
	.string	"Before swap: a = %d, b = %d\n"
.LC126:
	.string	"After swap: a = %d, b = %d\n"
	.align 8
.LC127:
	.string	"Pointer to constant and constant pointer:"
.LC128:
	.string	"  *ptr_to_const: %d\n"
.LC129:
	.string	"  *const_ptr: %d\n"
	.text
	.globl	demonstrate_pointers
	.type	demonstrate_pointers, @function
demonstrate_pointers:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$-128, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC110(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$42, -124(%rbp)
	leaq	-124(%rbp), %rax
	movq	%rax, -88(%rbp)
	movl	-124(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC111(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-124(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC112(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-88(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC113(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC114(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-88(%rbp), %rax
	movl	$100, (%rax)
	movl	-124(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC115(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$10, -32(%rbp)
	movl	$20, -28(%rbp)
	movl	$30, -24(%rbp)
	movl	$40, -20(%rbp)
	movl	$50, -16(%rbp)
	leaq	-32(%rbp), %rax
	movq	%rax, -80(%rbp)
	leaq	.LC116(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -100(%rbp)
	jmp	.L64
.L65:
	movl	-100(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-80(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-100(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC117(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -100(%rbp)
.L64:
	cmpl	$4, -100(%rbp)
	jle	.L65
	movl	$5, -120(%rbp)
	leaq	-120(%rbp), %rax
	movq	%rax, -96(%rbp)
	leaq	-96(%rbp), %rax
	movq	%rax, -72(%rbp)
	leaq	.LC118(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	-120(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC119(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-96(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC120(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-72(%rbp), %rax
	movq	(%rax), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC121(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	$0, -64(%rbp)
	movq	-64(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC122(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-124(%rbp), %rax
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC123(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-56(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC124(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$5, -116(%rbp)
	movl	$10, -112(%rbp)
	movl	-112(%rbp), %edx
	movl	-116(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC125(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-112(%rbp), %rdx
	leaq	-116(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	swap
	movl	-112(%rbp), %edx
	movl	-116(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC126(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$200, -108(%rbp)
	leaq	-108(%rbp), %rax
	movq	%rax, -48(%rbp)
	movl	$100, -104(%rbp)
	leaq	-104(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movl	$200, (%rax)
	leaq	.LC127(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC128(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC129(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L66
	call	__stack_chk_fail@PLT
.L66:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	demonstrate_pointers, .-demonstrate_pointers
	.globl	swap
	.type	swap, @function
swap:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movq	-32(%rbp), %rax
	movl	(%rax), %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	movq	-32(%rbp), %rax
	movl	-4(%rbp), %edx
	movl	%edx, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	swap, .-swap
	.section	.rodata
	.align 8
.LC130:
	.string	"\n==== DYNAMIC MEMORY ALLOCATION ===="
.LC131:
	.string	"Memory allocation failed"
.LC132:
	.string	"malloc demonstration:"
.LC133:
	.string	"  *int_ptr: %d\n"
	.align 8
.LC134:
	.string	"calloc demonstration (initialized to 0):"
.LC135:
	.string	"  arr_ptr[%d]: %d\n"
.LC136:
	.string	"After assignment:"
.LC137:
	.string	"Memory reallocation failed"
	.align 8
.LC138:
	.string	"realloc demonstration (resized to 10 elements):"
.LC139:
	.string	"  new_arr_ptr[%d]: %d\n"
	.align 8
.LC140:
	.string	"After adding data to expanded section:"
	.align 8
.LC141:
	.string	"Dynamic 2D array demonstration:"
	.align 8
.LC142:
	.string	"All dynamically allocated memory has been freed"
	.text
	.globl	demonstrate_dynamic_memory
	.type	demonstrate_dynamic_memory, @function
demonstrate_dynamic_memory:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$104, %rsp
	.cfi_offset 3, -24
	leaq	.LC130(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$4, %edi
	call	malloc@PLT
	movq	%rax, -48(%rbp)
	cmpq	$0, -48(%rbp)
	jne	.L69
	leaq	.LC131(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L68
.L69:
	movq	-48(%rbp), %rax
	movl	$42, (%rax)
	leaq	.LC132(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC133(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$4, %esi
	movl	$5, %edi
	call	calloc@PLT
	movq	%rax, -40(%rbp)
	cmpq	$0, -40(%rbp)
	jne	.L71
	leaq	.LC131(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	jmp	.L68
.L71:
	leaq	.LC134(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -100(%rbp)
	jmp	.L72
.L73:
	movl	-100(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-100(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC135(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -100(%rbp)
.L72:
	cmpl	$4, -100(%rbp)
	jle	.L73
	movl	$0, -96(%rbp)
	jmp	.L74
.L75:
	movl	-96(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-40(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movl	-96(%rbp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	movl	%eax, (%rcx)
	addl	$1, -96(%rbp)
.L74:
	cmpl	$4, -96(%rbp)
	jle	.L75
	leaq	.LC136(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -92(%rbp)
	jmp	.L76
.L77:
	movl	-92(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-92(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC135(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -92(%rbp)
.L76:
	cmpl	$4, -92(%rbp)
	jle	.L77
	movq	-40(%rbp), %rax
	movl	$40, %esi
	movq	%rax, %rdi
	call	realloc@PLT
	movq	%rax, -32(%rbp)
	cmpq	$0, -32(%rbp)
	jne	.L78
	leaq	.LC137(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	jmp	.L68
.L78:
	leaq	.LC138(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -88(%rbp)
	jmp	.L79
.L80:
	movl	-88(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-88(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC139(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -88(%rbp)
.L79:
	cmpl	$4, -88(%rbp)
	jle	.L80
	movl	$5, -84(%rbp)
	jmp	.L81
.L82:
	movl	-84(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-32(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movl	-84(%rbp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	movl	%eax, (%rcx)
	addl	$1, -84(%rbp)
.L81:
	cmpl	$9, -84(%rbp)
	jle	.L82
	leaq	.LC140(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$5, -80(%rbp)
	jmp	.L83
.L84:
	movl	-80(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-80(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC139(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -80(%rbp)
.L83:
	cmpl	$9, -80(%rbp)
	jle	.L84
	movl	$3, -56(%rbp)
	movl	$4, -52(%rbp)
	movl	-56(%rbp), %eax
	cltq
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -24(%rbp)
	cmpq	$0, -24(%rbp)
	jne	.L85
	leaq	.LC131(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	jmp	.L68
.L85:
	movl	$0, -76(%rbp)
	jmp	.L86
.L90:
	movl	-52(%rbp), %eax
	cltq
	salq	$2, %rax
	movl	-76(%rbp), %edx
	movslq	%edx, %rdx
	leaq	0(,%rdx,8), %rcx
	movq	-24(%rbp), %rdx
	leaq	(%rcx,%rdx), %rbx
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, (%rbx)
	movl	-76(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	jne	.L87
	leaq	.LC131(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -72(%rbp)
	jmp	.L88
.L89:
	movl	-72(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	free@PLT
	addl	$1, -72(%rbp)
.L88:
	movl	-72(%rbp), %eax
	cmpl	-76(%rbp), %eax
	jl	.L89
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	jmp	.L68
.L87:
	addl	$1, -76(%rbp)
.L86:
	movl	-76(%rbp), %eax
	cmpl	-56(%rbp), %eax
	jl	.L90
	leaq	.LC141(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -68(%rbp)
	jmp	.L91
.L94:
	movl	$0, -64(%rbp)
	jmp	.L92
.L93:
	movl	-68(%rbp), %eax
	imull	-52(%rbp), %eax
	movl	%eax, %ecx
	movl	-68(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movl	-64(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	-64(%rbp), %edx
	addl	%ecx, %edx
	movl	%edx, (%rax)
	movl	-68(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movl	-64(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %ecx
	movl	-64(%rbp), %edx
	movl	-68(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC91(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -64(%rbp)
.L92:
	movl	-64(%rbp), %eax
	cmpl	-52(%rbp), %eax
	jl	.L93
	addl	$1, -68(%rbp)
.L91:
	movl	-68(%rbp), %eax
	cmpl	-56(%rbp), %eax
	jl	.L94
	movl	$0, -60(%rbp)
	jmp	.L95
.L96:
	movl	-60(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	free@PLT
	addl	$1, -60(%rbp)
.L95:
	movl	-60(%rbp), %eax
	cmpl	-56(%rbp), %eax
	jl	.L96
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	leaq	.LC142(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
.L68:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	demonstrate_dynamic_memory, .-demonstrate_dynamic_memory
	.section	.rodata
	.align 8
.LC143:
	.string	"\n==== STRUCTURES, UNIONS, AND ENUMS ===="
.LC145:
	.string	"Structure demo:"
	.align 8
.LC146:
	.string	"  Person: %s, %d years old, %.2f meters tall\n"
.LC150:
	.string	"Nested structure demo:"
	.align 8
.LC151:
	.string	"  Employee: %s (ID: %d), %d years old, %.2f meters tall, $%.2f salary\n"
.LC152:
	.string	"Union demo:"
.LC153:
	.string	"  data.i: %d\n"
.LC155:
	.string	"  data.f: %.1f\n"
.LC156:
	.string	"  data.str: %s\n"
.LC157:
	.string	"Enumeration demo:"
	.align 8
.LC158:
	.string	"  Today is day %d of the week\n"
.LC159:
	.string	"  It's Monday"
.LC160:
	.string	"  It's Tuesday"
.LC161:
	.string	"  It's Wednesday"
.LC162:
	.string	"  It's Thursday"
.LC163:
	.string	"  It's Friday"
.LC164:
	.string	"  It's the weekend"
.LC165:
	.string	"  Invalid day"
.LC166:
	.string	"Bit fields demo:"
.LC167:
	.string	"  is_active: %u\n"
.LC168:
	.string	"  permission: %u\n"
.LC169:
	.string	"  user_level: %u\n"
	.align 8
.LC170:
	.string	"  Size of Flags struct: %zu bytes\n"
	.text
	.globl	demonstrate_structures
	.type	demonstrate_structures, @function
demonstrate_structures:
.LFB17:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$256, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC143(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	leaq	-208(%rbp), %rax
	movabsq	$7309135626327584586, %rcx
	movq	%rcx, (%rax)
	movb	$0, 8(%rax)
	movl	$30, -156(%rbp)
	movss	.LC144(%rip), %xmm0
	movss	%xmm0, -152(%rbp)
	leaq	.LC145(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movss	-152(%rbp), %xmm0
	pxor	%xmm2, %xmm2
	cvtss2sd	%xmm0, %xmm2
	movq	%xmm2, %rcx
	movl	-156(%rbp), %edx
	leaq	-208(%rbp), %rax
	movq	%rcx, %xmm0
	movq	%rax, %rsi
	leaq	.LC146(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	movabsq	$7596819545049686346, %rax
	movl	$26740, %edx
	movq	%rax, -144(%rbp)
	movq	%rdx, -136(%rbp)
	movq	$0, -128(%rbp)
	movq	$0, -120(%rbp)
	movq	$0, -112(%rbp)
	movq	$0, -104(%rbp)
	movw	$0, -96(%rbp)
	movl	$25, -92(%rbp)
	movss	.LC147(%rip), %xmm0
	movss	%xmm0, -88(%rbp)
	movss	-88(%rbp), %xmm0
	pxor	%xmm3, %xmm3
	cvtss2sd	%xmm0, %xmm3
	movq	%xmm3, %rcx
	movl	-92(%rbp), %edx
	leaq	-144(%rbp), %rax
	movq	%rcx, %xmm0
	movq	%rax, %rsi
	leaq	.LC146(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	movabsq	$7955731105911566146, %rax
	movl	$7237491, %edx
	movq	%rax, -80(%rbp)
	movq	%rdx, -72(%rbp)
	movq	$0, -64(%rbp)
	movq	$0, -56(%rbp)
	movq	$0, -48(%rbp)
	movq	$0, -40(%rbp)
	movw	$0, -32(%rbp)
	movl	$35, -28(%rbp)
	movss	.LC148(%rip), %xmm0
	movss	%xmm0, -24(%rbp)
	movl	$12345, -20(%rbp)
	movss	.LC149(%rip), %xmm0
	movss	%xmm0, -16(%rbp)
	leaq	.LC150(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movss	-16(%rbp), %xmm0
	cvtss2sd	%xmm0, %xmm0
	movss	-24(%rbp), %xmm1
	pxor	%xmm4, %xmm4
	cvtss2sd	%xmm1, %xmm4
	movq	%xmm4, %rsi
	movl	-28(%rbp), %ecx
	movl	-20(%rbp), %edx
	leaq	-80(%rbp), %rax
	movapd	%xmm0, %xmm1
	movq	%rsi, %xmm0
	movq	%rax, %rsi
	leaq	.LC151(%rip), %rax
	movq	%rax, %rdi
	movl	$2, %eax
	call	printf@PLT
	movl	$10, -240(%rbp)
	leaq	.LC152(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	-240(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC153(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movss	.LC154(%rip), %xmm0
	movss	%xmm0, -240(%rbp)
	movss	-240(%rbp), %xmm0
	pxor	%xmm5, %xmm5
	cvtss2sd	%xmm0, %xmm5
	movq	%xmm5, %rax
	movq	%rax, %xmm0
	leaq	.LC155(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	leaq	-240(%rbp), %rax
	movabsq	$7021788497380909123, %rcx
	movq	%rcx, (%rax)
	movabsq	$29113321687703922, %rcx
	movq	%rcx, 6(%rax)
	leaq	-240(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC156(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$3, -244(%rbp)
	leaq	.LC157(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	-244(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC158(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	cmpl	$7, -244(%rbp)
	ja	.L98
	movl	-244(%rbp), %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L100(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L100(%rip), %rdx
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L100:
	.long	.L98-.L100
	.long	.L105-.L100
	.long	.L104-.L100
	.long	.L103-.L100
	.long	.L102-.L100
	.long	.L101-.L100
	.long	.L99-.L100
	.long	.L99-.L100
	.text
.L105:
	leaq	.LC159(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L106
.L104:
	leaq	.LC160(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L106
.L103:
	leaq	.LC161(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L106
.L102:
	leaq	.LC162(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L106
.L101:
	leaq	.LC163(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L106
.L99:
	leaq	.LC164(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L106
.L98:
	leaq	.LC165(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
.L106:
	movzbl	-248(%rbp), %eax
	orl	$1, %eax
	movb	%al, -248(%rbp)
	movzbl	-248(%rbp), %eax
	andl	$-15, %eax
	orl	$10, %eax
	movb	%al, -248(%rbp)
	movzbl	-248(%rbp), %eax
	andl	$15, %eax
	orl	$-96, %eax
	movb	%al, -248(%rbp)
	leaq	.LC166(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movzbl	-248(%rbp), %eax
	andl	$1, %eax
	movzbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC167(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movzbl	-248(%rbp), %eax
	shrb	%al
	andl	$7, %eax
	movzbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC168(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movzbl	-248(%rbp), %eax
	shrb	$4, %al
	movzbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC169(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$4, %esi
	leaq	.LC170(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L107
	call	__stack_chk_fail@PLT
.L107:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	demonstrate_structures, .-demonstrate_structures
	.section	.rodata
	.align 8
.LC171:
	.string	"\n==== FILE I/O OPERATIONS ===="
.LC172:
	.string	"w"
.LC173:
	.string	"demo.txt"
	.align 8
.LC174:
	.string	"Failed to open file for writing"
.LC175:
	.string	"Hello, File I/O!\n"
	.align 8
.LC176:
	.string	"This is a demonstration of file operations in C.\n"
.LC178:
	.string	"Numbers: %d, %f\n"
.LC179:
	.string	"File written successfully"
.LC180:
	.string	"r"
	.align 8
.LC181:
	.string	"Failed to open file for reading"
.LC182:
	.string	"File contents:"
.LC183:
	.string	"  %s"
.LC187:
	.string	"wb"
.LC188:
	.string	"records.bin"
	.align 8
.LC189:
	.string	"Failed to open binary file for writing"
	.align 8
.LC190:
	.string	"Binary data written successfully"
.LC191:
	.string	"rb"
	.align 8
.LC192:
	.string	"Failed to open binary file for reading"
.LC193:
	.string	"Binary data read:"
.LC194:
	.string	"  Record %d: %s, Score: %.1f\n"
	.align 8
.LC195:
	.string	"Failed to open binary file for random access"
	.align 8
.LC196:
	.string	"Random access - Second record:"
	.align 8
.LC197:
	.string	"  ID: %d, Name: %s, Score: %.1f\n"
.LC198:
	.string	"Temporary files removed"
	.text
	.globl	demonstrate_file_operations
	.type	demonstrate_file_operations, @function
demonstrate_file_operations:
.LFB18:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$576, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC171(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	leaq	.LC172(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC173(%rip), %rax
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -568(%rbp)
	cmpq	$0, -568(%rbp)
	jne	.L109
	leaq	.LC174(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L108
.L109:
	movq	-568(%rbp), %rax
	movq	%rax, %rcx
	movl	$17, %edx
	movl	$1, %esi
	leaq	.LC175(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movq	-568(%rbp), %rax
	movq	%rax, %rcx
	movl	$49, %edx
	movl	$1, %esi
	leaq	.LC176(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movq	.LC177(%rip), %rdx
	movq	-568(%rbp), %rax
	movq	%rdx, %xmm0
	movl	$42, %edx
	leaq	.LC178(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$1, %eax
	call	fprintf@PLT
	movq	-568(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	leaq	.LC179(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	leaq	.LC180(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC173(%rip), %rax
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -568(%rbp)
	cmpq	$0, -568(%rbp)
	jne	.L111
	leaq	.LC181(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L108
.L111:
	leaq	.LC182(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L112
.L113:
	leaq	-112(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC183(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L112:
	movq	-568(%rbp), %rdx
	leaq	-112(%rbp), %rax
	movl	$100, %esi
	movq	%rax, %rdi
	call	fgets@PLT
	testq	%rax, %rax
	jne	.L113
	movq	-568(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	movl	$1, -496(%rbp)
	movabsq	$435459550273, %rax
	movl	$0, %edx
	movq	%rax, -492(%rbp)
	movq	%rdx, -484(%rbp)
	movq	$0, -476(%rbp)
	movq	$0, -468(%rbp)
	movq	$0, -460(%rbp)
	movq	$0, -452(%rbp)
	movw	$0, -444(%rbp)
	movss	.LC184(%rip), %xmm0
	movss	%xmm0, -440(%rbp)
	movl	$2, -436(%rbp)
	movq	$6451010, -432(%rbp)
	movq	$0, -424(%rbp)
	movq	$0, -416(%rbp)
	movq	$0, -408(%rbp)
	movq	$0, -400(%rbp)
	movq	$0, -392(%rbp)
	movw	$0, -384(%rbp)
	movss	.LC185(%rip), %xmm0
	movss	%xmm0, -380(%rbp)
	movl	$3, -376(%rbp)
	movabsq	$28544887144147011, %rax
	movl	$0, %edx
	movq	%rax, -372(%rbp)
	movq	%rdx, -364(%rbp)
	movq	$0, -356(%rbp)
	movq	$0, -348(%rbp)
	movq	$0, -340(%rbp)
	movq	$0, -332(%rbp)
	movw	$0, -324(%rbp)
	movss	.LC186(%rip), %xmm0
	movss	%xmm0, -320(%rbp)
	leaq	.LC187(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC188(%rip), %rax
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -568(%rbp)
	cmpq	$0, -568(%rbp)
	jne	.L114
	leaq	.LC189(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L108
.L114:
	movq	-568(%rbp), %rdx
	leaq	-496(%rbp), %rax
	movq	%rdx, %rcx
	movl	$3, %edx
	movl	$60, %esi
	movq	%rax, %rdi
	call	fwrite@PLT
	movq	-568(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	leaq	.LC190(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	leaq	.LC191(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC188(%rip), %rax
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -568(%rbp)
	cmpq	$0, -568(%rbp)
	jne	.L115
	leaq	.LC192(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L108
.L115:
	movq	-568(%rbp), %rdx
	leaq	-304(%rbp), %rax
	movq	%rdx, %rcx
	movl	$3, %edx
	movl	$60, %esi
	movq	%rax, %rdi
	call	fread@PLT
	movq	-568(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	leaq	.LC193(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -572(%rbp)
	jmp	.L116
.L117:
	movl	-572(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	salq	$4, %rax
	subq	%rdx, %rax
	salq	$2, %rax
	addq	%rbp, %rax
	subq	$248, %rax
	movss	(%rax), %xmm0
	pxor	%xmm1, %xmm1
	cvtss2sd	%xmm0, %xmm1
	movq	%xmm1, %rcx
	leaq	-304(%rbp), %rsi
	movl	-572(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	salq	$4, %rax
	subq	%rdx, %rax
	salq	$2, %rax
	addq	%rsi, %rax
	leaq	4(%rax), %rsi
	movl	-572(%rbp), %eax
	movslq	%eax, %rdx
	movq	%rdx, %rax
	salq	$4, %rax
	subq	%rdx, %rax
	salq	$2, %rax
	addq	%rbp, %rax
	subq	$304, %rax
	movl	(%rax), %eax
	movq	%rcx, %xmm0
	movq	%rsi, %rdx
	movl	%eax, %esi
	leaq	.LC194(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	addl	$1, -572(%rbp)
.L116:
	cmpl	$2, -572(%rbp)
	jle	.L117
	leaq	.LC191(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC188(%rip), %rax
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -568(%rbp)
	cmpq	$0, -568(%rbp)
	jne	.L118
	leaq	.LC195(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L108
.L118:
	movq	-568(%rbp), %rax
	movl	$0, %edx
	movl	$60, %esi
	movq	%rax, %rdi
	call	fseek@PLT
	movq	-568(%rbp), %rdx
	leaq	-560(%rbp), %rax
	movq	%rdx, %rcx
	movl	$1, %edx
	movl	$60, %esi
	movq	%rax, %rdi
	call	fread@PLT
	leaq	.LC196(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movss	-504(%rbp), %xmm0
	pxor	%xmm2, %xmm2
	cvtss2sd	%xmm0, %xmm2
	movq	%xmm2, %rdx
	movl	-560(%rbp), %eax
	leaq	-560(%rbp), %rcx
	addq	$4, %rcx
	movq	%rdx, %xmm0
	movq	%rcx, %rdx
	movl	%eax, %esi
	leaq	.LC197(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	movq	-568(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	leaq	.LC173(%rip), %rax
	movq	%rax, %rdi
	call	remove@PLT
	leaq	.LC188(%rip), %rax
	movq	%rax, %rdi
	call	remove@PLT
	leaq	.LC198(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
.L108:
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L120
	call	__stack_chk_fail@PLT
.L120:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	demonstrate_file_operations, .-demonstrate_file_operations
	.section	.rodata
	.align 8
.LC199:
	.string	"  Callback called with value: %d\n"
	.text
	.globl	callback_function
	.type	callback_function, @function
callback_function:
.LFB19:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC199(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	callback_function, .-callback_function
	.section	.rodata
.LC200:
	.string	"\n==== FUNCTION POINTERS ===="
	.align 8
.LC201:
	.string	"Calling function through pointer:"
	.align 8
.LC202:
	.string	"Calling functions from array of function pointers:"
.LC203:
	.string	"Original array: "
.LC204:
	.string	"%d "
.LC205:
	.string	"Sorted array: "
	.text
	.globl	demonstrate_function_pointers
	.type	demonstrate_function_pointers, @function
demonstrate_function_pointers:
.LFB20:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC200(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	leaq	callback_function(%rip), %rax
	movq	%rax, -72(%rbp)
	leaq	.LC201(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-72(%rbp), %rax
	movl	$42, %edi
	call	*%rax
	movl	$0, -104(%rbp)
	jmp	.L123
.L124:
	movl	-104(%rbp), %eax
	cltq
	leaq	callback_function(%rip), %rdx
	movq	%rdx, -64(%rbp,%rax,8)
	addl	$1, -104(%rbp)
.L123:
	cmpl	$2, -104(%rbp)
	jle	.L124
	leaq	.LC202(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, -100(%rbp)
	jmp	.L125
.L126:
	movl	-100(%rbp), %eax
	cltq
	movq	-64(%rbp,%rax,8), %rcx
	movl	-100(%rbp), %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	movl	%eax, %edi
	call	*%rcx
	addl	$1, -100(%rbp)
.L125:
	cmpl	$2, -100(%rbp)
	jle	.L126
	movl	$5, -32(%rbp)
	movl	$2, -28(%rbp)
	movl	$8, -24(%rbp)
	movl	$1, -20(%rbp)
	movl	$9, -16(%rbp)
	movl	$3, -12(%rbp)
	movl	$6, -80(%rbp)
	leaq	.LC203(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, -96(%rbp)
	jmp	.L127
.L128:
	movl	-96(%rbp), %eax
	cltq
	movl	-32(%rbp,%rax,4), %eax
	movl	%eax, %esi
	leaq	.LC204(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -96(%rbp)
.L127:
	movl	-96(%rbp), %eax
	cmpl	-80(%rbp), %eax
	jl	.L128
	movl	$10, %edi
	call	putchar@PLT
	movl	$0, -92(%rbp)
	jmp	.L129
.L133:
	movl	$0, -88(%rbp)
	jmp	.L130
.L132:
	movl	-88(%rbp), %eax
	cltq
	movl	-32(%rbp,%rax,4), %edx
	movl	-88(%rbp), %eax
	addl	$1, %eax
	cltq
	movl	-32(%rbp,%rax,4), %eax
	cmpl	%eax, %edx
	jle	.L131
	movl	-88(%rbp), %eax
	cltq
	movl	-32(%rbp,%rax,4), %eax
	movl	%eax, -76(%rbp)
	movl	-88(%rbp), %eax
	addl	$1, %eax
	cltq
	movl	-32(%rbp,%rax,4), %edx
	movl	-88(%rbp), %eax
	cltq
	movl	%edx, -32(%rbp,%rax,4)
	movl	-88(%rbp), %eax
	addl	$1, %eax
	cltq
	movl	-76(%rbp), %edx
	movl	%edx, -32(%rbp,%rax,4)
.L131:
	addl	$1, -88(%rbp)
.L130:
	movl	-80(%rbp), %eax
	subl	-92(%rbp), %eax
	subl	$1, %eax
	cmpl	%eax, -88(%rbp)
	jl	.L132
	addl	$1, -92(%rbp)
.L129:
	movl	-80(%rbp), %eax
	subl	$1, %eax
	cmpl	%eax, -92(%rbp)
	jl	.L133
	leaq	.LC205(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, -84(%rbp)
	jmp	.L134
.L135:
	movl	-84(%rbp), %eax
	cltq
	movl	-32(%rbp,%rax,4), %eax
	movl	%eax, %esi
	leaq	.LC204(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -84(%rbp)
.L134:
	movl	-84(%rbp), %eax
	cmpl	-80(%rbp), %eax
	jl	.L135
	movl	$10, %edi
	call	putchar@PLT
	nop
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L136
	call	__stack_chk_fail@PLT
.L136:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	demonstrate_function_pointers, .-demonstrate_function_pointers
	.section	.rodata
	.align 8
.LC206:
	.string	"\n==== PREPROCESSOR DIRECTIVES ===="
.LC208:
	.string	"Macro PI: %.5f\n"
.LC209:
	.string	"SQUARE(5): %d\n"
.LC210:
	.string	"MAX(10, 20): %d\n"
.LC211:
	.string	"x = %d\n"
.LC212:
	.string	"Debug mode is OFF"
.LC213:
	.string	"Using C11 or later"
.LC214:
	.string	"demo.c"
.LC215:
	.string	"Current file: %s\n"
.LC216:
	.string	"Current line: %d\n"
.LC217:
	.string	"Mar  9 2025"
.LC218:
	.string	"Compilation date: %s\n"
.LC219:
	.string	"18:52:19"
.LC220:
	.string	"Compilation time: %s\n"
.LC221:
	.string	"Hello World"
.LC222:
	.string	"Stringizing operator: %s\n"
.LC223:
	.string	"Token-pasting: %d\n"
.LC224:
	.string	"TEMP_VALUE: %d\n"
	.text
	.globl	demonstrate_preprocessor
	.type	demonstrate_preprocessor, @function
demonstrate_preprocessor:
.LFB21:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	leaq	.LC206(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	.LC207(%rip), %rax
	movq	%rax, %xmm0
	leaq	.LC208(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	movl	$25, %esi
	leaq	.LC209(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$20, %esi
	leaq	.LC210(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$42, -8(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC211(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC212(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	leaq	.LC213(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	leaq	.LC214(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC215(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$941, %esi
	leaq	.LC216(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC217(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC218(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC219(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC220(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC221(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC222(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$100, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC223(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$100, %esi
	leaq	.LC224(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	demonstrate_preprocessor, .-demonstrate_preprocessor
	.section	.rodata
	.align 8
.LC225:
	.string	"Variadic function demonstration:"
.LC226:
	.string	"  Format string: %s\n"
.LC227:
	.string	"  Integer argument: %d\n"
.LC228:
	.string	"  Float argument: %.2f\n"
.LC229:
	.string	"  String argument: %s\n"
	.text
	.globl	demonstrate_variadic_functions
	.type	demonstrate_variadic_functions, @function
demonstrate_variadic_functions:
.LFB22:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$256, %rsp
	movq	%rdi, -248(%rbp)
	movq	%rsi, -168(%rbp)
	movq	%rdx, -160(%rbp)
	movq	%rcx, -152(%rbp)
	movq	%r8, -144(%rbp)
	movq	%r9, -136(%rbp)
	testb	%al, %al
	je	.L139
	movaps	%xmm0, -128(%rbp)
	movaps	%xmm1, -112(%rbp)
	movaps	%xmm2, -96(%rbp)
	movaps	%xmm3, -80(%rbp)
	movaps	%xmm4, -64(%rbp)
	movaps	%xmm5, -48(%rbp)
	movaps	%xmm6, -32(%rbp)
	movaps	%xmm7, -16(%rbp)
.L139:
	movq	%fs:40, %rax
	movq	%rax, -184(%rbp)
	xorl	%eax, %eax
	movl	$8, -208(%rbp)
	movl	$48, -204(%rbp)
	leaq	16(%rbp), %rax
	movq	%rax, -200(%rbp)
	leaq	-176(%rbp), %rax
	movq	%rax, -192(%rbp)
	leaq	.LC225(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-248(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC226(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, -232(%rbp)
	jmp	.L140
.L152:
	movl	-232(%rbp), %eax
	movslq	%eax, %rdx
	movq	-248(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$37, %al
	jne	.L141
	movl	-232(%rbp), %eax
	cltq
	leaq	1(%rax), %rdx
	movq	-248(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	cmpl	$115, %eax
	je	.L142
	cmpl	$115, %eax
	jg	.L143
	cmpl	$100, %eax
	je	.L144
	cmpl	$102, %eax
	je	.L145
	jmp	.L143
.L144:
	movl	-208(%rbp), %eax
	cmpl	$47, %eax
	ja	.L146
	movq	-192(%rbp), %rax
	movl	-208(%rbp), %edx
	movl	%edx, %edx
	addq	%rdx, %rax
	movl	-208(%rbp), %edx
	addl	$8, %edx
	movl	%edx, -208(%rbp)
	jmp	.L147
.L146:
	movq	-200(%rbp), %rax
	leaq	8(%rax), %rdx
	movq	%rdx, -200(%rbp)
.L147:
	movl	(%rax), %eax
	movl	%eax, -228(%rbp)
	movl	-228(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC227(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L143
.L145:
	movl	-204(%rbp), %eax
	cmpl	$175, %eax
	ja	.L148
	movq	-192(%rbp), %rax
	movl	-204(%rbp), %edx
	movl	%edx, %edx
	addq	%rdx, %rax
	movl	-204(%rbp), %edx
	addl	$16, %edx
	movl	%edx, -204(%rbp)
	jmp	.L149
.L148:
	movq	-200(%rbp), %rax
	leaq	8(%rax), %rdx
	movq	%rdx, -200(%rbp)
.L149:
	movsd	(%rax), %xmm0
	movsd	%xmm0, -216(%rbp)
	movq	-216(%rbp), %rax
	movq	%rax, %xmm0
	leaq	.LC228(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	jmp	.L143
.L142:
	movl	-208(%rbp), %eax
	cmpl	$47, %eax
	ja	.L150
	movq	-192(%rbp), %rax
	movl	-208(%rbp), %edx
	movl	%edx, %edx
	addq	%rdx, %rax
	movl	-208(%rbp), %edx
	addl	$8, %edx
	movl	%edx, -208(%rbp)
	jmp	.L151
.L150:
	movq	-200(%rbp), %rax
	leaq	8(%rax), %rdx
	movq	%rdx, -200(%rbp)
.L151:
	movq	(%rax), %rax
	movq	%rax, -224(%rbp)
	movq	-224(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC229(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
.L143:
	addl	$1, -232(%rbp)
.L141:
	addl	$1, -232(%rbp)
.L140:
	movl	-232(%rbp), %eax
	movslq	%eax, %rdx
	movq	-248(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L152
	nop
	movq	-184(%rbp), %rax
	subq	%fs:40, %rax
	je	.L153
	call	__stack_chk_fail@PLT
.L153:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	demonstrate_variadic_functions, .-demonstrate_variadic_functions
	.section	.rodata
	.align 4
.LC6:
	.long	1078523331
	.align 8
.LC7:
	.long	1413754602
	.long	1074340347
	.align 4
.LC23:
	.long	1082088489
	.align 4
.LC144:
	.long	1071644672
	.align 4
.LC147:
	.long	1070805811
	.align 4
.LC148:
	.long	1072064102
	.align 4
.LC149:
	.long	1195593728
	.align 4
.LC154:
	.long	1130135552
	.align 8
.LC177:
	.long	1374389535
	.long	1074339512
	.align 4
.LC184:
	.long	1119813632
	.align 4
.LC185:
	.long	1118961664
	.align 4
.LC186:
	.long	1117585408
	.align 8
.LC207:
	.long	-266631570
	.long	1074340345
	.ident	"GCC: (GNU) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
