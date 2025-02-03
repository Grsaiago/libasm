global _start

section .data
	msg db "Hello, World", 0xa
	len equ $ - msg

section .text
_start:
	mov rax, 1 ; syscall write
	mov rdi, 1 ; fd
	lea rsi, [msg] ; buff
	mov rdx, len
	syscall ; call

	; Exit the program
	mov rax, 60      ; syscall number for exit (60)
	xor rdi, rdi     ; exit code (0 = success)
	syscall          ; invoke the syscall
