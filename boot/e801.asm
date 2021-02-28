[bits 16]
do_e801:
    XOR ECX, ECX
    XOR EDX, EDX
    MOV AX, 0xE801
    INT 0x15        ; request upper memory size
    JC SHORT .ERR
    CMP AH, 0x86        ; unsupported function
    JE SHORT .ERR
    CMP AH, 0x80        ; invalid command
    JE SHORT .ERR
    JCXZ .USEAX        ; was the CX result invalid?
 
    MOV AX, CX
    MOV BX, DX
    
    
    mov [0x8000], DX; copy to 0x8000
    ret;
.USEAX:
    ; AX = number of contiguous Kb, 1M to 16M
    ; BX = contiguous 64Kb pages above 16M
    mov [0x8000], BX; copy to 0x8000
    ret;
.ERR:
    mov ah, 0x0e ; tty mode
    mov al, 'H'
    int 0x10
    hlt;
    ret;