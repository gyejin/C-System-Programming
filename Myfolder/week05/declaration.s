	.file	"declaration.c"
	.text
	.globl	global_int
	.bss
	.align 4
	.type	global_int, @object
	.size	global_int, 4
global_int:
	.zero	4
	.globl	global_double
	.align 8
	.type	global_double, @object
	.size	global_double, 8
global_double:
	.zero	8
	.section	.rodata
.LC0:
	.string	"\354\240\225\354\240\201 \353\263\200\354\210\230: %d\n"
	.text
	.globl	check_static
	.type	check_static, @function
check_static:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	static_int.0(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	static_int.0(%rip), %eax
	addl	$1, %eax
	movl	%eax, static_int.0(%rip)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	check_static, .-check_static
	.section	.rodata
.LC1:
	.string	"\354\240\204\354\227\255 \354\240\225\354\210\230: %d\n"
.LC2:
	.string	"\354\240\204\354\227\255 \354\213\244\354\210\230: %f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	global_int(%rip), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	global_double(%rip), %rax
	movq	%rax, %xmm0
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	movl	$0, %eax
	call	check_static
	movl	$0, %eax
	call	check_static
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.local	static_int.0
	.comm	static_int.0,4,4
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
