nasm16 -f obj -o d:\%1.obj -l d:\%1.lst d:\%1.asm
freelink d:\%1
del d:\%1.obj
del d:\%1.lst
