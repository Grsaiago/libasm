global asm_write
extern __errno_location

section .text
asm_write:
	push	rbp ; function prologue
	mov	rbp, rsp; function prologue

	mov	rax, 1 ; write syscall
	syscall ; the registers already have the correct values loaded
	test	rax, rax
	js	error_path

	mov	rsp, rbp ; function prologue
	pop	rbp ; function prologue
	ret

error_path:
	not	rax ; two's-complement
	inc	rax ; two's-complement, I could go for "not rax" as well
	push	rax ; save the 'errno to be' value on the stack
	call	__errno_location wrt ..plt; takes the errno address value and places in rax
	pop	rcx
	mov	[rax], rcx ; *rax = rcx;
	mov	rax, -1 ; the syscall return value

	mov	rsp, rbp ; function epilogue
	pop	rbp ; function epilogue
	ret
