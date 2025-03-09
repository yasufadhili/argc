	
	.text
	.globl	lexer_init
	.type	lexer_init, @function
lexer_init:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, (%rax)
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movq	-8(%rbp), %rdx
	movq	%rax, 8(%rdx)
	movq	-8(%rbp), %rax
	movq	$0, 16(%rax)
	movq	-8(%rbp), %rax
	movl	$1, 24(%rax)
	movq	-8(%rbp), %rax
	movl	$1, 28(%rax)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L2
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movzbl	(%rax), %eax
	jmp	.L3
.L2:
	movl	$0, %eax
.L3:
	movq	-8(%rbp), %rdx
	movb	%al, 32(%rdx)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	lexer_init, .-lexer_init
	.globl	lexer_advance
	.type	lexer_advance, @function
lexer_advance:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	cmpq	%rax, %rdx
	jnb	.L10
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	leaq	1(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 16(%rax)
	movq	-8(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$10, %al
	jne	.L6
	movq	-8(%rbp), %rax
	movl	24(%rax), %eax
	leal	1(%rax), %edx
	movq	-8(%rbp), %rax
	movl	%edx, 24(%rax)
	movq	-8(%rbp), %rax
	movl	$1, 28(%rax)
	jmp	.L7
.L6:
	movq	-8(%rbp), %rax
	movl	28(%rax), %eax
	leal	1(%rax), %edx
	movq	-8(%rbp), %rax
	movl	%edx, 28(%rax)
.L7:
	movq	-8(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	cmpq	%rax, %rdx
	jnb	.L8
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	jmp	.L9
.L8:
	movl	$0, %eax
.L9:
	movq	-8(%rbp), %rdx
	movb	%al, 32(%rdx)
.L10:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	lexer_advance, .-lexer_advance
	.globl	lexer_skip_whitespace
	.type	lexer_skip_whitespace, @function
lexer_skip_whitespace:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	jmp	.L12
.L14:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
.L12:
	movq	-8(%rbp), %rax
	movzbl	32(%rax), %eax
	testb	%al, %al
	je	.L15
	call	__ctype_b_loc@PLT
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movzbl	32(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	jne	.L14
.L15:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	lexer_skip_whitespace, .-lexer_skip_whitespace
	.globl	lexer_skip_comments
	.type	lexer_skip_comments, @function
lexer_skip_comments:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$47, %al
	jne	.L24
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	leaq	1(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	cmpq	%rax, %rdx
	jnb	.L24
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	addq	$1, %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$47, %al
	jne	.L18
	jmp	.L19
.L20:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
.L19:
	movq	-8(%rbp), %rax
	movzbl	32(%rax), %eax
	testb	%al, %al
	je	.L24
	movq	-8(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$10, %al
	jne	.L20
	jmp	.L24
.L18:
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	addq	$1, %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$42, %al
	jne	.L24
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
	jmp	.L21
.L23:
	movq	-8(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$42, %al
	jne	.L22
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	leaq	1(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	cmpq	%rax, %rdx
	jnb	.L22
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	addq	$1, %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$47, %al
	jne	.L22
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
	nop
	jmp	.L24
.L22:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
.L21:
	movq	-8(%rbp), %rax
	movzbl	32(%rax), %eax
	testb	%al, %al
	jne	.L23
.L24:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	lexer_skip_comments, .-lexer_skip_comments
	.globl	is_keyword
	.type	is_keyword, @function
is_keyword:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L26
.L29:
	movl	-4(%rbp), %eax
	cltq
	salq	$4, %rax
	movq	%rax, %rdx
	leaq	keywords.0(%rip), %rax
	movq	(%rdx,%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L27
	movl	-4(%rbp), %eax
	cltq
	salq	$4, %rax
	movq	%rax, %rdx
	leaq	8+keywords.0(%rip), %rax
	movl	(%rdx,%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	movl	$1, %eax
	jmp	.L28
.L27:
	addl	$1, -4(%rbp)
.L26:
	movl	-4(%rbp), %eax
	cltq
	salq	$4, %rax
	movq	%rax, %rdx
	leaq	keywords.0(%rip), %rax
	movq	(%rdx,%rax), %rax
	testq	%rax, %rax
	jne	.L29
	movl	$0, %eax
.L28:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	is_keyword, .-is_keyword
	.globl	lexer_identifier
	.type	lexer_identifier, @function
lexer_identifier:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$344, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -344(%rbp)
	movq	%rsi, -352(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movq	$0, -288(%rbp)
	movq	$0, -280(%rbp)
	movq	$0, -272(%rbp)
	movq	$0, -264(%rbp)
	movq	$0, -256(%rbp)
	movq	$0, -248(%rbp)
	movq	$0, -240(%rbp)
	movq	$0, -232(%rbp)
	movq	$0, -224(%rbp)
	movq	$0, -216(%rbp)
	movq	$0, -208(%rbp)
	movq	$0, -200(%rbp)
	movq	$0, -192(%rbp)
	movq	$0, -184(%rbp)
	movq	$0, -176(%rbp)
	movq	$0, -168(%rbp)
	movq	$0, -160(%rbp)
	movq	$0, -152(%rbp)
	movq	$0, -144(%rbp)
	movq	$0, -136(%rbp)
	movq	$0, -128(%rbp)
	movq	$0, -120(%rbp)
	movq	$0, -112(%rbp)
	movq	$0, -104(%rbp)
	movq	$0, -96(%rbp)
	movq	$0, -88(%rbp)
	movq	$0, -80(%rbp)
	movq	$0, -72(%rbp)
	movq	$0, -64(%rbp)
	movq	$0, -56(%rbp)
	movq	$0, -48(%rbp)
	movq	$0, -40(%rbp)
	movl	$0, -324(%rbp)
	jmp	.L31
.L33:
	movl	-324(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -324(%rbp)
	movq	-352(%rbp), %rdx
	movzbl	32(%rdx), %edx
	cltq
	movb	%dl, -288(%rbp,%rax)
	movq	-352(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
.L31:
	movq	-352(%rbp), %rax
	movzbl	32(%rax), %eax
	testb	%al, %al
	je	.L32
	call	__ctype_b_loc@PLT
	movq	(%rax), %rdx
	movq	-352(%rbp), %rax
	movzbl	32(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8, %eax
	testl	%eax, %eax
	jne	.L33
	movq	-352(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$95, %al
	je	.L33
.L32:
	movl	-324(%rbp), %eax
	cltq
	movb	$0, -288(%rbp,%rax)
	leaq	-328(%rbp), %rdx
	leaq	-288(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	is_keyword
	testb	%al, %al
	je	.L34
	movl	$3, -320(%rbp)
	movl	-328(%rbp), %eax
	movl	%eax, -312(%rbp)
	jmp	.L35
.L34:
	movl	$1, -320(%rbp)
	leaq	-288(%rbp), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, -312(%rbp)
.L35:
	movq	-352(%rbp), %rax
	movl	24(%rax), %eax
	movl	%eax, -304(%rbp)
	movq	-352(%rbp), %rax
	movl	28(%rax), %eax
	movl	%eax, %ebx
	leaq	-288(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movl	%eax, %edx
	movl	%ebx, %eax
	subl	%edx, %eax
	movl	%eax, -300(%rbp)
	movq	-344(%rbp), %rcx
	movq	-320(%rbp), %rax
	movq	-312(%rbp), %rdx
	movq	%rax, (%rcx)
	movq	%rdx, 8(%rcx)
	movq	-304(%rbp), %rax
	movq	%rax, 16(%rcx)
	movq	-24(%rbp), %rax
	subq	%fs:40, %rax
	je	.L37
	call	__stack_chk_fail@PLT
.L37:
	movq	-344(%rbp), %rax
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	lexer_identifier, .-lexer_identifier
	.globl	lexer_number
	.type	lexer_number, @function
lexer_number:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$344, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -344(%rbp)
	movq	%rsi, -352(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movq	$0, -288(%rbp)
	movq	$0, -280(%rbp)
	movq	$0, -272(%rbp)
	movq	$0, -264(%rbp)
	movq	$0, -256(%rbp)
	movq	$0, -248(%rbp)
	movq	$0, -240(%rbp)
	movq	$0, -232(%rbp)
	movq	$0, -224(%rbp)
	movq	$0, -216(%rbp)
	movq	$0, -208(%rbp)
	movq	$0, -200(%rbp)
	movq	$0, -192(%rbp)
	movq	$0, -184(%rbp)
	movq	$0, -176(%rbp)
	movq	$0, -168(%rbp)
	movq	$0, -160(%rbp)
	movq	$0, -152(%rbp)
	movq	$0, -144(%rbp)
	movq	$0, -136(%rbp)
	movq	$0, -128(%rbp)
	movq	$0, -120(%rbp)
	movq	$0, -112(%rbp)
	movq	$0, -104(%rbp)
	movq	$0, -96(%rbp)
	movq	$0, -88(%rbp)
	movq	$0, -80(%rbp)
	movq	$0, -72(%rbp)
	movq	$0, -64(%rbp)
	movq	$0, -56(%rbp)
	movq	$0, -48(%rbp)
	movq	$0, -40(%rbp)
	movl	$0, -324(%rbp)
	jmp	.L39
.L41:
	movl	-324(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -324(%rbp)
	movq	-352(%rbp), %rdx
	movzbl	32(%rdx), %edx
	cltq
	movb	%dl, -288(%rbp,%rax)
	movq	-352(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
.L39:
	movq	-352(%rbp), %rax
	movzbl	32(%rax), %eax
	testb	%al, %al
	je	.L40
	call	__ctype_b_loc@PLT
	movq	(%rax), %rdx
	movq	-352(%rbp), %rax
	movzbl	32(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$2048, %eax
	testl	%eax, %eax
	jne	.L41
.L40:
	movl	-324(%rbp), %eax
	cltq
	movb	$0, -288(%rbp,%rax)
	movl	$2, -320(%rbp)
	leaq	-288(%rbp), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, -312(%rbp)
	movq	-352(%rbp), %rax
	movl	24(%rax), %eax
	movl	%eax, -304(%rbp)
	movq	-352(%rbp), %rax
	movl	28(%rax), %eax
	movl	%eax, %ebx
	leaq	-288(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movl	%eax, %edx
	movl	%ebx, %eax
	subl	%edx, %eax
	movl	%eax, -300(%rbp)
	movq	-344(%rbp), %rcx
	movq	-320(%rbp), %rax
	movq	-312(%rbp), %rdx
	movq	%rax, (%rcx)
	movq	%rdx, 8(%rcx)
	movq	-304(%rbp), %rax
	movq	%rax, 16(%rcx)
	movq	-24(%rbp), %rax
	subq	%fs:40, %rax
	je	.L43
	call	__stack_chk_fail@PLT
.L43:
	movq	-344(%rbp), %rax
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	lexer_number, .-lexer_number
	.globl	lexer_string
	.type	lexer_string, @function
lexer_string:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$1112, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -1112(%rbp)
	movq	%rsi, -1120(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	leaq	-1056(%rbp), %rdx
	movl	$0, %eax
	movl	$128, %ecx
	movq	%rdx, %rdi
	rep stosq
	movl	$0, -1092(%rbp)
	movq	-1120(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
	jmp	.L45
.L55:
	movq	-1120(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$92, %al
	jne	.L46
	movq	-1120(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
	movq	-1120(%rbp), %rax
	movzbl	32(%rax), %eax
	movsbl	%al, %eax
	cmpl	$116, %eax
	je	.L47
	cmpl	$116, %eax
	jg	.L48
	cmpl	$110, %eax
	je	.L49
	cmpl	$110, %eax
	jg	.L48
	cmpl	$34, %eax
	je	.L50
	cmpl	$92, %eax
	je	.L51
	jmp	.L48
.L49:
	movl	-1092(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -1092(%rbp)
	cltq
	movb	$10, -1056(%rbp,%rax)
	jmp	.L53
.L47:
	movl	-1092(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -1092(%rbp)
	cltq
	movb	$9, -1056(%rbp,%rax)
	jmp	.L53
.L51:
	movl	-1092(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -1092(%rbp)
	cltq
	movb	$92, -1056(%rbp,%rax)
	jmp	.L53
.L50:
	movl	-1092(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -1092(%rbp)
	cltq
	movb	$34, -1056(%rbp,%rax)
	jmp	.L53
.L48:
	movl	-1092(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -1092(%rbp)
	movq	-1120(%rbp), %rdx
	movzbl	32(%rdx), %edx
	cltq
	movb	%dl, -1056(%rbp,%rax)
	jmp	.L53
.L46:
	movl	-1092(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -1092(%rbp)
	movq	-1120(%rbp), %rdx
	movzbl	32(%rdx), %edx
	cltq
	movb	%dl, -1056(%rbp,%rax)
.L53:
	movq	-1120(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
.L45:
	movq	-1120(%rbp), %rax
	movzbl	32(%rax), %eax
	testb	%al, %al
	je	.L54
	movq	-1120(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$34, %al
	jne	.L55
.L54:
	movq	-1120(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$34, %al
	jne	.L56
	movq	-1120(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
.L56:
	movl	-1092(%rbp), %eax
	cltq
	movb	$0, -1056(%rbp,%rax)
	movl	$6, -1088(%rbp)
	leaq	-1056(%rbp), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, -1080(%rbp)
	movq	-1120(%rbp), %rax
	movl	24(%rax), %eax
	movl	%eax, -1072(%rbp)
	movq	-1120(%rbp), %rax
	movl	28(%rax), %eax
	movl	%eax, %ebx
	leaq	-1056(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movl	%eax, %edx
	movl	%ebx, %eax
	subl	%edx, %eax
	subl	$2, %eax
	movl	%eax, -1068(%rbp)
	movq	-1112(%rbp), %rcx
	movq	-1088(%rbp), %rax
	movq	-1080(%rbp), %rdx
	movq	%rax, (%rcx)
	movq	%rdx, 8(%rcx)
	movq	-1072(%rbp), %rax
	movq	%rax, 16(%rcx)
	movq	-24(%rbp), %rax
	subq	%fs:40, %rax
	je	.L58
	call	__stack_chk_fail@PLT
.L58:
	movq	-1112(%rbp), %rax
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	lexer_string, .-lexer_string
	.section	.rodata
.LC0:
	.string	"+-*/=<>!&|^%"
.LC1:
	.string	"(){}[];,."
	.align 8
.LC2:
	.string	"Unrecognized character '%c' at line %d, column %d\n"
	.text
	.globl	lexer_get_next_token
	.type	lexer_get_next_token, @function
lexer_get_next_token:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	jmp	.L60
.L71:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rdx
	movq	-48(%rbp), %rax
	movzbl	32(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L61
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_skip_whitespace
	jmp	.L60
.L61:
	movq	-48(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$47, %al
	jne	.L62
	movq	-48(%rbp), %rax
	movq	16(%rax), %rax
	leaq	1(%rax), %rdx
	movq	-48(%rbp), %rax
	movq	8(%rax), %rax
	cmpq	%rax, %rdx
	jnb	.L62
	movq	-48(%rbp), %rax
	movq	(%rax), %rdx
	movq	-48(%rbp), %rax
	movq	16(%rax), %rax
	addq	$1, %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$47, %al
	je	.L63
	movq	-48(%rbp), %rax
	movq	(%rax), %rdx
	movq	-48(%rbp), %rax
	movq	16(%rax), %rax
	addq	$1, %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$42, %al
	jne	.L62
.L63:
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_skip_comments
	jmp	.L60
.L62:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rdx
	movq	-48(%rbp), %rax
	movzbl	32(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$1024, %eax
	testl	%eax, %eax
	jne	.L64
	movq	-48(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$95, %al
	jne	.L65
.L64:
	movq	-40(%rbp), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	lexer_identifier
	jmp	.L59
.L65:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rdx
	movq	-48(%rbp), %rax
	movzbl	32(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$2048, %eax
	testl	%eax, %eax
	je	.L67
	movq	-40(%rbp), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	lexer_number
	jmp	.L59
.L67:
	movq	-48(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$34, %al
	jne	.L68
	movq	-40(%rbp), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	lexer_string
	jmp	.L59
.L68:
	movq	-48(%rbp), %rax
	movzbl	32(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	strchr@PLT
	testq	%rax, %rax
	je	.L69
	movl	$4, -32(%rbp)
	movq	-48(%rbp), %rax
	movzbl	32(%rax), %eax
	movb	%al, -24(%rbp)
	movq	-48(%rbp), %rax
	movl	24(%rax), %eax
	movl	%eax, -16(%rbp)
	movq	-48(%rbp), %rax
	movl	28(%rax), %eax
	movl	%eax, -12(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
	movq	-40(%rbp), %rcx
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rax, (%rcx)
	movq	%rdx, 8(%rcx)
	movq	-16(%rbp), %rax
	movq	%rax, 16(%rcx)
	jmp	.L59
.L69:
	movq	-48(%rbp), %rax
	movzbl	32(%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	call	strchr@PLT
	testq	%rax, %rax
	je	.L70
	movl	$5, -32(%rbp)
	movq	-48(%rbp), %rax
	movzbl	32(%rax), %eax
	movb	%al, -24(%rbp)
	movq	-48(%rbp), %rax
	movl	24(%rax), %eax
	movl	%eax, -16(%rbp)
	movq	-48(%rbp), %rax
	movl	28(%rax), %eax
	movl	%eax, -12(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
	movq	-40(%rbp), %rcx
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rax, (%rcx)
	movq	%rdx, 8(%rcx)
	movq	-16(%rbp), %rax
	movq	%rax, 16(%rcx)
	jmp	.L59
.L70:
	movq	-48(%rbp), %rax
	movl	28(%rax), %esi
	movq	-48(%rbp), %rax
	movl	24(%rax), %ecx
	movq	-48(%rbp), %rax
	movzbl	32(%rax), %eax
	movsbl	%al, %edx
	movq	stderr(%rip), %rax
	movl	%esi, %r8d
	leaq	.LC2(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	lexer_advance
.L60:
	movq	-48(%rbp), %rax
	movzbl	32(%rax), %eax
	testb	%al, %al
	jne	.L71
	movl	$0, -32(%rbp)
	movq	-48(%rbp), %rax
	movl	24(%rax), %eax
	movl	%eax, -16(%rbp)
	movq	-48(%rbp), %rax
	movl	28(%rax), %eax
	movl	%eax, -12(%rbp)
	movq	-40(%rbp), %rcx
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rax, (%rcx)
	movq	%rdx, 8(%rcx)
	movq	-16(%rbp), %rax
	movq	%rax, 16(%rcx)
.L59:
	movq	-8(%rbp), %rax
	subq	%fs:40, %rax
	je	.L73
	call	__stack_chk_fail@PLT
.L73:
	movq	-40(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	lexer_get_next_token, .-lexer_get_next_token
	.globl	parser_init
	.type	parser_init, @function
parser_init:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$72, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movq	-40(%rbp), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	lexer_init
	movq	-40(%rbp), %rdx
	movq	-40(%rbp), %rbx
	leaq	-80(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	lexer_get_next_token
	movq	-80(%rbp), %rax
	movq	-72(%rbp), %rdx
	movq	%rax, 40(%rbx)
	movq	%rdx, 48(%rbx)
	movq	-64(%rbp), %rax
	movq	%rax, 56(%rbx)
	nop
	movq	-24(%rbp), %rax
	subq	%fs:40, %rax
	je	.L75
	call	__stack_chk_fail@PLT
.L75:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	parser_init, .-parser_init
	.section	.rodata
	.align 8
.LC3:
	.string	"Unexpected token type at line %d, column %d\n"
	.text
	.globl	parser_eat
	.type	parser_eat, @function
parser_eat:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$72, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -40(%rbp)
	movl	%esi, -44(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movq	-40(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	%eax, -44(%rbp)
	jne	.L77
	movq	-40(%rbp), %rdx
	movq	-40(%rbp), %rbx
	leaq	-80(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	lexer_get_next_token
	movq	-80(%rbp), %rax
	movq	-72(%rbp), %rdx
	movq	%rax, 40(%rbx)
	movq	%rdx, 48(%rbx)
	movq	-64(%rbp), %rax
	movq	%rax, 56(%rbx)
	nop
	movq	-24(%rbp), %rax
	subq	%fs:40, %rax
	je	.L79
	jmp	.L80
.L77:
	movq	-40(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-40(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC3(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L80:
	call	__stack_chk_fail@PLT
.L79:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	parser_eat, .-parser_eat
	.section	.rodata
	.align 8
.LC4:
	.string	"Unexpected token in primary expression at line %d, column %d\n"
	.text
	.globl	parser_parse_primary
	.type	parser_parse_primary, @function
parser_parse_primary:
.LFB17:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$56, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -56(%rbp)
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -40(%rbp)
	movq	-56(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$6, %eax
	je	.L82
	cmpl	$6, %eax
	ja	.L83
	cmpl	$5, %eax
	je	.L84
	cmpl	$5, %eax
	ja	.L83
	cmpl	$1, %eax
	je	.L85
	cmpl	$2, %eax
	je	.L86
	jmp	.L83
.L85:
	movq	-56(%rbp), %rax
	movq	48(%rax), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, -24(%rbp)
	movq	-56(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-56(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L87
	movq	-56(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$40, %al
	jne	.L87
	movq	-40(%rbp), %rax
	movl	$11, (%rax)
	movq	-40(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-40(%rbp), %rax
	movq	$0, 16(%rax)
	movq	-40(%rbp), %rax
	movl	$0, 24(%rax)
	movq	-56(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-56(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L88
	movq	-56(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$41, %al
	je	.L89
.L88:
	movl	$80, %edi
	call	malloc@PLT
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 16(%rax)
.L90:
	movq	-40(%rbp), %rax
	movq	16(%rax), %rsi
	movq	-40(%rbp), %rax
	movl	24(%rax), %eax
	leal	1(%rax), %ecx
	movq	-40(%rbp), %rdx
	movl	%ecx, 24(%rdx)
	cltq
	salq	$3, %rax
	leaq	(%rsi,%rax), %rbx
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_expression
	movq	%rax, (%rbx)
	movq	-56(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L89
	movq	-56(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$44, %al
	jne	.L89
	movq	-56(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	jmp	.L90
.L89:
	movq	-56(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	jmp	.L92
.L87:
	movq	-40(%rbp), %rax
	movl	$12, (%rax)
	movq	-40(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, 8(%rax)
	jmp	.L92
.L86:
	movq	-40(%rbp), %rax
	movl	$13, (%rax)
	movq	-56(%rbp), %rax
	movl	48(%rax), %edx
	movq	-40(%rbp), %rax
	movl	%edx, 8(%rax)
	movq	-56(%rbp), %rax
	movl	$2, %esi
	movq	%rax, %rdi
	call	parser_eat
	jmp	.L92
.L82:
	movq	-40(%rbp), %rax
	movl	$14, (%rax)
	movq	-56(%rbp), %rax
	movq	48(%rax), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 8(%rax)
	movq	-56(%rbp), %rax
	movl	$6, %esi
	movq	%rax, %rdi
	call	parser_eat
	jmp	.L92
.L84:
	movq	-56(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$40, %al
	jne	.L83
	movq	-56(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_expression
	movq	%rax, -32(%rbp)
	movq	-56(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-32(%rbp), %rax
	jmp	.L93
.L83:
	movq	-56(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-56(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC4(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L92:
	movq	-40(%rbp), %rax
.L93:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	parser_parse_primary, .-parser_parse_primary
	.globl	parser_parse_unary
	.type	parser_parse_unary, @function
parser_parse_unary:
.LFB18:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$4, %eax
	jne	.L95
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	movb	%al, -9(%rbp)
	cmpb	$45, -9(%rbp)
	je	.L96
	cmpb	$33, -9(%rbp)
	je	.L96
	cmpb	$126, -9(%rbp)
	je	.L96
	cmpb	$38, -9(%rbp)
	je	.L96
	cmpb	$42, -9(%rbp)
	jne	.L95
.L96:
	movq	-24(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$10, (%rax)
	movq	-8(%rbp), %rax
	movzbl	-9(%rbp), %edx
	movb	%dl, 8(%rax)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_unary
	movq	-8(%rbp), %rdx
	movq	%rax, 16(%rdx)
	movq	-8(%rbp), %rax
	jmp	.L97
.L95:
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_primary
.L97:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	parser_parse_unary, .-parser_parse_unary
	.globl	parser_parse_multiplicative
	.type	parser_parse_multiplicative, @function
parser_parse_multiplicative:
.LFB19:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_unary
	movq	%rax, -16(%rbp)
	jmp	.L99
.L102:
	movq	-40(%rbp), %rax
	movzbl	48(%rax), %eax
	movb	%al, -17(%rbp)
	cmpb	$42, -17(%rbp)
	je	.L100
	cmpb	$47, -17(%rbp)
	je	.L100
	cmpb	$37, -17(%rbp)
	jne	.L101
.L100:
	movq	-40(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$9, (%rax)
	movq	-8(%rbp), %rax
	movzbl	-17(%rbp), %edx
	movb	%dl, 8(%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_unary
	movq	-8(%rbp), %rdx
	movq	%rax, 24(%rdx)
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
.L99:
	movq	-40(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$4, %eax
	je	.L102
.L101:
	movq	-16(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	parser_parse_multiplicative, .-parser_parse_multiplicative
	.globl	parser_parse_additive
	.type	parser_parse_additive, @function
parser_parse_additive:
.LFB20:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_multiplicative
	movq	%rax, -16(%rbp)
	jmp	.L105
.L108:
	movq	-40(%rbp), %rax
	movzbl	48(%rax), %eax
	movb	%al, -17(%rbp)
	cmpb	$43, -17(%rbp)
	je	.L106
	cmpb	$45, -17(%rbp)
	jne	.L107
.L106:
	movq	-40(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$9, (%rax)
	movq	-8(%rbp), %rax
	movzbl	-17(%rbp), %edx
	movb	%dl, 8(%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_multiplicative
	movq	-8(%rbp), %rdx
	movq	%rax, 24(%rdx)
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
.L105:
	movq	-40(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$4, %eax
	je	.L108
.L107:
	movq	-16(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	parser_parse_additive, .-parser_parse_additive
	.globl	parser_parse_relational
	.type	parser_parse_relational, @function
parser_parse_relational:
.LFB21:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_additive
	movq	%rax, -16(%rbp)
	jmp	.L111
.L120:
	movq	-40(%rbp), %rax
	movzbl	48(%rax), %eax
	movb	%al, -17(%rbp)
	cmpb	$60, -17(%rbp)
	je	.L112
	cmpb	$62, -17(%rbp)
	je	.L112
	cmpb	$60, -17(%rbp)
	jne	.L113
	movq	-40(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$61, %al
	je	.L112
.L113:
	cmpb	$62, -17(%rbp)
	jne	.L114
	movq	-40(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$61, %al
	jne	.L114
.L112:
	movq	-40(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
	movb	$0, -18(%rbp)
	movq	-40(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$4, %eax
	jne	.L115
	movq	-40(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$61, %al
	jne	.L115
	movb	$1, -18(%rbp)
	movq	-40(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
.L115:
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$9, (%rax)
	cmpb	$0, -18(%rbp)
	je	.L116
	cmpb	$60, -17(%rbp)
	jne	.L117
	movl	$76, %eax
	jmp	.L119
.L117:
	movl	$71, %eax
	jmp	.L119
.L116:
	movzbl	-17(%rbp), %eax
.L119:
	movq	-8(%rbp), %rdx
	movb	%al, 8(%rdx)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_additive
	movq	-8(%rbp), %rdx
	movq	%rax, 24(%rdx)
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
.L111:
	movq	-40(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$4, %eax
	je	.L120
.L114:
	movq	-16(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	parser_parse_relational, .-parser_parse_relational
	.globl	parser_parse_equality
	.type	parser_parse_equality, @function
parser_parse_equality:
.LFB22:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_relational
	movq	%rax, -16(%rbp)
	jmp	.L123
.L128:
	movq	-40(%rbp), %rax
	movzbl	48(%rax), %eax
	movb	%al, -17(%rbp)
	cmpb	$61, -17(%rbp)
	je	.L124
	cmpb	$33, -17(%rbp)
	jne	.L125
.L124:
	movq	-40(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$61, %al
	jne	.L125
	movq	-40(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-40(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$9, (%rax)
	cmpb	$61, -17(%rbp)
	jne	.L126
	movl	$69, %edx
	jmp	.L127
.L126:
	movl	$78, %edx
.L127:
	movq	-8(%rbp), %rax
	movb	%dl, 8(%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_relational
	movq	-8(%rbp), %rdx
	movq	%rax, 24(%rdx)
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
.L123:
	movq	-40(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$4, %eax
	je	.L128
.L125:
	movq	-16(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	parser_parse_equality, .-parser_parse_equality
	.globl	parser_parse_logical_and
	.type	parser_parse_logical_and, @function
parser_parse_logical_and:
.LFB23:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_equality
	movq	%rax, -16(%rbp)
	jmp	.L131
.L133:
	movq	-24(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$9, (%rax)
	movq	-8(%rbp), %rax
	movb	$65, 8(%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_equality
	movq	-8(%rbp), %rdx
	movq	%rax, 24(%rdx)
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
.L131:
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$4, %eax
	jne	.L132
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$38, %al
	jne	.L132
	movq	-24(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$38, %al
	je	.L133
.L132:
	movq	-16(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE23:
	.size	parser_parse_logical_and, .-parser_parse_logical_and
	.globl	parser_parse_logical_or
	.type	parser_parse_logical_or, @function
parser_parse_logical_or:
.LFB24:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_logical_and
	movq	%rax, -16(%rbp)
	jmp	.L136
.L138:
	movq	-24(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$9, (%rax)
	movq	-8(%rbp), %rax
	movb	$79, 8(%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_logical_and
	movq	-8(%rbp), %rdx
	movq	%rax, 24(%rdx)
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
.L136:
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$4, %eax
	jne	.L137
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$124, %al
	jne	.L137
	movq	-24(%rbp), %rax
	movzbl	32(%rax), %eax
	cmpb	$124, %al
	je	.L138
.L137:
	movq	-16(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE24:
	.size	parser_parse_logical_or, .-parser_parse_logical_or
	.section	.rodata
.LC5:
	.string	"Invalid assignment target\n"
	.text
	.globl	parser_parse_assignment
	.type	parser_parse_assignment, @function
parser_parse_assignment:
.LFB25:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_logical_or
	movq	%rax, -16(%rbp)
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$4, %eax
	jne	.L141
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$61, %al
	jne	.L141
	movq	-16(%rbp), %rax
	movl	(%rax), %eax
	cmpl	$12, %eax
	je	.L142
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$26, %edx
	movl	$1, %esi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L142:
	movq	-24(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$4, (%rax)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_assignment
	movq	-8(%rbp), %rdx
	movq	%rax, 16(%rdx)
	movq	-8(%rbp), %rax
	jmp	.L143
.L141:
	movq	-16(%rbp), %rax
.L143:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE25:
	.size	parser_parse_assignment, .-parser_parse_assignment
	.globl	parser_parse_expression
	.type	parser_parse_expression, @function
parser_parse_expression:
.LFB26:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_assignment
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE26:
	.size	parser_parse_expression, .-parser_parse_expression
	.section	.rodata
.LC6:
	.string	"int"
.LC7:
	.string	"char"
.LC8:
	.string	"void"
	.align 8
.LC9:
	.string	"Expected type specifier at line %d, column %d\n"
	.align 8
.LC10:
	.string	"Expected identifier at line %d, column %d\n"
	.align 8
.LC11:
	.string	"Expected ';' at line %d, column %d\n"
	.text
	.globl	parser_parse_declaration
	.type	parser_parse_declaration, @function
parser_parse_declaration:
.LFB27:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -56(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movl	$3, (%rax)
	leaq	.LC6(%rip), %rax
	movq	%rax, -32(%rbp)
	leaq	.LC7(%rip), %rax
	movq	%rax, -24(%rbp)
	leaq	.LC8(%rip), %rax
	movq	%rax, -16(%rbp)
	movb	$0, -45(%rbp)
	movq	-56(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$3, %eax
	jne	.L147
	movl	$0, -44(%rbp)
	jmp	.L148
.L150:
	movq	-56(%rbp), %rax
	movl	48(%rax), %eax
	movl	-44(%rbp), %edx
	addl	$5, %edx
	cmpl	%edx, %eax
	jne	.L149
	movl	-44(%rbp), %eax
	cltq
	movq	-32(%rbp,%rax,8), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 8(%rax)
	movb	$1, -45(%rbp)
	jmp	.L147
.L149:
	addl	$1, -44(%rbp)
.L148:
	cmpl	$2, -44(%rbp)
	jle	.L150
.L147:
	movzbl	-45(%rbp), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L151
	movq	-56(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-56(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC9(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L151:
	movq	-56(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-56(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$1, %eax
	je	.L152
	movq	-56(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-56(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC10(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L152:
	movq	-56(%rbp), %rax
	movq	48(%rax), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 16(%rax)
	movq	-56(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-56(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$4, %eax
	jne	.L153
	movq	-56(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$61, %al
	jne	.L153
	movq	-56(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_expression
	movq	-40(%rbp), %rdx
	movq	%rax, 24(%rdx)
	jmp	.L154
.L153:
	movq	-40(%rbp), %rax
	movq	$0, 24(%rax)
.L154:
	movq	-56(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L155
	movq	-56(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$59, %al
	je	.L156
.L155:
	movq	-56(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-56(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC11(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L156:
	movq	-56(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-40(%rbp), %rax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L158
	call	__stack_chk_fail@PLT
.L158:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE27:
	.size	parser_parse_declaration, .-parser_parse_declaration
	.section	.rodata
	.align 8
.LC12:
	.string	"Expected '(' at line %d, column %d\n"
	.align 8
.LC13:
	.string	"Expected ')' at line %d, column %d\n"
	.text
	.globl	parser_parse_if
	.type	parser_parse_if, @function
parser_parse_if:
.LFB28:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$5, (%rax)
	movq	-24(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L160
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$40, %al
	je	.L161
.L160:
	movq	-24(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-24(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC12(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L161:
	movq	-24(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_expression
	movq	-8(%rbp), %rdx
	movq	%rax, 8(%rdx)
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L162
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$41, %al
	je	.L163
.L162:
	movq	-24(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-24(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC13(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L163:
	movq	-24(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_statement
	movq	-8(%rbp), %rdx
	movq	%rax, 16(%rdx)
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$3, %eax
	jne	.L164
	movq	-24(%rbp), %rax
	movl	48(%rax), %eax
	cmpl	$1, %eax
	jne	.L164
	movq	-24(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_statement
	movq	-8(%rbp), %rdx
	movq	%rax, 24(%rdx)
	jmp	.L165
.L164:
	movq	-8(%rbp), %rax
	movq	$0, 24(%rax)
.L165:
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE28:
	.size	parser_parse_if, .-parser_parse_if
	.globl	parser_parse_while
	.type	parser_parse_while, @function
parser_parse_while:
.LFB29:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$6, (%rax)
	movq	-24(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L168
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$40, %al
	je	.L169
.L168:
	movq	-24(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-24(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC12(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L169:
	movq	-24(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_expression
	movq	-8(%rbp), %rdx
	movq	%rax, 8(%rdx)
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L170
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$41, %al
	je	.L171
.L170:
	movq	-24(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-24(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC13(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L171:
	movq	-24(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_statement
	movq	-8(%rbp), %rdx
	movq	%rax, 16(%rdx)
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE29:
	.size	parser_parse_while, .-parser_parse_while
	.globl	parser_parse_for
	.type	parser_parse_for, @function
parser_parse_for:
.LFB30:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$7, (%rax)
	movq	-24(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L174
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$40, %al
	je	.L175
.L174:
	movq	-24(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-24(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC12(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L175:
	movq	-24(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$3, %eax
	jne	.L176
	movq	-24(%rbp), %rax
	movl	48(%rax), %eax
	cmpl	$5, %eax
	je	.L177
	movq	-24(%rbp), %rax
	movl	48(%rax), %eax
	cmpl	$6, %eax
	je	.L177
	movq	-24(%rbp), %rax
	movl	48(%rax), %eax
	cmpl	$7, %eax
	jne	.L176
.L177:
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_declaration
	movq	-8(%rbp), %rdx
	movq	%rax, 8(%rdx)
	jmp	.L178
.L176:
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L179
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$59, %al
	je	.L180
.L179:
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_expression
	movq	-8(%rbp), %rdx
	movq	%rax, 8(%rdx)
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L181
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$59, %al
	je	.L182
.L181:
	movq	-24(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-24(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC11(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L182:
	movq	-24(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	jmp	.L178
.L180:
	movq	-8(%rbp), %rax
	movq	$0, 8(%rax)
	movq	-24(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
.L178:
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L183
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$59, %al
	je	.L184
.L183:
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_expression
	movq	-8(%rbp), %rdx
	movq	%rax, 16(%rdx)
	jmp	.L185
.L184:
	movq	-8(%rbp), %rax
	movq	$0, 16(%rax)
.L185:
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L186
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$59, %al
	je	.L187
.L186:
	movq	-24(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-24(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC11(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L187:
	movq	-24(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L188
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$41, %al
	je	.L189
.L188:
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_expression
	movq	-8(%rbp), %rdx
	movq	%rax, 24(%rdx)
	jmp	.L190
.L189:
	movq	-8(%rbp), %rax
	movq	$0, 24(%rax)
.L190:
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L191
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$41, %al
	je	.L192
.L191:
	movq	-24(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-24(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC13(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L192:
	movq	-24(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_statement
	movq	-8(%rbp), %rdx
	movq	%rax, 32(%rdx)
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE30:
	.size	parser_parse_for, .-parser_parse_for
	.globl	parser_parse_return
	.type	parser_parse_return, @function
parser_parse_return:
.LFB31:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$8, (%rax)
	movq	-24(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L195
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$59, %al
	je	.L196
.L195:
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_expression
	movq	-8(%rbp), %rdx
	movq	%rax, 8(%rdx)
	jmp	.L197
.L196:
	movq	-8(%rbp), %rax
	movq	$0, 8(%rax)
.L197:
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L198
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$59, %al
	je	.L199
.L198:
	movq	-24(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-24(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC11(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L199:
	movq	-24(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE31:
	.size	parser_parse_return, .-parser_parse_return
	.section	.rodata
	.align 8
.LC14:
	.string	"Expected '{' at line %d, column %d\n"
	.text
	.globl	parser_parse_block
	.type	parser_parse_block, @function
parser_parse_block:
.LFB32:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -40(%rbp)
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movl	$2, (%rax)
	movq	-24(%rbp), %rax
	movq	$0, 8(%rax)
	movq	-24(%rbp), %rax
	movl	$0, 16(%rax)
	movq	-40(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L202
	movq	-40(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$123, %al
	je	.L203
.L202:
	movq	-40(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-40(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC14(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L203:
	movq	-40(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movl	$800, %edi
	call	malloc@PLT
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 8(%rax)
	jmp	.L204
.L205:
	movq	-24(%rbp), %rax
	movq	8(%rax), %rsi
	movq	-24(%rbp), %rax
	movl	16(%rax), %eax
	leal	1(%rax), %ecx
	movq	-24(%rbp), %rdx
	movl	%ecx, 16(%rdx)
	cltq
	salq	$3, %rax
	leaq	(%rsi,%rax), %rbx
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_statement
	movq	%rax, (%rbx)
.L204:
	movq	-40(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L205
	movq	-40(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$125, %al
	jne	.L205
	movq	-40(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-24(%rbp), %rax
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE32:
	.size	parser_parse_block, .-parser_parse_block
	.globl	parser_parse_expression_statement
	.type	parser_parse_expression_statement, @function
parser_parse_expression_statement:
.LFB33:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_expression
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L208
	movq	-24(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$59, %al
	je	.L209
.L208:
	movq	-24(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-24(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC11(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L209:
	movq	-24(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE33:
	.size	parser_parse_expression_statement, .-parser_parse_expression_statement
	.section	.rodata
	.align 8
.LC15:
	.string	"Unexpected keyword at line %d, column %d\n"
	.text
	.globl	parser_parse_statement
	.type	parser_parse_statement, @function
parser_parse_statement:
.LFB34:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$3, %eax
	jne	.L212
	movq	-8(%rbp), %rax
	movl	48(%rax), %eax
	cmpl	$7, %eax
	ja	.L213
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L215(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L215(%rip), %rdx
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L215:
	.long	.L219-.L215
	.long	.L213-.L215
	.long	.L218-.L215
	.long	.L217-.L215
	.long	.L216-.L215
	.long	.L214-.L215
	.long	.L214-.L215
	.long	.L214-.L215
	.text
.L219:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_if
	jmp	.L220
.L218:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_while
	jmp	.L220
.L217:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_for
	jmp	.L220
.L216:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_return
	jmp	.L220
.L214:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_declaration
	jmp	.L220
.L213:
	movq	-8(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-8(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC15(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L212:
	movq	-8(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L221
	movq	-8(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$123, %al
	jne	.L221
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_block
	jmp	.L220
.L221:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_expression_statement
.L220:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE34:
	.size	parser_parse_statement, .-parser_parse_statement
	.globl	parser_parse_parameter
	.type	parser_parse_parameter, @function
parser_parse_parameter:
.LFB35:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -56(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movl	$3, (%rax)
	leaq	.LC6(%rip), %rax
	movq	%rax, -32(%rbp)
	leaq	.LC7(%rip), %rax
	movq	%rax, -24(%rbp)
	leaq	.LC8(%rip), %rax
	movq	%rax, -16(%rbp)
	movb	$0, -45(%rbp)
	movq	-56(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$3, %eax
	jne	.L223
	movl	$0, -44(%rbp)
	jmp	.L224
.L226:
	movq	-56(%rbp), %rax
	movl	48(%rax), %eax
	movl	-44(%rbp), %edx
	addl	$5, %edx
	cmpl	%edx, %eax
	jne	.L225
	movl	-44(%rbp), %eax
	cltq
	movq	-32(%rbp,%rax,8), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 8(%rax)
	movb	$1, -45(%rbp)
	jmp	.L223
.L225:
	addl	$1, -44(%rbp)
.L224:
	cmpl	$2, -44(%rbp)
	jle	.L226
.L223:
	movzbl	-45(%rbp), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L227
	movq	-56(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-56(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC9(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L227:
	movq	-56(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-56(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$1, %eax
	je	.L228
	movq	-56(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-56(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC10(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L228:
	movq	-56(%rbp), %rax
	movq	48(%rax), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, 16(%rax)
	movq	-56(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-40(%rbp), %rax
	movq	$0, 24(%rax)
	movq	-40(%rbp), %rax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L230
	call	__stack_chk_fail@PLT
.L230:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE35:
	.size	parser_parse_parameter, .-parser_parse_parameter
	.section	.rodata
	.align 8
.LC16:
	.string	"Expected return type at line %d, column %d\n"
	.align 8
.LC17:
	.string	"Expected function name at line %d, column %d\n"
	.text
	.globl	parser_parse_function
	.type	parser_parse_function, @function
parser_parse_function:
.LFB36:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$72, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -72(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rax
	movl	$1, (%rax)
	leaq	.LC6(%rip), %rax
	movq	%rax, -48(%rbp)
	leaq	.LC7(%rip), %rax
	movq	%rax, -40(%rbp)
	leaq	.LC8(%rip), %rax
	movq	%rax, -32(%rbp)
	movb	$0, -61(%rbp)
	movq	-72(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$3, %eax
	jne	.L232
	movl	$0, -60(%rbp)
	jmp	.L233
.L235:
	movq	-72(%rbp), %rax
	movl	48(%rax), %eax
	movl	-60(%rbp), %edx
	addl	$5, %edx
	cmpl	%edx, %eax
	jne	.L234
	movl	-60(%rbp), %eax
	cltq
	movq	-48(%rbp,%rax,8), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, %rdx
	movq	-56(%rbp), %rax
	movq	%rdx, 40(%rax)
	movb	$1, -61(%rbp)
	jmp	.L232
.L234:
	addl	$1, -60(%rbp)
.L233:
	cmpl	$2, -60(%rbp)
	jle	.L235
.L232:
	movzbl	-61(%rbp), %eax
	xorl	$1, %eax
	testb	%al, %al
	je	.L236
	movq	-72(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-72(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC16(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L236:
	movq	-72(%rbp), %rax
	movl	$3, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-72(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$1, %eax
	je	.L237
	movq	-72(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-72(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC17(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L237:
	movq	-72(%rbp), %rax
	movq	48(%rax), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, %rdx
	movq	-56(%rbp), %rax
	movq	%rdx, 8(%rax)
	movq	-72(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-72(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L238
	movq	-72(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$40, %al
	je	.L239
.L238:
	movq	-72(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-72(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC12(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L239:
	movq	-72(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movl	$80, %edi
	call	malloc@PLT
	movq	%rax, %rdx
	movq	-56(%rbp), %rax
	movq	%rdx, 16(%rax)
	movq	-56(%rbp), %rax
	movl	$0, 24(%rax)
	movq	-72(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L242
	movq	-72(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$41, %al
	je	.L241
.L242:
	movq	-56(%rbp), %rax
	movq	16(%rax), %rsi
	movq	-56(%rbp), %rax
	movl	24(%rax), %eax
	leal	1(%rax), %ecx
	movq	-56(%rbp), %rdx
	movl	%ecx, 24(%rdx)
	cltq
	salq	$3, %rax
	leaq	(%rsi,%rax), %rbx
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_parameter
	movq	%rax, (%rbx)
	movq	-72(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L241
	movq	-72(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$44, %al
	jne	.L241
	movq	-72(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	jmp	.L242
.L241:
	movq	-72(%rbp), %rax
	movl	40(%rax), %eax
	cmpl	$5, %eax
	jne	.L243
	movq	-72(%rbp), %rax
	movzbl	48(%rax), %eax
	cmpb	$41, %al
	je	.L244
.L243:
	movq	-72(%rbp), %rax
	movl	60(%rax), %ecx
	movq	-72(%rbp), %rax
	movl	56(%rax), %edx
	movq	stderr(%rip), %rax
	leaq	.LC13(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L244:
	movq	-72(%rbp), %rax
	movl	$5, %esi
	movq	%rax, %rdi
	call	parser_eat
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_block
	movq	-56(%rbp), %rdx
	movq	%rax, 32(%rdx)
	movq	-56(%rbp), %rax
	movq	-24(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L246
	call	__stack_chk_fail@PLT
.L246:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE36:
	.size	parser_parse_function, .-parser_parse_function
	.globl	parser_parse_program
	.type	parser_parse_program, @function
parser_parse_program:
.LFB37:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -40(%rbp)
	movl	$48, %edi
	call	malloc@PLT
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movl	$0, (%rax)
	movl	$800, %edi
	call	malloc@PLT
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 8(%rax)
	movq	-24(%rbp), %rax
	movl	$0, 16(%rax)
	jmp	.L248
.L249:
	movq	-24(%rbp), %rax
	movq	8(%rax), %rsi
	movq	-24(%rbp), %rax
	movl	16(%rax), %eax
	leal	1(%rax), %ecx
	movq	-24(%rbp), %rdx
	movl	%ecx, 16(%rdx)
	cltq
	salq	$3, %rax
	leaq	(%rsi,%rax), %rbx
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_function
	movq	%rax, (%rbx)
.L248:
	movq	-40(%rbp), %rax
	movl	40(%rax), %eax
	testl	%eax, %eax
	jne	.L249
	movq	-24(%rbp), %rax
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE37:
	.size	parser_parse_program, .-parser_parse_program
	.globl	parser_parse
	.type	parser_parse, @function
parser_parse:
.LFB38:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_program
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE38:
	.size	parser_parse, .-parser_parse
	.globl	symbol_table_create
	.type	symbol_table_create, @function
symbol_table_create:
.LFB39:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$16, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	$0, (%rax)
	movq	-8(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE39:
	.size	symbol_table_create, .-symbol_table_create
	.globl	symbol_table_add
	.type	symbol_table_add, @function
symbol_table_add:
.LFB40:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movl	%ecx, -44(%rbp)
	movl	$32, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 8(%rax)
	movq	-8(%rbp), %rax
	movl	-44(%rbp), %edx
	movl	%edx, 16(%rax)
	movq	-8(%rbp), %rax
	movl	$0, 20(%rax)
	movq	-24(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 24(%rax)
	movq	-24(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, (%rax)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE40:
	.size	symbol_table_add, .-symbol_table_add
	.globl	symbol_table_lookup
	.type	symbol_table_lookup, @function
symbol_table_lookup:
.LFB41:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -16(%rbp)
	jmp	.L257
.L262:
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	jmp	.L258
.L261:
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L259
	movq	-8(%rbp), %rax
	jmp	.L260
.L259:
	movq	-8(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rax, -8(%rbp)
.L258:
	cmpq	$0, -8(%rbp)
	jne	.L261
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -16(%rbp)
.L257:
	cmpq	$0, -16(%rbp)
	jne	.L262
	movl	$0, %eax
.L260:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE41:
	.size	symbol_table_lookup, .-symbol_table_lookup
	.section	.rodata
.LC18:
	.string	"Undeclared identifier: %s\n"
.LC19:
	.string	"Undeclared function: %s\n"
.LC20:
	.string	"%s is not a function\n"
	.text
	.globl	semantic_check_expression
	.type	semantic_check_expression, @function
semantic_check_expression:
.LFB42:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	cmpq	$0, -40(%rbp)
	je	.L278
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	subl	$4, %eax
	cmpl	$8, %eax
	ja	.L279
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L268(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L268(%rip), %rdx
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L268:
	.long	.L272-.L268
	.long	.L279-.L268
	.long	.L279-.L268
	.long	.L279-.L268
	.long	.L279-.L268
	.long	.L271-.L268
	.long	.L270-.L268
	.long	.L269-.L268
	.long	.L267-.L268
	.text
.L271:
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
	jmp	.L263
.L270:
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
	jmp	.L263
.L267:
	movq	-40(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-48(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	symbol_table_lookup
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jne	.L280
	movq	-40(%rbp), %rax
	movq	8(%rax), %rdx
	movq	stderr(%rip), %rax
	leaq	.LC18(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L269:
	movq	-40(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-48(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	symbol_table_lookup
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L274
	movq	-40(%rbp), %rax
	movq	8(%rax), %rdx
	movq	stderr(%rip), %rax
	leaq	.LC19(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L274:
	movq	-8(%rbp), %rax
	movl	16(%rax), %eax
	cmpl	$1, %eax
	je	.L275
	movq	-40(%rbp), %rax
	movq	8(%rax), %rdx
	movq	stderr(%rip), %rax
	leaq	.LC20(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L275:
	movl	$0, -20(%rbp)
	jmp	.L276
.L277:
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movl	-20(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
	addl	$1, -20(%rbp)
.L276:
	movq	-40(%rbp), %rax
	movl	24(%rax), %eax
	cmpl	%eax, -20(%rbp)
	jl	.L277
	jmp	.L263
.L272:
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
	jmp	.L263
.L278:
	nop
	jmp	.L263
.L279:
	nop
	jmp	.L263
.L280:
	nop
.L263:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE42:
	.size	semantic_check_expression, .-semantic_check_expression
	.section	.rodata
.LC21:
	.string	"Redeclaration of '%s'\n"
	.text
	.globl	semantic_check_statement
	.type	semantic_check_statement, @function
semantic_check_statement:
.LFB43:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	cmpq	$0, -40(%rbp)
	je	.L302
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	cmpl	$14, %eax
	ja	.L303
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L286(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L286(%rip), %rdx
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L286:
	.long	.L303-.L286
	.long	.L303-.L286
	.long	.L292-.L286
	.long	.L291-.L286
	.long	.L285-.L286
	.long	.L290-.L286
	.long	.L289-.L286
	.long	.L288-.L286
	.long	.L287-.L286
	.long	.L285-.L286
	.long	.L285-.L286
	.long	.L285-.L286
	.long	.L285-.L286
	.long	.L285-.L286
	.long	.L285-.L286
	.text
.L292:
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	symbol_table_create
	movq	%rax, -8(%rbp)
	movl	$0, -28(%rbp)
	jmp	.L293
.L294:
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movl	-28(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_statement
	addl	$1, -28(%rbp)
.L293:
	movq	-40(%rbp), %rax
	movl	16(%rax), %eax
	cmpl	%eax, -28(%rbp)
	jl	.L294
	jmp	.L281
.L291:
	movq	-40(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-48(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	symbol_table_lookup
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	je	.L295
	movq	-16(%rbp), %rax
	movq	24(%rax), %rdx
	movq	-48(%rbp), %rax
	movq	(%rax), %rax
	cmpq	%rax, %rdx
	jne	.L295
	movq	-40(%rbp), %rax
	movq	16(%rax), %rdx
	movq	stderr(%rip), %rax
	leaq	.LC21(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L295:
	movq	-40(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-40(%rbp), %rax
	movq	16(%rax), %rsi
	movq	-48(%rbp), %rax
	movl	$0, %ecx
	movq	%rax, %rdi
	call	symbol_table_add
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	je	.L304
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
	jmp	.L304
.L290:
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_statement
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	je	.L305
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_statement
	jmp	.L305
.L289:
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_statement
	jmp	.L281
.L288:
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	symbol_table_create
	movq	%rax, -24(%rbp)
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L298
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_statement
.L298:
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	testq	%rax, %rax
	je	.L299
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
.L299:
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	je	.L300
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
.L300:
	movq	-40(%rbp), %rax
	movq	32(%rax), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_statement
	jmp	.L281
.L287:
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L306
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	-48(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
	jmp	.L306
.L285:
	movq	-48(%rbp), %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_expression
	jmp	.L281
.L302:
	nop
	jmp	.L281
.L303:
	nop
	jmp	.L281
.L304:
	nop
	jmp	.L281
.L305:
	nop
	jmp	.L281
.L306:
	nop
.L281:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE43:
	.size	semantic_check_statement, .-semantic_check_statement
	.globl	semantic_check_function
	.type	semantic_check_function, @function
semantic_check_function:
.LFB44:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	symbol_table_create
	movq	%rax, -16(%rbp)
	movl	$0, -20(%rbp)
	jmp	.L308
.L309:
	movq	-40(%rbp), %rax
	movq	16(%rax), %rax
	movl	-20(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	16(%rax), %rsi
	movq	-16(%rbp), %rax
	movl	$0, %ecx
	movq	%rax, %rdi
	call	symbol_table_add
	addl	$1, -20(%rbp)
.L308:
	movq	-40(%rbp), %rax
	movl	24(%rax), %eax
	cmpl	%eax, -20(%rbp)
	jl	.L309
	movq	-40(%rbp), %rax
	movq	32(%rax), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_statement
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE44:
	.size	semantic_check_function, .-semantic_check_function
	.section	.rodata
.LC22:
	.string	"print"
	.text
	.globl	semantic_check_program
	.type	semantic_check_program, @function
semantic_check_program:
.LFB45:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movl	$0, %edi
	call	symbol_table_create
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movl	$1, %ecx
	leaq	.LC8(%rip), %rdx
	leaq	.LC22(%rip), %rsi
	movq	%rax, %rdi
	call	symbol_table_add
	movl	$0, -24(%rbp)
	jmp	.L311
.L312:
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movl	-24(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	40(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	8(%rax), %rsi
	movq	-16(%rbp), %rax
	movl	$1, %ecx
	movq	%rax, %rdi
	call	symbol_table_add
	addl	$1, -24(%rbp)
.L311:
	movq	-40(%rbp), %rax
	movl	16(%rax), %eax
	cmpl	%eax, -24(%rbp)
	jl	.L312
	movl	$0, -20(%rbp)
	jmp	.L313
.L314:
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movl	-20(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	semantic_check_function
	addl	$1, -20(%rbp)
.L313:
	movq	-40(%rbp), %rax
	movl	16(%rax), %eax
	cmpl	%eax, -20(%rbp)
	jl	.L314
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE45:
	.size	semantic_check_program, .-semantic_check_program
	.globl	codegen_init
	.type	codegen_init, @function
codegen_init:
.LFB46:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$1000, 12(%rax)
	movq	-8(%rbp), %rax
	movl	12(%rax), %eax
	cltq
	salq	$3, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-8(%rbp), %rax
	movl	$0, 8(%rax)
	movl	$0, %edi
	call	symbol_table_create
	movq	-8(%rbp), %rdx
	movq	%rax, 16(%rdx)
	movq	-8(%rbp), %rax
	movq	$0, 24(%rax)
	movq	-8(%rbp), %rax
	movl	$0, 32(%rax)
	movl	$800, %edi
	call	malloc@PLT
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 40(%rax)
	movl	$400, %edi
	call	malloc@PLT
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 48(%rax)
	movq	-8(%rbp), %rax
	movl	$0, 56(%rax)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE46:
	.size	codegen_init, .-codegen_init
	.globl	codegen_add
	.type	codegen_add, @function
codegen_add:
.LFB47:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movq	-8(%rbp), %rax
	movl	8(%rax), %edx
	movq	-8(%rbp), %rax
	movl	12(%rax), %eax
	cmpl	%eax, %edx
	jl	.L317
	movq	-8(%rbp), %rax
	movl	12(%rax), %eax
	leal	(%rax,%rax), %edx
	movq	-8(%rbp), %rax
	movl	%edx, 12(%rax)
	movq	-8(%rbp), %rax
	movl	12(%rax), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	realloc@PLT
	movq	-8(%rbp), %rdx
	movq	%rax, (%rdx)
.L317:
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	cltq
	salq	$3, %rax
	addq	%rax, %rdx
	movl	-12(%rbp), %eax
	movl	%eax, (%rdx)
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	cltq
	salq	$3, %rax
	addq	%rax, %rdx
	movl	-16(%rbp), %eax
	movl	%eax, 4(%rdx)
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	leal	1(%rax), %edx
	movq	-8(%rbp), %rax
	movl	%edx, 8(%rax)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE47:
	.size	codegen_add, .-codegen_add
	.globl	codegen_get_address
	.type	codegen_get_address, @function
codegen_get_address:
.LFB48:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE48:
	.size	codegen_get_address, .-codegen_get_address
	.globl	codegen_patch
	.type	codegen_patch, @function
codegen_patch:
.LFB49:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rax, %rdx
	movl	-16(%rbp), %eax
	movl	%eax, 4(%rdx)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE49:
	.size	codegen_patch, .-codegen_patch
	.section	.rodata
.LC23:
	.string	"Function not found: %s\n"
	.align 8
.LC24:
	.string	"Unsupported node type in codegen_expression\n"
	.text
	.globl	codegen_expression
	.type	codegen_expression, @function
codegen_expression:
.LFB50:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%rdi, -72(%rbp)
	movq	%rsi, -80(%rbp)
	movq	-80(%rbp), %rax
	movl	(%rax), %eax
	subl	$4, %eax
	cmpl	$9, %eax
	ja	.L322
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L324(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L324(%rip), %rdx
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L324:
	.long	.L329-.L324
	.long	.L322-.L324
	.long	.L322-.L324
	.long	.L322-.L324
	.long	.L322-.L324
	.long	.L328-.L324
	.long	.L327-.L324
	.long	.L326-.L324
	.long	.L325-.L324
	.long	.L323-.L324
	.text
.L323:
	movq	-80(%rbp), %rax
	movl	8(%rax), %edx
	movq	-72(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L330
.L325:
	movq	$0, -40(%rbp)
	movq	-72(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	je	.L331
	movq	-80(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	symbol_table_lookup
	movq	%rax, -40(%rbp)
.L331:
	cmpq	$0, -40(%rbp)
	je	.L332
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	je	.L333
	movq	-40(%rbp), %rax
	movq	24(%rax), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	jne	.L334
	movl	$1, %eax
	jmp	.L336
.L334:
	movl	$2, %eax
	jmp	.L336
.L333:
	movl	$0, %eax
.L336:
	movl	%eax, -48(%rbp)
	movl	-48(%rbp), %edx
	movq	-72(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L330
.L332:
	movq	-80(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	symbol_table_lookup
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	setne	%al
	movzbl	%al, %edx
	movq	-72(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L330
.L328:
	movq	-80(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
	movq	-80(%rbp), %rax
	movq	24(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
	movq	-80(%rbp), %rax
	movzbl	8(%rax), %eax
	movsbl	%al, %eax
	subl	$37, %eax
	cmpl	$42, %eax
	ja	.L371
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L340(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L340(%rip), %rdx
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L340:
	.long	.L352-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L351-.L340
	.long	.L350-.L340
	.long	.L371-.L340
	.long	.L349-.L340
	.long	.L371-.L340
	.long	.L348-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L347-.L340
	.long	.L371-.L340
	.long	.L346-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L345-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L344-.L340
	.long	.L371-.L340
	.long	.L343-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L371-.L340
	.long	.L342-.L340
	.long	.L371-.L340
	.long	.L341-.L340
	.long	.L339-.L340
	.text
.L350:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$3, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L338
.L349:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$4, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L338
.L351:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$5, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L338
.L348:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$6, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L338
.L352:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$7, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L338
.L344:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$8, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L338
.L341:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$9, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L338
.L347:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$10, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L338
.L346:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$12, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L338
.L342:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$11, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L338
.L343:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$13, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L338
.L345:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$14, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L338
.L339:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$15, %esi
	movq	%rax, %rdi
	call	codegen_add
	nop
.L338:
	jmp	.L371
.L327:
	movq	-80(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
	movq	-80(%rbp), %rax
	movzbl	8(%rax), %eax
	movsbl	%al, %eax
	cmpl	$33, %eax
	je	.L353
	cmpl	$45, %eax
	jne	.L372
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$17, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L354
.L353:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$16, %esi
	movq	%rax, %rdi
	call	codegen_add
	nop
.L354:
	jmp	.L372
.L329:
	movq	-80(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
	movq	-80(%rbp), %rax
	movq	8(%rax), %rax
	movq	8(%rax), %rax
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	movq	-72(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	je	.L355
	movq	-72(%rbp), %rax
	movq	24(%rax), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	symbol_table_lookup
	movq	%rax, -32(%rbp)
.L355:
	cmpq	$0, -32(%rbp)
	je	.L356
	movq	-32(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	je	.L357
	movq	-32(%rbp), %rax
	movq	24(%rax), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	jne	.L358
	movl	$1, %eax
	jmp	.L360
.L358:
	movl	$2, %eax
	jmp	.L360
.L357:
	movl	$0, %eax
.L360:
	movl	%eax, -44(%rbp)
	movl	-44(%rbp), %edx
	movq	-72(%rbp), %rax
	movl	$2, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L330
.L356:
	movq	-72(%rbp), %rax
	movq	16(%rax), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	symbol_table_lookup
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	setne	%al
	movzbl	%al, %edx
	movq	-72(%rbp), %rax
	movl	$2, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L330
.L326:
	movl	$0, -60(%rbp)
	jmp	.L362
.L363:
	movq	-80(%rbp), %rax
	movq	16(%rax), %rax
	movl	-60(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
	addl	$1, -60(%rbp)
.L362:
	movq	-80(%rbp), %rax
	movl	24(%rax), %eax
	cmpl	%eax, -60(%rbp)
	jl	.L363
	movl	$-1, -56(%rbp)
	movl	$0, -52(%rbp)
	jmp	.L364
.L367:
	movq	-80(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	40(%rax), %rax
	movl	-52(%rbp), %ecx
	movslq	%ecx, %rcx
	salq	$3, %rcx
	addq	%rcx, %rax
	movq	(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L365
	movq	-72(%rbp), %rax
	movq	48(%rax), %rax
	movl	-52(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, -56(%rbp)
	jmp	.L366
.L365:
	addl	$1, -52(%rbp)
.L364:
	movq	-72(%rbp), %rax
	movl	56(%rax), %eax
	cmpl	%eax, -52(%rbp)
	jl	.L367
.L366:
	cmpl	$-1, -56(%rbp)
	jne	.L368
	movq	-80(%rbp), %rax
	movq	8(%rax), %rax
	leaq	.LC22(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L369
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$22, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L330
.L369:
	movq	-80(%rbp), %rax
	movq	8(%rax), %rdx
	movq	stderr(%rip), %rax
	leaq	.LC23(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L368:
	movl	-56(%rbp), %edx
	movq	-72(%rbp), %rax
	movl	$20, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L330
.L322:
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$44, %edx
	movl	$1, %esi
	leaq	.LC24(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L371:
	nop
	jmp	.L373
.L372:
	nop
.L330:
.L373:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE50:
	.size	codegen_expression, .-codegen_expression
	.section	.rodata
.LC25:
	.string	"Undefined variable: %s\n"
	.text
	.globl	codegen_statement
	.type	codegen_statement, @function
codegen_statement:
.LFB51:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movq	%rdi, -72(%rbp)
	movq	%rsi, -80(%rbp)
	movq	-80(%rbp), %rax
	movl	(%rax), %eax
	cmpl	$8, %eax
	ja	.L375
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L377(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L377(%rip), %rdx
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L377:
	.long	.L375-.L377
	.long	.L375-.L377
	.long	.L383-.L377
	.long	.L382-.L377
	.long	.L381-.L377
	.long	.L380-.L377
	.long	.L379-.L377
	.long	.L378-.L377
	.long	.L376-.L377
	.text
.L383:
	movq	-72(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	symbol_table_create
	movq	-72(%rbp), %rdx
	movq	%rax, 24(%rdx)
	movl	$0, -52(%rbp)
	jmp	.L384
.L385:
	movq	-80(%rbp), %rax
	movq	8(%rax), %rax
	movl	-52(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_statement
	addl	$1, -52(%rbp)
.L384:
	movq	-80(%rbp), %rax
	movl	16(%rax), %eax
	cmpl	%eax, -52(%rbp)
	jl	.L385
	movq	-72(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, 24(%rax)
	jmp	.L386
.L382:
	movq	-80(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-80(%rbp), %rax
	movq	16(%rax), %rsi
	movq	-72(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	jne	.L387
	movq	-72(%rbp), %rax
	movq	16(%rax), %rax
	jmp	.L388
.L387:
	movq	-72(%rbp), %rax
	movq	24(%rax), %rax
.L388:
	movl	$0, %ecx
	movq	%rax, %rdi
	call	symbol_table_add
	movq	-80(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	je	.L401
	movq	-80(%rbp), %rax
	movq	24(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
	jmp	.L401
.L381:
	movq	-80(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
	movq	-80(%rbp), %rax
	movq	8(%rax), %rax
	movl	(%rax), %eax
	cmpl	$12, %eax
	jne	.L390
	movq	-80(%rbp), %rax
	movq	8(%rax), %rax
	movq	8(%rax), %rax
	movq	%rax, -24(%rbp)
	movq	-72(%rbp), %rax
	movq	24(%rax), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	symbol_table_lookup
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jne	.L391
	movq	-72(%rbp), %rax
	movq	16(%rax), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	symbol_table_lookup
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jne	.L392
	movq	stderr(%rip), %rax
	movq	-24(%rbp), %rdx
	leaq	.LC25(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L392:
	movq	-16(%rbp), %rax
	movl	20(%rax), %edx
	movq	-72(%rbp), %rax
	movl	$2, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L386
.L391:
	movq	-16(%rbp), %rax
	movl	20(%rax), %edx
	movq	-72(%rbp), %rax
	movl	$2, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L386
.L390:
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$26, %edx
	movl	$1, %esi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L380:
	movq	-80(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	codegen_get_address
	movl	%eax, -32(%rbp)
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$19, %esi
	movq	%rax, %rdi
	call	codegen_add
	movq	-80(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_statement
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	codegen_get_address
	movl	%eax, -28(%rbp)
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$18, %esi
	movq	%rax, %rdi
	call	codegen_add
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	codegen_get_address
	movl	%eax, %edx
	movl	-32(%rbp), %ecx
	movq	-72(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	codegen_patch
	movq	-80(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	je	.L394
	movq	-80(%rbp), %rax
	movq	24(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_statement
.L394:
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	codegen_get_address
	movl	%eax, %edx
	movl	-28(%rbp), %ecx
	movq	-72(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	codegen_patch
	jmp	.L386
.L379:
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	codegen_get_address
	movl	%eax, -40(%rbp)
	movq	-80(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	codegen_get_address
	movl	%eax, -36(%rbp)
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$19, %esi
	movq	%rax, %rdi
	call	codegen_add
	movq	-80(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_statement
	movl	-40(%rbp), %edx
	movq	-72(%rbp), %rax
	movl	$18, %esi
	movq	%rax, %rdi
	call	codegen_add
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	codegen_get_address
	movl	%eax, %edx
	movl	-36(%rbp), %ecx
	movq	-72(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	codegen_patch
	jmp	.L386
.L378:
	movq	-80(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L395
	movq	-80(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_statement
.L395:
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	codegen_get_address
	movl	%eax, -48(%rbp)
	movq	-80(%rbp), %rax
	movq	16(%rax), %rax
	testq	%rax, %rax
	je	.L396
	movq	-80(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
	jmp	.L397
.L396:
	movq	-72(%rbp), %rax
	movl	$1, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	codegen_add
.L397:
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	codegen_get_address
	movl	%eax, -44(%rbp)
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$19, %esi
	movq	%rax, %rdi
	call	codegen_add
	movq	-80(%rbp), %rax
	movq	32(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_statement
	movq	-80(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	je	.L398
	movq	-80(%rbp), %rax
	movq	24(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
.L398:
	movl	-48(%rbp), %edx
	movq	-72(%rbp), %rax
	movl	$18, %esi
	movq	%rax, %rdi
	call	codegen_add
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	codegen_get_address
	movl	%eax, %edx
	movl	-44(%rbp), %ecx
	movq	-72(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	codegen_patch
	jmp	.L386
.L376:
	movq	-80(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L399
	movq	-80(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
	jmp	.L400
.L399:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	codegen_add
.L400:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$21, %esi
	movq	%rax, %rdi
	call	codegen_add
	jmp	.L386
.L375:
	movq	-80(%rbp), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_expression
	jmp	.L386
.L401:
	nop
.L386:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE51:
	.size	codegen_statement, .-codegen_statement
	.globl	codegen_function
	.type	codegen_function, @function
codegen_function:
.LFB52:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$72, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -72(%rbp)
	movq	%rsi, -80(%rbp)
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	codegen_get_address
	movl	%eax, -48(%rbp)
	movq	-72(%rbp), %rax
	movl	56(%rax), %eax
	addl	$1, %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-72(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	realloc@PLT
	movq	-72(%rbp), %rdx
	movq	%rax, 40(%rdx)
	movq	-72(%rbp), %rax
	movl	56(%rax), %eax
	addl	$1, %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-72(%rbp), %rax
	movq	48(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	realloc@PLT
	movq	-72(%rbp), %rdx
	movq	%rax, 48(%rdx)
	movq	-80(%rbp), %rax
	movq	8(%rax), %rax
	movq	-72(%rbp), %rdx
	movq	40(%rdx), %rcx
	movq	-72(%rbp), %rdx
	movl	56(%rdx), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	leaq	(%rcx,%rdx), %rbx
	movq	%rax, %rdi
	call	strdup@PLT
	movq	%rax, (%rbx)
	movq	-72(%rbp), %rax
	movq	48(%rax), %rdx
	movq	-72(%rbp), %rax
	movl	56(%rax), %eax
	cltq
	salq	$2, %rax
	addq	%rax, %rdx
	movl	-48(%rbp), %eax
	movl	%eax, (%rdx)
	movq	-72(%rbp), %rax
	movl	56(%rax), %eax
	leal	1(%rax), %edx
	movq	-72(%rbp), %rax
	movl	%edx, 56(%rax)
	movq	-72(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rax, -40(%rbp)
	movq	-72(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, %rdi
	call	symbol_table_create
	movq	-72(%rbp), %rdx
	movq	%rax, 24(%rdx)
	movq	-72(%rbp), %rax
	movl	32(%rax), %eax
	movl	%eax, -44(%rbp)
	movq	-72(%rbp), %rax
	movl	$0, 32(%rax)
	movl	$0, -52(%rbp)
	jmp	.L403
.L404:
	movq	-80(%rbp), %rax
	movq	16(%rax), %rax
	movl	-52(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-32(%rbp), %rax
	movq	16(%rax), %rsi
	movq	-72(%rbp), %rax
	movq	24(%rax), %rax
	movl	$0, %ecx
	movq	%rax, %rdi
	call	symbol_table_add
	movq	-32(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	24(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	symbol_table_lookup
	movq	%rax, -24(%rbp)
	movq	-72(%rbp), %rax
	movl	32(%rax), %eax
	leal	1(%rax), %ecx
	movq	-72(%rbp), %rdx
	movl	%ecx, 32(%rdx)
	movq	-24(%rbp), %rdx
	movl	%eax, 20(%rdx)
	addl	$1, -52(%rbp)
.L403:
	movq	-80(%rbp), %rax
	movl	24(%rax), %eax
	cmpl	%eax, -52(%rbp)
	jl	.L404
	movq	-80(%rbp), %rax
	movq	32(%rax), %rdx
	movq	-72(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_statement
	movq	-72(%rbp), %rax
	movl	8(%rax), %eax
	testl	%eax, %eax
	je	.L405
	movq	-72(%rbp), %rax
	movq	(%rax), %rdx
	movq	-72(%rbp), %rax
	movl	8(%rax), %eax
	cltq
	salq	$3, %rax
	subq	$8, %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	cmpl	$21, %eax
	je	.L406
.L405:
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	codegen_add
	movq	-72(%rbp), %rax
	movl	$0, %edx
	movl	$21, %esi
	movq	%rax, %rdi
	call	codegen_add
.L406:
	movq	-72(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rdx, 24(%rax)
	movq	-72(%rbp), %rax
	movl	-44(%rbp), %edx
	movl	%edx, 32(%rax)
	nop
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE52:
	.size	codegen_function, .-codegen_function
	.globl	codegen_program
	.type	codegen_program, @function
codegen_program:
.LFB53:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	codegen_init
	movl	$0, %edi
	call	symbol_table_create
	movq	-24(%rbp), %rdx
	movq	%rax, 16(%rdx)
	movl	$0, -4(%rbp)
	jmp	.L408
.L409:
	movq	-32(%rbp), %rax
	movq	8(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_function
	addl	$1, -4(%rbp)
.L408:
	movq	-32(%rbp), %rax
	movl	16(%rax), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L409
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE53:
	.size	codegen_program, .-codegen_program
	.globl	vm_init
	.type	vm_init, @function
vm_init:
.LFB54:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$1024, 12(%rax)
	movq	-8(%rbp), %rax
	movl	12(%rax), %eax
	cltq
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-8(%rbp), %rax
	movl	$0, 8(%rax)
	movq	-8(%rbp), %rax
	movl	$0, 16(%rax)
	movq	-8(%rbp), %rax
	movl	$0, 20(%rax)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE54:
	.size	vm_init, .-vm_init
	.globl	vm_push
	.type	vm_push, @function
vm_push:
.LFB55:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movq	-8(%rbp), %rax
	movl	8(%rax), %edx
	movq	-8(%rbp), %rax
	movl	12(%rax), %eax
	cmpl	%eax, %edx
	jl	.L412
	movq	-8(%rbp), %rax
	movl	12(%rax), %eax
	leal	(%rax,%rax), %edx
	movq	-8(%rbp), %rax
	movl	%edx, 12(%rax)
	movq	-8(%rbp), %rax
	movl	12(%rax), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	realloc@PLT
	movq	-8(%rbp), %rdx
	movq	%rax, (%rdx)
.L412:
	movq	-8(%rbp), %rax
	movq	(%rax), %rsi
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	leal	1(%rax), %ecx
	movq	-8(%rbp), %rdx
	movl	%ecx, 8(%rdx)
	cltq
	salq	$2, %rax
	leaq	(%rsi,%rax), %rdx
	movl	-12(%rbp), %eax
	movl	%eax, (%rdx)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE55:
	.size	vm_push, .-vm_push
	.section	.rodata
.LC26:
	.string	"Stack underflow\n"
	.text
	.globl	vm_pop
	.type	vm_pop, @function
vm_pop:
.LFB56:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	testl	%eax, %eax
	jg	.L414
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$16, %edx
	movl	$1, %esi
	leaq	.LC26(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L414:
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	leal	-1(%rax), %ecx
	movq	-8(%rbp), %rax
	movl	%ecx, 8(%rax)
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	cltq
	salq	$2, %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE56:
	.size	vm_pop, .-vm_pop
	.section	.rodata
.LC27:
	.string	"main"
.LC28:
	.string	"Division by zero\n"
.LC29:
	.string	"Modulo by zero\n"
.LC30:
	.string	"Program exited with code: %d\n"
.LC31:
	.string	"%d\n"
.LC32:
	.string	"Unknown opcode: %d\n"
	.text
	.globl	vm_execute
	.type	vm_execute, @function
vm_execute:
.LFB57:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$200, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -200(%rbp)
	movq	%rsi, -208(%rbp)
	movl	$0, -180(%rbp)
	jmp	.L417
.L420:
	movq	-208(%rbp), %rax
	movq	40(%rax), %rax
	movl	-180(%rbp), %edx
	movslq	%edx, %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movq	(%rax), %rax
	leaq	.LC27(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcmp@PLT
	testl	%eax, %eax
	jne	.L418
	movq	-208(%rbp), %rax
	movq	48(%rax), %rax
	movl	-180(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %edx
	movq	-200(%rbp), %rax
	movl	%edx, 16(%rax)
	jmp	.L419
.L418:
	addl	$1, -180(%rbp)
.L417:
	movq	-208(%rbp), %rax
	movl	56(%rax), %eax
	cmpl	%eax, -180(%rbp)
	jl	.L420
.L419:
	jmp	.L421
.L461:
	movq	-208(%rbp), %rax
	movq	(%rax), %rsi
	movq	-200(%rbp), %rax
	movl	16(%rax), %eax
	leal	1(%rax), %ecx
	movq	-200(%rbp), %rdx
	movl	%ecx, 16(%rdx)
	cltq
	salq	$3, %rax
	addq	%rsi, %rax
	movq	(%rax), %rax
	movq	%rax, -32(%rbp)
	movl	-32(%rbp), %eax
	cmpl	$22, %eax
	ja	.L422
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L424(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L424(%rip), %rdx
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L424:
	.long	.L446-.L424
	.long	.L445-.L424
	.long	.L444-.L424
	.long	.L443-.L424
	.long	.L442-.L424
	.long	.L441-.L424
	.long	.L440-.L424
	.long	.L439-.L424
	.long	.L438-.L424
	.long	.L437-.L424
	.long	.L436-.L424
	.long	.L435-.L424
	.long	.L434-.L424
	.long	.L433-.L424
	.long	.L432-.L424
	.long	.L431-.L424
	.long	.L430-.L424
	.long	.L429-.L424
	.long	.L428-.L424
	.long	.L427-.L424
	.long	.L426-.L424
	.long	.L425-.L424
	.long	.L423-.L424
	.text
.L446:
	movl	-28(%rbp), %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L445:
	movl	-28(%rbp), %eax
	movl	%eax, -36(%rbp)
	movq	-200(%rbp), %rax
	movq	(%rax), %rdx
	movq	-200(%rbp), %rax
	movl	20(%rax), %ecx
	movl	-36(%rbp), %eax
	addl	%ecx, %eax
	cltq
	salq	$2, %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L444:
	movl	-28(%rbp), %eax
	movl	%eax, -40(%rbp)
	movq	-200(%rbp), %rax
	movq	(%rax), %rdx
	movq	-200(%rbp), %rax
	movl	20(%rax), %ecx
	movl	-40(%rbp), %eax
	addl	%ecx, %eax
	cltq
	salq	$2, %rax
	leaq	(%rdx,%rax), %rbx
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, (%rbx)
	jmp	.L447
.L443:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -48(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -44(%rbp)
	movl	-44(%rbp), %edx
	movl	-48(%rbp), %eax
	addl	%eax, %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L442:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -56(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -52(%rbp)
	movl	-52(%rbp), %eax
	subl	-56(%rbp), %eax
	movl	%eax, %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L441:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -64(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -60(%rbp)
	movl	-60(%rbp), %eax
	imull	-64(%rbp), %eax
	movl	%eax, %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L440:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -72(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -68(%rbp)
	cmpl	$0, -72(%rbp)
	jne	.L448
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$17, %edx
	movl	$1, %esi
	leaq	.LC28(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L448:
	movl	-68(%rbp), %eax
	cltd
	idivl	-72(%rbp)
	movl	%eax, %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L439:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -80(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -76(%rbp)
	cmpl	$0, -80(%rbp)
	jne	.L449
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$15, %edx
	movl	$1, %esi
	leaq	.LC29(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
.L449:
	movl	-76(%rbp), %eax
	cltd
	idivl	-80(%rbp)
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L438:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -88(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -84(%rbp)
	movl	-84(%rbp), %eax
	cmpl	-88(%rbp), %eax
	sete	%al
	movzbl	%al, %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L437:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -96(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -92(%rbp)
	movl	-92(%rbp), %eax
	cmpl	-96(%rbp), %eax
	setne	%al
	movzbl	%al, %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L436:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -104(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -100(%rbp)
	movl	-100(%rbp), %eax
	cmpl	-104(%rbp), %eax
	setl	%al
	movzbl	%al, %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L435:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -112(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -108(%rbp)
	movl	-108(%rbp), %eax
	cmpl	-112(%rbp), %eax
	setle	%al
	movzbl	%al, %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L434:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -120(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -116(%rbp)
	movl	-116(%rbp), %eax
	cmpl	-120(%rbp), %eax
	setg	%al
	movzbl	%al, %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L433:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -128(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -124(%rbp)
	movl	-124(%rbp), %eax
	cmpl	-128(%rbp), %eax
	setge	%al
	movzbl	%al, %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L432:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -136(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -132(%rbp)
	cmpl	$0, -132(%rbp)
	je	.L450
	cmpl	$0, -136(%rbp)
	je	.L450
	movl	$1, %edx
	jmp	.L451
.L450:
	movl	$0, %edx
.L451:
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L431:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -144(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -140(%rbp)
	cmpl	$0, -140(%rbp)
	jne	.L452
	cmpl	$0, -144(%rbp)
	je	.L453
.L452:
	movl	$1, %edx
	jmp	.L454
.L453:
	movl	$0, %edx
.L454:
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L430:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -148(%rbp)
	cmpl	$0, -148(%rbp)
	sete	%al
	movzbl	%al, %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L429:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -152(%rbp)
	movl	-152(%rbp), %eax
	negl	%eax
	movl	%eax, %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L428:
	movl	-28(%rbp), %edx
	movq	-200(%rbp), %rax
	movl	%edx, 16(%rax)
	jmp	.L447
.L427:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -156(%rbp)
	cmpl	$0, -156(%rbp)
	jne	.L462
	movl	-28(%rbp), %edx
	movq	-200(%rbp), %rax
	movl	%edx, 16(%rax)
	jmp	.L462
.L426:
	movq	-200(%rbp), %rax
	movl	20(%rax), %eax
	movl	%eax, -160(%rbp)
	movq	-200(%rbp), %rax
	movl	8(%rax), %edx
	movq	-200(%rbp), %rax
	movl	%edx, 20(%rax)
	movq	-200(%rbp), %rax
	movl	16(%rax), %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	movl	-160(%rbp), %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	movl	-28(%rbp), %edx
	movq	-200(%rbp), %rax
	movl	%edx, 16(%rax)
	jmp	.L447
.L425:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -172(%rbp)
	movq	-200(%rbp), %rax
	movl	20(%rax), %edx
	movq	-200(%rbp), %rax
	movl	%edx, 8(%rax)
	movq	-200(%rbp), %rax
	movl	8(%rax), %eax
	testl	%eax, %eax
	jg	.L456
	movl	-172(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC30(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L416
.L456:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -168(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -164(%rbp)
	movq	-200(%rbp), %rax
	movl	-168(%rbp), %edx
	movl	%edx, 20(%rax)
	movq	-200(%rbp), %rax
	movl	-164(%rbp), %edx
	movl	%edx, 16(%rax)
	movl	-172(%rbp), %edx
	movq	-200(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	vm_push
	jmp	.L447
.L423:
	movq	-200(%rbp), %rax
	movq	%rax, %rdi
	call	vm_pop
	movl	%eax, -176(%rbp)
	movl	-176(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC31(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L447
.L422:
	movl	-32(%rbp), %edx
	movq	stderr(%rip), %rax
	leaq	.LC32(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L462:
	nop
.L447:
	movq	-200(%rbp), %rax
	movl	16(%rax), %edx
	movq	-208(%rbp), %rax
	movl	8(%rax), %eax
	cmpl	%eax, %edx
	jge	.L421
	movq	-208(%rbp), %rax
	movq	(%rax), %rdx
	movq	-200(%rbp), %rax
	movl	16(%rax), %eax
	cltq
	salq	$3, %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, -24(%rbp)
	movl	-24(%rbp), %eax
	cmpl	$18, %eax
	je	.L421
	movl	-24(%rbp), %eax
	cmpl	$19, %eax
	je	.L421
	movl	-24(%rbp), %eax
	cmpl	$20, %eax
	je	.L421
	movl	-24(%rbp), %eax
	cmpl	$21, %eax
	je	.L421
	movl	-32(%rbp), %eax
	cmpl	$3, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$4, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$5, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$6, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$7, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$8, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$9, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$10, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$11, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$12, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$13, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$14, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$15, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$16, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$17, %eax
	je	.L460
	movl	-32(%rbp), %eax
	cmpl	$20, %eax
	jne	.L421
.L460:
	movq	-200(%rbp), %rax
	movl	8(%rax), %eax
	testl	%eax, %eax
	jle	.L421
	movq	-200(%rbp), %rax
	movl	8(%rax), %eax
	leal	-1(%rax), %edx
	movq	-200(%rbp), %rax
	movl	%edx, 8(%rax)
.L421:
	movq	-200(%rbp), %rax
	movl	16(%rax), %edx
	movq	-208(%rbp), %rax
	movl	8(%rax), %eax
	cmpl	%eax, %edx
	jl	.L461
.L416:
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE57:
	.size	vm_execute, .-vm_execute
	.globl	vm_cleanup
	.type	vm_cleanup, @function
vm_cleanup:
.LFB58:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	free@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE58:
	.size	vm_cleanup, .-vm_cleanup
	.section	.rodata
.LC33:
	.string	"Usage: %s <source_file>\n"
.LC34:
	.string	"r"
.LC35:
	.string	"Could not open file: %s\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB59:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$272, %rsp
	movl	%edi, -260(%rbp)
	movq	%rsi, -272(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	cmpl	$1, -260(%rbp)
	jg	.L465
	movq	-272(%rbp), %rax
	movq	(%rax), %rdx
	movq	stderr(%rip), %rax
	leaq	.LC33(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %eax
	jmp	.L468
.L465:
	movq	-272(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	leaq	.LC34(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -256(%rbp)
	cmpq	$0, -256(%rbp)
	jne	.L467
	movq	-272(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rdx
	movq	stderr(%rip), %rax
	leaq	.LC35(%rip), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %eax
	jmp	.L468
.L467:
	movq	-256(%rbp), %rax
	movl	$2, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	fseek@PLT
	movq	-256(%rbp), %rax
	movq	%rax, %rdi
	call	ftell@PLT
	movq	%rax, -248(%rbp)
	movq	-256(%rbp), %rax
	movl	$0, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	fseek@PLT
	movq	-248(%rbp), %rax
	addq	$1, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, -240(%rbp)
	movq	-248(%rbp), %rdx
	movq	-256(%rbp), %rcx
	movq	-240(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread@PLT
	movq	-248(%rbp), %rdx
	movq	-240(%rbp), %rax
	addq	%rdx, %rax
	movb	$0, (%rax)
	movq	-256(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	movq	-240(%rbp), %rdx
	leaq	-192(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	lexer_init
	movq	-240(%rbp), %rdx
	leaq	-144(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	parser_init
	leaq	-144(%rbp), %rax
	movq	%rax, %rdi
	call	parser_parse_program
	movq	%rax, -232(%rbp)
	movq	-232(%rbp), %rax
	movq	%rax, %rdi
	call	semantic_check_program
	movq	-232(%rbp), %rdx
	leaq	-80(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	codegen_program
	leaq	-224(%rbp), %rax
	movq	%rax, %rdi
	call	vm_init
	leaq	-80(%rbp), %rdx
	leaq	-224(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	vm_execute
	leaq	-224(%rbp), %rax
	movq	%rax, %rdi
	call	vm_cleanup
	movq	-240(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movl	$0, %eax
.L468:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L469
	call	__stack_chk_fail@PLT
.L469:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE59:
	.size	main, .-main
	.section	.rodata
.LC36:
	.string	"if"
.LC37:
	.string	"else"
.LC38:
	.string	"while"
.LC39:
	.string	"for"
.LC40:
	.string	"return"
.LC41:
	.string	"struct"
	.section	.data.rel.ro.local,"aw"
	.align 32
	.type	keywords.0, @object
	.size	keywords.0, 160
keywords.0:
	.quad	.LC36
	.long	0
	.zero	4
	.quad	.LC37
	.long	1
	.zero	4
	.quad	.LC38
	.long	2
	.zero	4
	.quad	.LC39
	.long	3
	.zero	4
	.quad	.LC40
	.long	4
	.zero	4
	.quad	.LC6
	.long	5
	.zero	4
	.quad	.LC7
	.long	6
	.zero	4
	.quad	.LC8
	.long	7
	.zero	4
	.quad	.LC41
	.long	8
	.zero	4
	.quad	0
	.long	0
	.zero	4
	.ident	"GCC: (GNU) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
