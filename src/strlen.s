global asm_strlen

section .text
asm_strlen:
	push	rbp ; function prologue
	mov	rbp, rsp; function prologue

	xor	rcx, rcx; i = 0
; while
count_loop:
	cmp	byte [rdi + rcx], 0 ; byte faz olhar só os primeiros 8 bits e o [] pra '*': *(rdi + rcx) == ?
	jz	end ; pula pro final caso seja true
	inc	rcx; poderia ser add rcx, 1. É meu i++
	jmp	count_loop ; vol
; end while
end:
	mov	rax, rcx; rax tem que ter o valor de retorno da função

	mov	rsp, rbp ; function epilogue
	pop	rbp ; function epilogue
	ret
