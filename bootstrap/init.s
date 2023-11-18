	.file	"init.c"
	.text
	.p2align 4,,15
	.globl	_start
	.type	_start, @function
_start:
.LFB0:
	.cfi_startproc
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
#APP
# 20 "init.c" 1
	mov (%rsp), %ebx
lea 8(%rsp), %rbp
# 0 "" 2
#NO_APP
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	mem_init@PLT
	cmpl	$-1, %eax
	je	.L5
	movslq	%ebx, %rax
	movl	%ebx, %edi
	movq	%rbp, %rsi
	leaq	8(%rbp,%rax,8), %rdx
	call	main@PLT
	movl	%eax, %edi
	call	_exit@PLT
.L5:
	movl	$255, %edi
	call	_exit@PLT
	.cfi_endproc
.LFE0:
	.size	_start, .-_start
	.ident	"GCC: (Debian 6.3.0-18+deb9u1) 6.3.0 20170516"
	.section	.note.GNU-stack,"",@progbits
