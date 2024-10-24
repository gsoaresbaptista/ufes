segment code
..start:
    ;start DS and SS segment registers and SP stack pointer
    mov ax, data
    mov ds, ax
    mov ax, stack
    mov ss, ax
    mov sp, stacktop

    ; char2bin function test
    mov bx, numbers
    call vec2bin

    ; program end
    mov ah, 4ch
    int 21h

; functions
vec2bin:
    ; ax -> final value
    ; bx -> memory address
    ; cx -> 10d to multiply ax
    ; dx -> current value read
    mov ax, 0000h
    mov cx, 000Ah
    mov dx, 0000h
    jmp next_char
    reset_number:
        add bx, 1
        mov ax, 0000h
    next_char:
        mov dl, [bx]
        cmp dl, ' '
        ; do something here
        je reset_number
        mul cl
    start_conversion:
        add al, dl
        add bx, 1
        sub ax, '0'
        jmp next_char
    ret

; other segments
segment data
    numbers db '103 104 99 105 '
segment stack stack
    resb 256
stacktop:
