segment code
..start:
    mov ax, data
    mov ds, ax
    mov ax, stack
    mov ss, ax
    mov sp, stacktop

    ; program keyboard interrupt
    cli
    xor ax, ax
    mov es, ax
    mov ax, [es:9h*4] ; load ax with previous offset
    mov [previous_dos_address], ax ; save addr. for int 9h previously pointed
    mov ax, [es:9h*4 + 2] ; save addr. previous of cs
    mov [previous_dos_segment], ax
    mov [es:9h*4+2], cs
    mov word[es:9h*4], keyboard_interrupt
    sti

    ; save last video mode
    mov ah, 0Fh
    int 10h
    mov [previous_video_mode], al

    ; change to graphic mode
    mov al, 12h
    mov ah, 0
    int 10h

    call draw_hud
    call draw_texts
    call draw_racket

    call draw_instructions

    wait_start:
        ; check exit condition
        cmp byte[exit_interrupt_flag], 1
        je exit
        ; check start condition
        cmp byte[game_start], 0
        je wait_start

    call clean_instructions

    main_loop:
        ; check exit condition
        cmp byte[exit_interrupt_flag], 1
        je exit
        ; draw ball
        call clean_ball
        call move_ball
        call draw_ball
        ; compute points
        call check_computer_point
        call check_player_point
        ; return to init
        call draw_racket
        call draw_speed
        call wait_speed
        jmp main_loop

exit:
    ; return previous video mode
    xor ax, ax
    mov al, [previous_video_mode]
    int 10h
    ; return previous keyboard dos
    cli
    xor ax, ax
    mov es, ax
    mov ax, [previous_dos_address]
    mov [es:9h*4], ax
    mov ax, [previous_dos_segment]
    mov [es:9h*4+2], ax
    ; exit call
    mov ah, 4Ch
    int 21h

draw_instructions:
    ; title
    mov byte[color], green
    mov cx, 10
    mov dh, 9
    mov dl, 10
    mov di, instructions_title
    call draw_instruction_text
    mov byte[color], grey
    ; line 1
    mov cx, 58
    mov dh, 11
    mov dl, 10
    mov di, instructions_1
    call draw_instruction_text
    ; line 2
    mov cx, 55
    mov dh, 12
    mov dl, 10
    mov di, instructions_2
    call draw_instruction_text
    ; line 3
    mov cx, 27
    mov dh, 13
    mov dl, 10
    mov di, instructions_3
    call draw_instruction_text
    ; line 3c - U
    mov byte[color], red
    mov cx, 1
    mov dh, 15
    mov dl, 10
    mov di, instructions_c
    call draw_instruction_text
    ; line c - middle text
    mov byte[color], grey
    mov cx, 13
    mov dh, 15
    mov dl, 11
    mov di, instructions_c + 1
    call draw_instruction_text
    ; line c - D
    mov byte[color], red
    mov cx, 2
    mov dh, 15
    mov dl, 24
    mov di, instructions_c + 14
    call draw_instruction_text
    ; line c - rest text
    mov byte[color], grey
    mov cx, 13
    mov dh, 15
    mov dl, 26
    mov di, instructions_c + 16
    call draw_instruction_text

    ; line s - +
    mov byte[color], red
    mov cx, 1
    mov dh, 16
    mov dl, 10
    mov di, instructions_s
    call draw_instruction_text
    ; line s - middle text
    mov byte[color], grey
    mov cx, 30
    mov dh, 16
    mov dl, 11
    mov di, instructions_s + 1
    call draw_instruction_text
    ; line s - -
    mov byte[color], red
    mov cx, 1
    mov dh, 16
    mov dl, 41
    mov di, instructions_s + 31
    call draw_instruction_text
    ; line s - rest text
    mov byte[color], grey
    mov cx, 15
    mov dh, 16
    mov dl, 42
    mov di, instructions_s + 32
    call draw_instruction_text

    ; line 4 - ENTER
    mov byte[color], red
    mov cx, 5
    mov dh, 17
    mov dl, 10
    mov di, instructions_4
    call draw_instruction_text
    ; line 4 - middle text
    mov byte[color], grey
    mov cx, 16
    mov dh, 17
    mov dl, 15
    mov di, instructions_4 + 5
    call draw_instruction_text
    ; line 4 - ESC
    mov byte[color], red
    mov cx, 3
    mov dh, 17
    mov dl, 31
    mov di, instructions_4 + 21
    call draw_instruction_text
    ; line 4 - rest text
    mov byte[color], grey
    mov cx, 11
    mov dh, 17
    mov dl, 34
    mov di, instructions_4 + 24
    call draw_instruction_text
    ret

clean_instructions:
    ; title
    mov byte[color], black
    mov cx, 10
    mov dh, 9
    mov dl, 10
    mov di, instructions_title
    call draw_instruction_text
    ; line 1
    mov cx, 58
    mov dh, 11
    mov dl, 10
    mov di, instructions_1
    call draw_instruction_text
    ; line 2
    mov cx, 55
    mov dh, 12
    mov dl, 10
    mov di, instructions_2
    call draw_instruction_text
    ; line 3
    mov cx, 27
    mov dh, 13
    mov dl, 10
    mov di, instructions_3
    call draw_instruction_text
    ; line c
    mov cx, 29
    mov dh, 15
    mov dl, 10
    mov di, instructions_c
    call draw_instruction_text
    ; line s
    mov cx, 47
    mov dh, 16
    mov dl, 10
    mov di, instructions_s
    call draw_instruction_text
    ; line 4
    mov cx, 35
    mov dh, 17
    mov dl, 10
    mov di, instructions_4
    call draw_instruction_text
    ret

draw_instruction_text:
    ; mov cx, 59 ; characters
    mov bx, 0
    ; mov dh, 11 ; line (0-29)
    ; mov dl, 10 ;column (0-79)

    draw_instruction_text_loop:
        call cursor
        mov al, [bx + di]
        call caracter
        inc bx ; next character
        inc dl ; next column
        loop draw_instruction_text_loop
    ret

check_player_point:
    cmp word[ball_dx], 0
    jl check_player_point_end
    cmp word[ball_x], 595
    jne check_player_point_end

    ; check y
    mov ax, word[player_y]
    mov bx, ax
    add ax, 61 ; upper part
    sub bx, 11 ; bottom part

    cmp word[ball_y], ax
    jg check_player_point_end
    cmp word[ball_y], bx
    jl check_player_point_end

    ; increase point
        cmp byte[player_points], 9
        jne normal_increase_player
        cmp byte[player_points + 1], 9
        jne not_reset_player_points
        mov byte[player_points], 1
        mov byte[player_points + 1], 0
        call draw_player_points
        jmp collision
    not_reset_player_points:
        mov byte[player_points], 0
        inc byte[player_points + 1]
        call draw_player_points
        jmp collision
    normal_increase_player:
        inc byte[player_points]
        call draw_player_points

    collision:
        xor word[ball_dx], 0xFFFFFFFF
        add word[ball_dx], 1

    check_player_point_end:
    ret

clean_racket:
    mov word[color], black
    mov ax, word[player_y]
    mov bx, ax
    add bx, 53
    sub ax, 3
    ; line 1
    push word 600
    push ax
    push word 600
    push bx
    call line
    ; line 2
    push word 601
    push ax
    push word 601
    push bx
    call line
    ; line 3
    push word 602
    push ax
    push word 602
    push bx
    call line
    ret

draw_racket:
    mov word[color], intense_white
    mov ax, word[player_y]
    mov bx, ax
    add bx, 50
    ; line 1
    push word 600
    push ax
    push word 600
    push bx
    call line
    ; line 2
    push word 601
    push ax
    push word 601
    push bx
    call line
    ; line 3
    push word 602
    push ax
    push word 602
    push bx
    call line
    ret

check_computer_point:
    cmp word[ball_x], 634
    jl check_computer_point_end
    ; increase point
        cmp byte[computer_points], 9
        jne normal_increase
        cmp byte[computer_points + 1], 9
        jne not_reset_computer_points
        mov byte[computer_points], 1
        mov byte[computer_points + 1], 0
        call draw_computer_points
        jmp check_computer_point_end
    not_reset_computer_points:
        mov byte[computer_points], 0
        inc byte[computer_points + 1]
        call draw_computer_points
        jmp check_computer_point_end
    normal_increase:
        inc byte[computer_points]
        call draw_computer_points
    check_computer_point_end:
    ret

move_ball:
    ; move x axis
    mov ax, word[ball_x]
    cmp ax, 5
    jle invert_x_direction
    cmp ax, 634
    jge invert_x_direction
    jmp move_y_axis

    invert_x_direction:
        xor word[ball_dx], 0xFFFFFFFF
        add word[ball_dx], 1

    move_y_axis:
        mov ax, word[ball_y]
        cmp ax, 6
        jle invert_y_direction
        cmp ax, 393
        jge invert_y_direction
        jmp move_ball_end

        invert_y_direction:
            xor word[ball_dy], 0xFFFFFFFF
            add word[ball_dy], 1

    move_ball_end:
        ; increase x axis position
        mov ax, word[ball_dx]
        add word[ball_x], ax
        mov ax, word[ball_dy]
        add word[ball_y], ax
    ret

draw_ball:
    mov byte[color], red
    push word[ball_x]
    push word[ball_y]
    push word 5
    call full_circle
    ret

clean_ball:
    mov byte[color], black
    push word[ball_x]
    push word[ball_y]
    push word 5
    call full_circle
    ret

wait_speed:
    xor bx, bx
    xor cx, cx
    xor ax, ax
    mov bl, [speed_index]
    mov dx, [speed + bx]
    mov ah, 86h
    int 15h
    ret

; new keyboard interrupt
keyboard_interrupt:
    push ax
    push bx
    push ds
    mov ax, data
    mov ds, ax
    in al, 60h
    inc word[p_i]
    and word[p_i], 7
    mov bx, [p_i]
    mov [bx + key], al
    in al, 61h
    or al, 80h
    out 61h, al
    and al, 7Fh
    out 61h, al
    ; convert key
    mov ax, word[p_i]
    cmp ax, word[p_t]
    jne check_is_required
    jmp keyboard_interrupt_exit

    check_is_required:
        inc word[p_t]
        and word[p_t], 7
        mov bx, word[p_t]
        xor ax, ax
        mov al, byte[bx + key]
        mov byte[key_u], al

    ; check keys
    check_keys_pressed:
        cmp byte[key_u], 1ch
        jne check_key_esc
        mov byte[game_start], 1

        check_key_esc:
            cmp byte[key_u], 81h
            jne check_free_key
            mov byte[exit_interrupt_flag], 1

        check_free_key:
            cmp byte[game_start], 0
            je keyboard_interrupt_exit

        check_key_u:
            cmp byte[key_u], 16h
            jne check_key_d
            jmp move_up

        check_key_d:
            cmp byte[key_u], 20h
            jne check_speed_key
            jmp move_down

        jmp check_speed_key

    ; interrupt exit
    keyboard_interrupt_exit:
        mov al, 20h
        out 20h, al
        pop ds
        pop bx
        pop ax
    iret

check_speed_key:
    cmp byte[key_u], 0Dh ; +
    jne game_speed_decrease_check
    ; check max speed
    cmp byte[speed_index], 8
    jge game_speed_end
    ; increase speed
    add byte[speed_index], 2
    jmp game_speed_end

    ; check_decrease_speed:
    game_speed_decrease_check:
        cmp BYTE [key_u], 0Ch ; -
        jne game_speed_end
        ; check minimum speed
        cmp byte[speed_index], 0
        jle game_speed_end
        ; decrease speed
        sub byte[speed_index], 2
        jmp game_speed_end

    game_speed_end:
        jmp keyboard_interrupt_exit ; return to keyboard interrupt

move_up:
    call clean_racket
    cmp word[player_y], 340
    jge move_up_end
    add word[player_y], 10
    move_up_end:
        jmp check_key_d ; return to keyboard interrupt

move_down:
    call clean_racket
    cmp word[player_y], 11
    jle move_down_end
    sub word[player_y], 10
    move_down_end:
        jmp check_speed_key ; return to keyboard interrupt

draw_texts:
    call draw_subject
    call draw_game_info_labels
    call draw_player_points
    call draw_computer_points
    call draw_speed
    ret

draw_speed:
    mov dh, 3 ; line (0-29)
    mov dl, 73 ;column (0-79)
    mov byte[color], intense_white
    call cursor
    mov al, [speed_index]
    cbw
    mov bl, 2
    div bl
    add al, '1'
    call caracter
    ret

draw_computer_points:
    mov cx, 2 ; characters
    mov bx, 1
    mov dh, 3 ; line (0-29)
    mov dl, 31 ;column (0-79)
    mov byte[color], intense_white

    draw_computer_points_loop:
        call cursor
        mov al, [bx + computer_points]
        add al, '0'
        call caracter
        dec bx ; next character
        inc dl ; next column
        loop draw_computer_points_loop
    ret

draw_player_points:
    mov cx, 2 ; characters
    mov bx, 1
    mov dh, 3 ; line (0-29)
    mov dl, 26 ;column (0-79)
    mov byte[color], intense_white

    draw_player_points_loop:
        call cursor
        mov al, [bx + player_points]
        add al, '0'
        call caracter
        dec bx ; next character
        inc dl ; next column
        loop draw_player_points_loop
    ret

draw_hud:
    ; top-line
    push word 0
    push word 479
    push word 639
    push word 479
    call line
    ; top-bottom line
    push word 0
    push word 399
    push word 639
    push word 399
    call line
    ; let line
    push word 0
    push word 0
    push word 0
    push word 480
    call line
    ; right line
    push word 639
    push word 0
    push word 639
    push word 480
    call line
    ; bottom line
    push word 0
    push word 0
    push word 639
    push word 0
    call line
    ret

draw_subject:
    mov cx, 58 ; characters
    mov bx, 0
    mov dh, 1 ; line (0-29)
    mov dl, 11 ;column (0-79)
    mov byte[color], intense_white

    open_text_loop:
        call cursor
        mov al, [bx + subject]
        call caracter
        inc bx ; next character
        inc dl ; next column
        loop open_text_loop
    ret

draw_game_info_labels:
    mov cx, 23 ; characters
    mov bx, 0
    mov dh, 3 ; line (0-29)
    mov dl, 2 ;column (0-79)
    mov byte[color], intense_white

    student_text_loop:
        call cursor
        mov al, [bx + student]
        call caracter
        inc bx ; next character
        inc dl ; next column
        loop student_text_loop

    ; draw x
    mov bx, 0
    mov dl, 29 ;column (0-79)
    call cursor
    mov al,'x'
    call caracter

    mov cx, 10 ; characters
    mov bx, 0
    mov dl, 34 ;column (0-79)

    computer_label_loop:
        call cursor
        mov al, [bx + computer]
        call caracter
        inc bx ; next character
        inc dl ; next column
        loop computer_label_loop
    
    ; speed label
    mov cx, 17 ; characters
    mov bx, 0
    mov dl, 55 ;column (0-79)

    speed_label_text_loop:
        call cursor
        mov al, [bx + speed_label]
        call caracter
        inc bx ; next character
        inc dl ; next column
        loop speed_label_text_loop
    ret

; dh = row (0-29) e  dl=column  (0-79)
cursor:
    pushf
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push bp
    mov ah, 2
    mov bh, 0
    int 10h
    pop bp
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    popf
    ret

; draw character in cursor position
; al= character
; color in color variable
caracter:
    pushf
    push ax
    push bx
    push cx
    mov ah, 9
    mov bh, 0
    mov cx, 1
    mov bl, [color]
    int 10h
    pop cx
    pop bx
    pop ax
    popf
    ret

;   plot_xy
; push x; push y; call plot_xy;  (x<639, y<479)
; color in color variable
plot_xy:
    push bp
    mov bp, sp
    pushf
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    mov ah, 0ch
    mov al, [color]
    mov bh, 0
    mov dx, 479
    sub dx, [bp+4]
    mov cx, [bp+6]
    int 10h
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    popf
    pop bp
    ret 4

; draw a line
; push x, push y
line:
    push bp
    mov bp, sp
    pushf
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    mov ax, [bp+10]
    mov bx, [bp+8]
    mov cx, [bp+6]
    mov dx, [bp+4]
    cmp ax, cx
    je line2
    jb line1
    xchg ax, cx
    xchg bx, dx
    jmp line1
line2:
    cmp bx, dx
    jb line3
    xchg bx, dx
line3:
    push ax
    push bx
    call plot_xy
    cmp bx, dx
    jne line31
    jmp fim_line
line31: inc bx
    jmp line3
line1:
    push cx
    sub cx, ax
    mov [delta_x], cx
    pop cx
    push dx
    sub dx, bx
    ja line32
    neg dx
line32: 
    mov [delta_y], dx
    pop dx
    push ax
    mov ax, [delta_x]
    cmp ax, [delta_y]
    pop ax
    jb line5
    push cx
    sub cx, ax
    mov [delta_x], cx
    pop cx
    push dx
    sub dx, bx
    mov [delta_y], dx
    pop dx
    mov si, ax
line4:
    push ax
    push dx
    push si
    sub si, ax
    mov ax, [delta_y]
    imul si
    mov si, [delta_x]
    shr si, 1
    cmp dx, 0
    jl ar1
    add ax, si
    adc dx, 0
    jmp arc1
ar1: sub ax, si
    sbb dx, 0
arc1:
    idiv word [delta_x]
    add ax, bx
    pop si
    push si
    push ax
    call plot_xy
    pop dx
    pop ax
    cmp si, cx
    je fim_line
    inc si
    jmp line4
line5: cmp bx, dx
    jb line7
    xchg ax, cx
    xchg bx, dx
line7:
    push cx
    sub cx, ax
    mov [delta_x], cx
    pop cx
    push dx
    sub dx, bx
    mov [delta_y], dx
    pop dx
    mov si, bx
line6:
    push dx
    push si
    push ax
    sub si, bx
    mov ax, [delta_x]
    imul si
    mov si, [delta_y]
    shr si, 1
    cmp dx, 0
    jl ar2
    add ax, si
    adc dx, 0
    jmp arc2
ar2: sub ax, si
    sbb dx, 0
arc2:
    idiv word [delta_y]
    mov di, ax
    pop ax
    add di, ax
    pop si
    push di
    push si
    call plot_xy
    pop dx
    cmp si, dx
    je fim_line
    inc si
    jmp line6
fim_line:
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    popf
    pop bp
    ret 8

; full_circle
; push xc; push yc; push r; call full_circle;
; (xc+r < 639, yc+r < 479) and (xc-r > 0, yc-r > 0)
full_circle:
    push bp
    mov bp,sp
    pushf
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    mov ax,[bp+8]
    mov bx,[bp+6]
    mov cx,[bp+4]
    mov si,bx
    sub si,cx
    push ax
    push si
    mov si,bx
    add si,cx
    push ax
    push si
    call line
    mov di,cx
    sub di,1
    mov dx,0
    stay_full:
        mov si,di
        cmp si,0
        jg  inf_full
        mov si,dx
        sal si,1
        add si,3
        add di,si
        inc dx
        jmp plotar_full
    inf_full:
        mov si,dx
        sub si,cx
        sal si,1
        add si,5
        add di,si
        inc dx
        dec cx

    plotar_full:
        mov si,ax
        add si,cx
        push si
        mov si,bx
        sub si,dx
        push si
        mov si,ax
        add si,cx
        push si
        mov si,bx
        add si,dx
        push si
        call line
        mov si,ax
        add si,dx
        push si
        mov si,bx
        sub si,cx
        push si
        mov si,ax
        add si,dx
        push si
        mov si,bx
        add si,cx
        push si
        call line
        mov si,ax
        sub si,dx
        push si
        mov si,bx
        sub si,cx
        push si
        mov si,ax
        sub si,dx
        push si
        mov si,bx
        add si,cx
        push si
        call line
        mov si,ax
        sub si,cx
        push si
        mov si,bx
        sub si,dx
        push si
        mov si,ax
        sub si,cx
        push si
        mov si,bx
        add si,dx
        push si
        call line
        cmp cx,dx
        jb  fim_full_circle
        jmp stay_full
    fim_full_circle:
        pop di
        pop si
        pop dx
        pop cx
        pop bx
        pop ax
        popf
        pop bp
    ret 6

segment data
    ; video
    delta_x dw 0
    delta_y dw 0
    color db intense_white ; initial_color

    ; keyboard
    key_u db 0
    key resb 8
    p_i dw 0 ; key press
    p_t dw 0 ; key up
    exit_interrupt_flag db 0

    ; colors
    black equ 0
    blue equ 1
    green equ 2
    cyan equ 3
    red equ 4
    magenta equ 5
    brown equ 6
    white equ 7
    grey equ 8
    light_blue equ 9
    light_green equ 10
    light_cyan equ 11
    pink equ 12
    light_magenta equ 13
    yellow equ 14
    intense_white equ 15

    ; initial state
    previous_video_mode db 0
    previous_dos_address dw 0
    previous_dos_segment dw 0

    ; hud texts
    speed_label db 'Velocidade atual:'
    student db 'Gabriel Soares Baptista'
    computer db 'Computador'
    subject db 'Exercicio de Programacao de Sistemas Embarcados 1 - 2022/2'

    ; points
    player_points db 0, 0
    computer_points db 0, 0

    ; speed variables
    speed_index db 0
    speed dw 6000, 4000, 2800, 1400, 500

    ; ball variables
    ball_x dw 319
    ball_y dw 210
    ball_dx dw -1
    ball_dy dw 1

    ; player variables
    player_y dw 214

    ; game instructions
    game_start db 0
    instructions_title db 'Como jogar'
    instructions_1 db 'A colisao com a bola so acontece enquanto sua movimentacao'
    instructions_2 db 'for para direita, portanto, nao existe colisao enquanto'
    instructions_3 db 'ela se mover para esquerda.'
    instructions_s db '+ para aumentar a velocidade e - para diminuir,'
    instructions_c db 'U para subir e D para descer,'
    instructions_4 db 'ENTER para comecar e ESC para sair!'
segment stack stack
    resb 256
stacktop:
