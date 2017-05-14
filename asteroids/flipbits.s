	PRESERVE8			;
							;
	THUMB				;
	AREA 	|.text|, CODE, READONLY		;
	EXPORT bitflip
	entry
bitflip	FUNCTION
	PUSH {R1, R2, LR}	; push registers onto the stack
	MOV R2, R0			;
	RBIT R1, R2			; reverses the bit order
	LSR R0, R1, #24		; logical right shift by 24
	POP {R1,R2, LR}		; restore previously saved register states
	BX LR				; branch to link register and continue as normal
	ENDFUNC
	END