global _start

section .text
_start:
	mov rax, 60 ; exit syscall
	cmp rax, 60 ; compare rax with the number 60
	jl
	syscall

