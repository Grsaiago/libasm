global asm_strcmp

section .text
asm_strcmp:
	push	rbp ; function prologue
	mov	rbp, rsp; function prologue

	xor	rcx, rcx; i = 0
loop:
	mov	al, byte [rdi + rcx] ; calcula e carrega o que contém nesse endereço no rax
	mov	dl, byte [rsi + rcx] ; calcula e carrega o que contém nesse endereço no rdx
	cmp	al, dl; compara as duas
	jne	bad_ending ; se não for 0 a comparação, pula pro bad ending
	cmp	al, 0 ; se as duas chegaram no final. sabendo que são iguais, se uma for 0 a outra tbm é.
	jz	good_ending ; pulo igual ao descrito acima
	inc	rcx
	jmp	loop
good_ending:
	mov	rsp, rbp ; function epilogue
	pop	rbp ; function epilogue

	mov	rax, 0;
	ret
bad_ending:
	movzx	rax, al
	movzx	rdx, dl
	sub	rax, rdx ; the values are already in those registers because of the mov

	mov	rsp, rbp ; function epilogue
	pop	rbp ; function epilogue
	ret
