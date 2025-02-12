global asm_strdup

; rdi: const char *s
section .text
extern __errno_location
extern asm_strlen
extern asm_strcpy
extern malloc

asm_strdup:
	push	rbp ; function prologue
	mov	rbp, rsp; function prologue

	call	asm_strlen
	inc	rax
	push	rdi ; push rdi to the stack, the const char *s, I'll get it before the loop
	mov	rdi, rax ; place the len of the dup into the rdi for malloc call
	call	malloc wrt ..plt ; rax now has return from malloc
	cmp	rax, 0 ; if malloc errors, I gotta set the errno and return null
	jz	bad_ending
	pop	rsi ; the src parameter for the strcpy
	mov	rdi, rax ; the dst parameter for the strcpy
	call	asm_strcpy ; it places dst in rax, so I just have to return

	mov	rsp, rbp ; function epilogue
	pop	rbp ; function epilogue
	ret
bad_ending:
	call	__errno_location wrt ..plt; takes the errno address value and places in rax
	mov dword [rax], 12 ; set the errno to 12 ENOMEM. dword is because errno is 32 bits
	xor rax, rax ; set rax to 0 (NULL)

	mov	rsp, rbp ; function epilogue
	pop	rbp ; function epilogue
	ret

