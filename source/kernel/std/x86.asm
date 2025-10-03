bits 16

section _TEXT class=CODE
; int 10h ah=0Eh
; args: character, page
;
global _x86_Video_WriteCharTeletype
global _x86_diskread_function
global _x86_readbyte_function
global _x86_protected_mode_jump

%define ENDL 0x0D, 0x0A

bdb_sectors_per_track:      dw 18
bdb_heads:                  dw 2

_x86_protected_mode_jump:
    jmp 0x0000:0x0500
;--------------------------------------------------------------------------------------------------------------------------------------------------------------


_x86_Video_WriteCharTeletype:
    
    ; make new call frame
    push bp             ; save old call frame
    mov bp, sp          ; initialize new call frame

    ; save bx
    push bx

    ; [bp + 0] - old call frame
    ; [bp + 2] - return address (small memory model => 2 bytes)
    ; [bp + 4] - first argument (character)
    ; [bp + 6] - second argument (page)
    ; note: bytes are converted to words (you can't push a single byte on the stack)
    mov ah, 0Eh
    mov al, [bp + 4]
    mov bh, [bp + 6]

    int 10h

    ; restore bx
    pop bx

    ; restore old call frame
    mov sp, bp
    pop bp
    ret


_x86_readbyte_function:
    
    ; make new call frame
    push bp             ; save old call frame
    mov bp, sp          ; initialize new call frame

    ; save bx
    push bx

    ; [bp + 0] - old call frame
    ; [bp + 2] - return address (small memory model => 2 bytes)
    ; [bp + 4] - first argument (character)
    ; [bp + 6] - second argument (page)
    ; note: bytes are converted to words (you can't push a single byte on the stack)
    mov bx, [bp + 4]
    mov es, bx
    mov bx, [bp + 6]
    mov al, es:[bx]     ; Read byte from segment:offset
    mov ah, 0x00        ; Clear AH, return value in AX (AL contains byte)

    ; restore bx
    pop bx

    ; restore old call frame
    mov sp, bp
    pop bp
    ret


_x86_diskread_function:
    
    ; make new call frame
    push bp             ; save old call frame
    mov bp, sp          ; initialize new call frame

    ; save bx
    push bx

    ; [bp + 0] - old call frame
    ; [bp + 2] - return address (small memory model => 2 bytes)
    ; [bp + 4] - first argument (character)
    ; [bp + 6] - second argument (page)
    ; note: bytes are converted to words (you can't push a single byte on the stack

    mov ax, [bp + 4]
    call lba_to_chs
    mov ah, 0x02
    mov al, 0x01
    xor dl, dl
    mov bx, [bp + 6]
    mov es, bx
    mov bx, [bp + 8]
    int 0x13

    ;mov al, [es:bx]
    ;mov bh, 0
    ;call print_hex

    ; restore bx
    pop bx

    ; restore old call frame
    mov sp, bp
    pop bp
    ret

; Converts an LBA address to a CHS address
; Parameters:
;   - ax: LBA address
; Returns:
;   - cx [bits 0-5]: sector number
;   - cx [bits 6-15]: cylinder
;   - dh: head
;

lba_to_chs:

    push ax
    push dx

    xor dx, dx                          ; dx = 0
    div word [bdb_sectors_per_track]    ; ax = LBA / SectorsPerTrack
                                        ; dx = LBA % SectorsPerTrack

    inc dx                              ; dx = (LBA % SectorsPerTrack + 1) = sector
    mov cx, dx                          ; cx = sector

    xor dx, dx                          ; dx = 0
    div word [bdb_heads]                ; ax = (LBA / SectorsPerTrack) / Heads = cylinder
                                        ; dx = (LBA / SectorsPerTrack) % Heads = head
    mov dh, dl                          ; dh = head
    mov ch, al                          ; ch = cylinder (lower 8 bits)
    shl ah, 6
    or cl, ah                           ; put upper 2 bits of cylinder in CL

    pop ax
    mov dl, al                          ; restore DL
    pop ax
    ret

print_hex:
    push ax
    push bx
    push cx
    push dx

    mov ah, 0x0E       ; BIOS teletype function

    ; Get upper nibble
    mov cl, al         ; Copy byte to CL
    shr cl, 4          ; Shift right 4 times to get upper nibble
    call print_nibble  ; Print upper nibble

    ; Get lower nibble
    mov cl, al         ; Restore original byte
    and cl, 0x0F       ; Mask out upper nibble
    call print_nibble  ; Print lower nibble

    pop dx
    pop cx
    pop bx
    pop ax
    ret

print_nibble:
    add cl, 0x30       ; Convert 0-9 to '0'-'9'
    cmp cl, 0x39       ; Check if it's above '9'
    jbe print_char     ; If <= '9', print it
    add cl, 7          ; Convert A-F (add 7 to turn '9' into 'A'-'F')

print_char:
    mov al, cl         ; Load ASCII char into AL
    int 0x10           ; Print char
    ret
