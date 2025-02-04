global asm_strcpy

; rdi: dest, rsi: src
section .text
asm_strcpy:
	xor	rcx, rcx; i = 0
loop:
	mov	al, byte [rsi + rcx]
	cmp	al, 0
	jz	end
	mov	byte [rdi + rcx], al
	inc	rcx
	jmp	loop
end:
	mov	byte [rdi + rcx], byte 0 ; adiciona o '0' no final
	mov	rax, rdi
	ret
