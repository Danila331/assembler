#include <stdio.h>

void printBin8(unsigned char val) {
    for (int i = 7; i >= 0; i--)
        printf("%d", (val >> i) & 1);
}

void printBin16(unsigned short val) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (val >> i) & 1);
        if (i == 8) printf(" ");
    }
}

void processByte(unsigned char *A, int n) {
    __asm {
        mov esi, A              ; адрес начала массива A в esi
        mov ecx, n              ; количество элементов в ecx (счётчик цикла)

    loop_byte:
        mov al, [esi]           ; текущий элемент массива в al
        mov bl, al              ; копия в bl
        mov dl, al              ; копия в dl

        and al, 10000000b       ; маска старшего бита
        shr al, 7               ; прижать старший бит к правому краю

        and dl, 00000001b       ; маска младшего бита
        xor dl, 00000001b       ; инверсия младшего бита

        cmp al, dl              ; сравнить старший бит с инверсией младшего
        jne skip_byte           ; не совпало - пропустить обмен полей

        mov al, [esi]           ; текущий элемент массива в al
        mov bl, al              ; копия в bl для бита 4
        mov dl, al              ; копия в dl для бита 2

        and bl, 00010000b       ; маска бита 4
        shr bl, 4               ; прижать бит 4 к правому краю

        and dl, 00000100b       ; маска бита 2
        shr dl, 2               ; прижать бит 2 к правому краю

        cmp bl, dl              ; сравнить биты 4 и 2
        jne swap_42_byte        ; отличаются - поменять местами через xor

        mov bl, 0               ; переход к проверке битов 3 и 1
        mov dl, 1
        cmp bl, dl
        jne check_31_byte

    swap_42_byte:
        xor byte ptr [esi], 00010100b  ; обмен битов 4 и 2

    check_31_byte:
        mov al, [esi]           ; текущий элемент массива в al
        mov bl, al              ; копия в bl для бита 3
        mov dl, al              ; копия в dl для бита 1

        and bl, 00001000b       ; маска бита 3
        shr bl, 3               ; прижать бит 3 к правому краю

        and dl, 00000010b       ; маска бита 1
        shr dl, 1               ; прижать бит 1 к правому краю

        cmp bl, dl              ; сравнить биты 3 и 1
        jne swap_31_byte        ; отличаются - поменять местами через xor

        mov bl, 0               ; переход к следующему элементу
        mov dl, 1
        cmp bl, dl
        jne skip_byte

    swap_31_byte:
        xor byte ptr [esi], 00001010b  ; обмен битов 3 и 1

    skip_byte:
        inc esi                 ; перейти к следующему байту
        loop loop_byte          ; повторить цикл для следующего элемента

        nop                     ; конец ассемблерной вставки
    }
}

void processWord(unsigned short *A, int n) {
    __asm {
        mov esi, A              ; адрес начала массива A в esi
        mov ecx, n              ; количество элементов в ecx

    loop_word:
        mov ax, [esi]           ; текущее слово в ax
        mov bx, ax              ; копия в bx
        mov dx, ax              ; копия в dx

        and ax, 1000000000000000b  ; маска старшего бита
        shr ax, 15                 ; прижать старший бит к правому краю

        and dx, 0000000000000001b  ; маска младшего бита
        xor dx, 0000000000000001b  ; инверсия младшего бита

        cmp ax, dx              ; сравнить старший бит с инверсией младшего
        jne skip_word           ; не совпало - пропустить обмен полей

        mov ax, [esi]           ; текущее слово в ax
        mov bx, ax              ; копия в bx для бита 4
        mov dx, ax              ; копия в dx для бита 2

        and bx, 0000000000010000b  ; маска бита 4
        shr bx, 4                  ; прижать бит 4 к правому краю

        and dx, 0000000000000100b  ; маска бита 2
        shr dx, 2                  ; прижать бит 2 к правому краю

        cmp bx, dx              ; сравнить биты 4 и 2
        jne swap_42_word        ; отличаются - поменять местами через xor

        mov bx, 0               ; переход к проверке битов 3 и 1
        mov dx, 1
        cmp bx, dx
        jne check_31_word

    swap_42_word:
        xor word ptr [esi], 0000000000010100b  ; обмен битов 4 и 2

    check_31_word:
        mov ax, [esi]           ; текущее слово в ax
        mov bx, ax              ; копия в bx для бита 3
        mov dx, ax              ; копия в dx для бита 1

        and bx, 0000000000001000b  ; маска бита 3
        shr bx, 3                  ; прижать бит 3 к правому краю

        and dx, 0000000000000010b  ; маска бита 1
        shr dx, 1                  ; прижать бит 1 к правому краю

        cmp bx, dx              ; сравнить биты 3 и 1
        jne swap_31_word        ; отличаются - поменять местами через xor

        mov bx, 0               ; переход к следующему элементу
        mov dx, 1
        cmp bx, dx
        jne skip_word

    swap_31_word:
        xor word ptr [esi], 0000000000001010b  ; обмен битов 3 и 1

    skip_word:
        add esi, 2              ; перейти к следующему слову (+2 байта)
        loop loop_word          ; повторить цикл для следующего элемента

        nop                     ; конец ассемблерной вставки
    }
}


void printArrayByte(unsigned char *A, int n) {
    for (int i = 0; i < n; i++) {
        printf("[%d] ", i);
        printBin8(A[i]);
        printf(" 0x%02X\n", A[i]);
    }
    printf("\n");
}

void printArrayWord(unsigned short *A, int n) {
    for (int i = 0; i < n; i++) {
        printf("[%d] ", i);
        printBin16(A[i]);
        printf(" 0x%04X\n", A[i]);
    }
    printf("\n");
}

int main() {

    unsigned char byteArr[6] = { 0x05, 0x7B, 0x65, 0x37, 0x1B, 0x6F };
    unsigned short wordArr[6] = { 0x0005, 0x007B, 0x1265, 0xAB37, 0x001B, 0x556F };

    printf("BYTES BEFORE:\n");
    printArrayByte(byteArr, 6);

    processByte(byteArr, 6);

    printf("BYTES AFTER:\n");
    printArrayByte(byteArr, 6);

    printf("WORDS BEFORE:\n");
    printArrayWord(wordArr, 6);

    processWord(wordArr, 6);

    printf("WORDS AFTER:\n");
    printArrayWord(wordArr, 6);

    return 0;
}
