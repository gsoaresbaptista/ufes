segment code
..start:
    ;start DS and SS segment registers and SP stack pointer
    mov ax, data
    mov ds, ax
    mov ax, stack
    mov ss, ax
    mov sp, stacktop

    ; char2bin function test
    mov bx, number
    call char2bin

    ; program end
    mov ah, 4ch
    int 21h

; functions
char2bin:
    mov ah, 00h
    mov al, [bx + 0]
    sub al, 30h
    mov cx, 10
    mul cx
    add al, [bx + 1]
    sub al, 30h
    mul cx
    add al, [bx + 2]
    sub al, 30h
    ret

; other segments
segment data
    number db 31h, 32h ,35h
segment stack stack
    resb 256
stacktop:
