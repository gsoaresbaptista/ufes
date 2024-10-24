; Trabalho de Sistemas Embarcados - 2022/01, UFES
; Gabriel Soares Baptista - 2018102117, turma 5.1
segment code
..start:
    mov ax, data
    mov ds, ax
    mov ax, stack
    mov ss, ax
    mov ax, extra
    mov es, ax
    mov sp, stacktop

    ; save previous video mode
    mov ah, 0Fh
    int 10h
    mov [previous_mode], al

    ; change video mode to 640x480, 16 colors
    mov al, 12h
    mov ah, 0
    int 10h

    ; draw interface
    call draw_interface

    ; enable mouse
    mov ax, 1
    int 33h

main_loop:
    ; Get mouse information
    mov ax, 05h
    mov bx, 00h
    int 33h

    ; check left button
    cmp word ax, 01h
    jne main_loop

    ; check clickable area
    cmp cx, 20
    jb main_loop
    cmp dx, 5
    jb main_loop
    cmp dx, 90
    ja main_loop

    ; clean interface to draw new yellow button
    call draw_texts
    mov byte[color], yellow

    check_open_button:
        cmp word cx, 90
        ja check_close_button
        call draw_open_text
        call draw_reading_text
        call read_and_plot_image
        jmp main_loop

    check_close_button:
        cmp word cx, 170
        ja check_low_pass_button
        call close_program

    check_low_pass_button:
        cmp word cx, 320
        ja check_high_pass_button
        call draw_low_pass_text
        call set_low_pass_mask
        mov word [current_image_index], 303
        mov byte [not_mean_flag], 0

        mov word si, update_image_head_crop
        mov word di, convolve_mask
        push word 321
        push word 89
        push word 149
        call convolve

        mov word [current_image_index], 303
        mov word si, update_image_tail_crop
        mov word di, convolve_mask
        push word 321
        push word 239
        push word 150
        call convolve

        jmp main_loop

    check_high_pass_button:
        cmp word cx, 470
        ja check_gradient_button
        call draw_high_pass_text

        ;
        call set_high_boost_mask
        mov word [current_image_index], 303
        mov byte [not_mean_flag], 1
        mov word si, update_image_head_crop
        mov word di, convolve_mask
        push word 321
        push word 89
        push word 151
        call convolve

        mov word [current_image_index], 303
        mov word si, update_image_tail_crop
        mov word di, convolve_mask
        push word 321
        push word 240
        push word 149
        call convolve
        jmp main_loop

    check_gradient_button:
        cmp word cx, 620
        call draw_gradient_text

        ;
        call set_gradient_horizontal_mask
        call set_gradient_vertical_mask
        mov word [current_image_index], 303
        mov byte [not_mean_flag], 1
        mov word si, update_image_head_crop
        mov word di, convolve_double_mask
        push word 321
        push word 89
        push word 151
        call convolve

        mov word [current_image_index], 303
        mov word si, update_image_tail_crop
        mov word di, convolve_double_mask
        push word 321
        push word 240
        push word 149
        call convolve

        jmp main_loop

; Functions

fill_image_head_last_line:
    push bp
    mov bp, sp
    sub sp, 2
    mov word [bp - 2], 0 ; column
    mov bx, 0

    fill_image_head_last_line_loop:
        mov byte al, [es:image_tail + bx]
        mov byte [image_head + 45602 + bx], al
        inc word bx

        ; check column limit
        inc word [bp - 2]
        cmp word [bp - 2], 604
        jl fill_image_head_last_line_loop

    fill_image_head_last_line_end:
        add sp, 2
        pop bp
        ret

;
convolve:
    push bp
    mov bp, sp
    sub sp, 6
    mov word [bp - 2], 0 ; convolve index
    mov word [bp - 4], 0 ; column
    mov word [bp - 6], 0 ; row

    convolve_loop:
        call si
        call di

        ; check if has mean or not
        cmp byte [not_mean_flag], 1
        je not_mean
        mov word dx, 0
        mov word bx, 9
        div bx
        not_mean:
            call check_result
            mov byte bl, 16
            div bl

        ; plot pixel
        mov [color], al
        mov word bx, [bp - 4]
        add word bx, [bp + 8]
        push word bx
        mov word bx, [bp - 6]
        add word bx, [bp + 6]
        push word bx
        call plot_xy

        ; check column limit
        inc word [bp - 4]
        cmp word [bp - 4], 300
        jl convolve_loop

        ; check row limit
        inc word [bp - 6]
        mov word bx, [bp + 4]
        cmp word [bp - 6], bx
        jg convolve_end
        mov word [bp - 4], 0 ; columns = 0
        add word [current_image_index], 2
        jmp convolve_loop

    convolve_end:
        add sp, 6
        pop bp
        ret 4

;
convolve_mask:
    xor word bx, bx
    push bp
    mov bp, sp
    sub sp, 2
    mov word [bp - 2], 0

    ;
    convolve_mask_loop:
        ; multiply
        mov word ax, [image_crop + bx]
        mov word cx, [mask + bx]
        imul cx
        add word [bp - 2], ax
        ; check end
        add bx, 2
        cmp bx, 16
        jle convolve_mask_loop

    convolve_mask_end:
        inc word [current_image_index]
        mov word ax, [bp - 2]
        add sp, 2
        pop bp
        ret

;
convolve_double_mask:
    xor word bx, bx
    push bp
    mov bp, sp
    sub sp, 4
    mov word [bp - 2], 0
    mov word [bp - 4], 0

    ;
    convolve_mask_loop_1:
        ; multiply
        mov word ax, [image_crop + bx]
        mov word cx, [mask + bx]
        imul cx
        add word [bp - 2], ax
        ; check end
        add bx, 2
        cmp bx, 16
        jle convolve_mask_loop_1

    ;
    cmp word [bp - 2], 0
    jge convolve_mask_loop_2_pre
    mov word ax, [bp - 2]
    xor dx, dx
    mov word bx, -1
    imul bx
    mov [bp - 2], ax

    ;
    convolve_mask_loop_2_pre:
    xor word bx, bx

    convolve_mask_loop_2:
        ; multiply
        mov word ax, [image_crop + bx]
        mov word cx, [mask2 + bx]
        imul cx
        add word [bp - 4], ax
        ; check end
        add bx, 2
        cmp bx, 16
        jle convolve_mask_loop_2

    ;
    cmp word [bp - 4], 0
    jge convolve_double_mask_end
    mov word ax, [bp - 4]
    mov word bx, -1
    xor dx, dx
    imul bx
    mov [bp - 4], ax

    convolve_double_mask_end:
        inc word [current_image_index]
        mov word ax, [bp - 2]
        add word ax, [bp - 4]
        add sp, 4
        pop bp
        ret

;
check_result:
    cmp ax, 0
    jge check_result_positive
    mov ax, 0
    check_result_positive:
        cmp ax, 255
        jle check_result_unsaturated
        mov ax, 255
    check_result_unsaturated:
    ret

;
set_low_pass_mask:
    ; row 0
    mov word [mask + 0], 1
    mov word [mask + 2], 1
    mov word [mask + 4], 1
    ; row 1
    mov word [mask + 6], 1
    mov word [mask + 8], 1
    mov word [mask + 10], 1
    ; row 2
    mov word [mask + 12], 1
    mov word [mask + 14], 1
    mov word [mask + 16], 1
    ret

set_gradient_horizontal_mask:
    ; row 0
    mov word [mask + 0], -1
    mov word [mask + 2], -2
    mov word [mask + 4], -1
    ; row 1
    mov word [mask + 6], 0
    mov word [mask + 8], 0
    mov word [mask + 10], 0
    ; row 2
    mov word [mask + 12], 1
    mov word [mask + 14], 2
    mov word [mask + 16], 1
    ret

set_gradient_vertical_mask:
    ; row 0
    mov word [mask2 + 0], -1
    mov word [mask2 + 2], 0
    mov word [mask2 + 4], 1
    ; row 1
    mov word [mask2 + 6], -2
    mov word [mask2 + 8], 0
    mov word [mask2 + 10], 2
    ; row 2
    mov word [mask2 + 12], -1
    mov word [mask2 + 14], 0
    mov word [mask2 + 16], 1
    ret

set_high_boost_mask:
    ; row 0
    mov word [mask + 0], -1
    mov word [mask + 2], -1
    mov word [mask + 4], -1
    ; row 1
    mov word [mask + 6], -1
    mov word [mask + 8],  9
    mov word [mask + 10], -1
    ; row 2
    mov word [mask + 12], -1
    mov word [mask + 14], -1
    mov word [mask + 16], -1
    ret

update_image_head_crop:
    mov bx, [current_image_index]
    ; row 0
    mov byte dh, 0
    mov byte dl, [ds:image_head + bx - 303]
    mov word [image_crop + 0], dx
    mov byte dl, [ds:image_head + bx - 302]
    mov word [image_crop + 2], dx
    mov byte dl, [ds:image_head + bx - 301]
    mov word [image_crop + 4], dx
    ; row 1
    mov byte dl, [ds:image_head + bx - 1]
    mov word [image_crop + 6], dx
    mov byte dl, [ds:image_head + bx]
    mov word [image_crop + 8], dx
    mov byte dl, [ds:image_head + bx + 1]
    mov word [image_crop + 10], dx
    ; row 2
    mov byte dl, [ds:image_head + bx + 301]
    mov word [image_crop + 12], dx
    mov byte dl, [ds:image_head + bx + 302]
    mov word [image_crop + 14], dx
    mov byte dl, [ds:image_head + bx + 303]
    mov word [image_crop + 16], dx
    ret

update_image_tail_crop:
    mov bx, [current_image_index]
    ; row 0
    mov byte dh, 0
    mov byte dl, [es:image_tail + bx - 303]
    mov word [image_crop + 0], dx
    mov byte dl, [es:image_tail + bx - 302]
    mov word [image_crop + 2], dx
    mov byte dl, [es:image_tail + bx - 301]
    mov word [image_crop + 4], dx
    ; row 1
    mov byte dl, [es:image_tail + bx - 1]
    mov word [image_crop + 6], dx
    mov byte dl, [es:image_tail + bx]
    mov word [image_crop + 8], dx
    mov byte dl, [es:image_tail + bx + 1]
    mov word [image_crop + 10], dx
    ; row 2
    mov byte dl, [es:image_tail + bx + 301]
    mov word [image_crop + 12], dx
    mov byte dl, [es:image_tail + bx + 302]
    mov word [image_crop + 14], dx
    mov byte dl, [es:image_tail + bx + 303]
    mov word [image_crop + 16], dx
    ret

update_image_head_and_tail_crop:
    mov bx, [current_image_index]
    ; row 0
    mov byte dh, 0
    mov byte dl, [ds:image_head + bx - 303]
    mov word [image_crop + 0], dx
    mov byte dl, [ds:image_head + bx - 302]
    mov word [image_crop + 2], dx
    mov byte dl, [ds:image_head + bx - 301]
    mov word [image_crop + 4], dx
    ; row 1
    mov byte dl, [ds:image_head + bx - 1]
    mov word [image_crop + 6], dx
    mov byte dl, [ds:image_head + bx]
    mov word [image_crop + 8], dx
    mov byte dl, [ds:image_head + bx + 1]
    mov word [image_crop + 10], dx
    ; row 2
    mov byte dl, [es:image_head + bx - 45300]
    mov word [image_crop + 12], dx
    mov byte dl, [es:image_head + bx - 45301]
    mov word [image_crop + 14], dx
    mov byte dl, [es:image_head + bx - 45302]
    mov word [image_crop + 16], dx
    ret

; read image file, load to memory and plot
read_and_plot_image:
    ; open file and fill buffer
    call open_image

    ;
    push word 303
    mov word si, set_image_head
    call read_image_part

    ;
    push word 1
    mov word si, set_image_tail
    call read_image_part

    ;
    call draw_plotting_text

    ;
    call fill_image_head_last_line

    ;
    push word 303
    push word 20 ; x0
    push word 89 ; y0
    mov word si, get_image_head
    call plot_image_part

    ;
    push word 1
    push word 20 ; x0
    push word 239 ; y0
    mov word si, get_image_tail
    call plot_image_part

    ; close file
    call close_image
    ret

; si <- offset to set function, [bp + 2] initial pixel pos
read_image_part:
    push bp
    mov bp, sp
    sub sp, 6
    mov word [bp - 2], 0 ; columns
    mov word [bp - 4], 0 ; rows
    mov word bx, [bp + 4]
    mov word [bp - 6], bx ; pixel_index

    ; update buffer
    call fill_image_buffer

    read_image_part_loop:
        ; set pixel in memory
        call get_next_pixel
        mov word bx, [bp - 6]
        call si
        inc word [bp - 6]

        ; check column limit
        inc word [bp - 2]
        cmp word [bp - 2], 300
        jl read_image_part_loop

        ; check row limit
        inc word [bp - 4]
        cmp word [bp - 4], 150
        jg read_image_part_end

        ; update column and row
        mov word [bp - 2], 0 ; columns = 0
        add word [bp - 6], 2 ; skip borders
        call fill_image_buffer
        jmp read_image_part_loop

    read_image_part_end:
        add sp, 6
        pop bp
        ret 2

; initial_x: [bp + 6] and initial_y: [bp + 4],
; initial_pos: [bp + 8], si <- offset to set function
plot_image_part:
    push bp
    mov bp, sp
    sub sp, 6
    mov word [bp - 2], 0 ; columns
    mov word [bp - 4], 0 ; rows
    mov word bx, [bp + 8]
    mov word [bp - 6], bx ; pixel_index

    ;
    mov word bx, 0
    mov byte cl, 16

    plot_image_part_loop:
        mov word bx, [bp - 6]
        call si
        inc word [bp - 6]
        mov ah, 0
        div cl
        mov byte [color], al
        mov bx, [bp + 6]
        add bx, [bp - 2]
        push bx
        mov bx, [bp + 4]
        add bx, [bp - 4]
        push bx
        call plot_xy

        ; check column limit
        inc word [bp - 2]
        cmp word [bp - 2], 300
        jl plot_image_part_loop

        ; check row limit
        inc word [bp - 4]
        cmp word [bp - 4], 150
        jg plot_image_part_end
        mov word [bp - 2], 0
        add word [bp - 6], 2 ; skip borders
        jmp plot_image_part_loop
        
    plot_image_part_end:
        add sp, 6
        pop bp
        ret 6

;
set_image_head:
    mov byte [ds:image_head + bx], al
    ret

set_image_tail:
    mov byte [es:image_tail + bx], al
    ret

;
get_image_head:
    mov byte al, [ds:image_head + bx]
    ret

get_image_tail:
    mov byte al, [es:image_tail + bx]
    ret

; get next pixel value from buffer
get_next_pixel:
    mov ax, 0000h
    mov bx, [buffer_index]
    mov cl, 10

    ; read the first character to not have to do multiplication
    mov dl, [bx + buffer]
    inc bx
    cmp dl, ' '
    je end_conversion
    jmp start_conversion

    next_char:
        mov dl, [bx + buffer]
        inc bx
        cmp dl, ' '
        je end_conversion
        mul cl
    start_conversion:
        sub dl, '0'
        add al, dl
        jmp next_char
    end_conversion:
        mov word [buffer_index], bx
        ret

; ensure that image buffer has buffer_size characters
fill_image_buffer:
    ; update file offset value
    mov dx, [file_offset]
    mov cx, [file_offset + 2]
    add dx, [buffer_index]
    adc cx, 0
    mov [file_offset], dx
    mov [file_offset + 2], cx

    ; update file position
    mov al, 0
    mov bx, [handle]
    mov ah, 42h
    int 21h

    ; read image and fill the buffer
    mov cx, buffer_size
    call update_image_buffer
    mov word [buffer_index], 0
    ret

; Update image buffer, need put amount to read in cx
update_image_buffer:
    mov ah, 3fh
    mov bx, [handle]
    mov dx, buffer
    int 21h
    ret

; open image putting buffer_size characters in the buffer
open_image:
    ; clean image offset
    mov word [file_offset], 0
    mov word [file_offset + 2], 0
    mov word [buffer_index], 0

    mov ax, 3d00h ; open in read-only mode
    mov dx, image_path
    int 21h
    mov [handle], ax
    ret

; close image with handle
close_image:
    mov ah, 3eh
    mov bx, [handle]
    int 21h
    ret

; close program and back to dos
close_program:
    mov ah, 0
    mov al, [previous_mode]
    int 10h
    mov ax, 4c00h
    int 21h
    ret

; Draw functions

draw_texts:
    pusha
    mov byte[color], intense_white
    call draw_open_text
    call draw_exit_text
    call draw_low_pass_text
    call draw_high_pass_text
    call draw_gradient_text
    call draw_student_text
    call draw_subject_text
    popa
    ret

draw_interface:
    call draw_interface_lines
    call draw_texts
    ret

draw_reading_text:
    mov byte [color], red
    mov cx, 18 ; charactere number
    mov bx, 0
    mov dh, 15 ; line 0-29
    mov dl, 12 ; column 0-79

    draw_reading_text_loop:
        call cursor
        mov al, [bx + reading]
        call character
        inc bx ; next character
        inc dl ; next column
        loop draw_reading_text_loop
    ret


draw_plotting_text:
    mov byte [color], green
    mov cx, 18 ; charactere number
    mov bx, 0
    mov dh, 15 ; line 0-29
    mov dl, 12 ; column 0-79

    draw_plotting_text_loop:
        call cursor
        mov al, [bx + plotting]
        call character
        inc bx ; next character
        inc dl ; next column
        loop draw_plotting_text_loop
    ret

draw_student_text:
    mov cx, 36 ; charactere number
    mov bx, 0
    mov dh, 26 ; line 0-29
    mov dl, 22 ; column 0-79

    draw_student_text_loop:
        call cursor
        mov al, [bx + student]
        call character
        inc bx ; next character
        inc dl ; next column
        loop draw_student_text_loop
    ret

draw_subject_text:
    mov cx, 31 ; charactere number
    mov bx, 0
    mov dh, 27 ; line 0-29
    mov dl, 24 ; column 0-79

    draw_subject_text_loop:
        call cursor
        mov al, [bx + subject]
        call character
        inc bx ; next character
        inc dl ; next column
        loop draw_subject_text_loop
    ret

draw_open_text:
    mov cx, 5 ; charactere number
    mov bx, 0
    mov dh, 3 ; line 0-29
    mov dl, 5 ; column 0-79

    draw_open_text_loop:
        call cursor
        mov al, [bx + open]
        call character
        inc bx ; next character
        inc dl ; next column
        loop draw_open_text_loop
    ret

draw_exit_text:
    mov cx, 4 ; charactere number
    mov bx, 0
    mov dh, 3 ; line 0-29
    mov dl, 15 ; column 0-79

    draw_exit_text_loop:
        call cursor
        mov al, [bx + exit]
        call character
        inc bx ; next character
        inc dl ; next column
        loop draw_exit_text_loop
    ret

draw_low_pass_text:
    mov cx, 12 ; charactere number
    mov bx, 0
    mov dh, 3 ; line 0-29
    mov dl, 25 ; column 0-79

    draw_low_pass_text_loop:
        call cursor
        mov al, [bx + low_filter_text]
        call character
        inc bx ; next character
        inc dl ; next column
        loop draw_low_pass_text_loop
    ret

draw_high_pass_text:
    mov cx, 11 ; charactere number
    mov bx, 0
    mov dh, 3 ; line 0-29
    mov dl, 44 ; column 0-79

    draw_high_pass_text_loop:
        call cursor
        mov al, [bx + high_filter_text]
        call character
        inc bx ; next character
        inc dl ; next column
        loop draw_high_pass_text_loop
    ret

draw_gradient_text:
    mov cx, 9 ; charactere number
    mov bx, 0
    mov dh, 3 ; line 0-29
    mov dl, 64 ; column 0-79

    draw_gradient_text_loop:
        call cursor
        mov al, [bx + gradient_text]
        call character
        inc bx ; next character
        inc dl ; next column
        loop draw_gradient_text_loop
    ret

draw_interface_lines:
    ; bottom line
    mov byte[color], intense_white
    push word 20
    push word 474 ; 5
    push word 620
    push word 474 ; 5
    call line
    ; left line
    push word 19
    push word 474 ; 5
    push word 19
    push word 9 ; 470
    call line
    ; top line
    push word 19
    push word 9 ; 470
    push word 621
    push word 9 ; 470
    call line
    ; right line
    push word 621
    push word 9 ; 470
    push word 621
    push word 474 ; 5
    call line
    ; HUD - top line
    push word 19
    push word 88 ; 391
    push word 621
    push word 88 ; 391
    call line
    ; HUD - bottom line
    push word 19
    push word 390 ; 90
    push word 621
    push word 390 ; 90
    call line
    ; HUD - middle line
    push word 320
    push word 9 ; 470
    push word 320
    push word 389 ; 90
    call line
    ; HUD - name - top line
    push word 80
    push word 409 ; 70
    push word 560
    push word 409 ; 70
    call line
    ; HUD - name - bottom line
    push word 80
    push word 454 ; 25
    push word 560
    push word 454 ; 25
    call line
    ; HUD - name - left line
    push word 80
    push word 409 ; 70
    push word 80
    push word 454 ; 25
    call line
    ; HUD - name - right line
    push word 560
    push word 409 ; 70
    push word 560
    push word 454 ; 25
    call line
    ; button line 0
    push word 95
    push word 9 ; 470
    push word 95
    push word 88 ; 391
    call line
    ; button line 1
    push word 170
    push word 9 ; 470
    push word 170
    push word 88 ; 391
    call line
    ; button line 2
    push word 470
    push word 9 ; 470
    push word 470
    push word 88 ; 391
    call line
    ret

; dh = line (0-29) and dl = column (0-79)
cursor:
    push ax
    push bx
    mov ah, 2
    mov bh, 0
    int 10h
    pop bx
    pop ax
    ret

; draw character on cursor position, using color in color global variable
character:
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

; draw a pixel on cursor position, using color in color global variable
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
    mov dx, [bp+4]
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

; draw a line, using color in color global variable
; need push on the stack px0, py0, px1, py1 in that respective order
line:
    push bp
    mov bp,sp
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
    jmp end_line
line31:
    inc bx
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
    mov  si,ax
line4:
    push ax
    push dx
    push si
    sub si, ax
    mov ax, [delta_y]
    imul si
    mov si, [delta_x]
    shr si,1
    cmp dx, 0
    jl ar1
    add ax, si
    adc dx, 0
    jmp arc1
ar1:
    sub ax, si
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
    je end_line
    inc si
    jmp line4
line5: 
    cmp bx, dx
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
ar2:
    sub ax, si
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
    je end_line
    inc si
    jmp line6
end_line:
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    popf
    pop bp
    ret 8

; Other segments
segment data
    ; current color
    color db intense_white

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

    ; other global variables
    previous_mode db 0
    delta_x dw 0
    delta_y dw 0
    student db 'GABRIEL SOARES BAPTISTA - 2018102117'
    subject db 'SISTEMAS EMBARCADOS I - 2022/01'
    low_filter_text db 'Passa-Baixas'
    high_filter_text db 'Passa-Altas'
    gradient_text db 'Gradiente'
    open db 'Abrir'
    exit db 'Sair'
    reading db 'Convertendo imagem'
    plotting db ' Plotando imagem  '

    ; image variables
    image_path db 'imagem.txt', 0
    handle dw 0

    ; read image variables
    buffer_size equ 1200
    buffer: resb buffer_size
    buffer_index dw 0
    char_index dw 0

    ; file seek variables
    file_offset dw 0, 0

    ; image buffer variables
    image_head times 46206 db 0 ; half (302*302/2 = 45602) and last two lines of image_tail (604)

    ;
    current_image_index dw 302
    image_crop: times 9 dw 0
    mask: times 9 dw 0
    mask2: times 9 dw 0

    ;
    not_mean_flag db 0

segment extra
    image_tail times 45602 db 0

segment stack stack
    resb 512
stacktop:
