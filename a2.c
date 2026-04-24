#include <stdio.h>
#include <locale.h>

int main() {
    
    int i;
    char y, z;
    short int x, v, v_c, v_as;
    setlocale(LC_ALL, "rus");

    printf("Enter test number: ");
    scanf("%d", &i);
    switch (i) {
        case 1:
            x = -0x2;
            y = 0x7;
            z = 0x3;
            v = -0x1F;
            break;
        case 2:
            x = 0xFBE;
            y = 0x56;
            z = -0x7F;
            v = 0x2E5B;
            break;
    }

    v_c = -4 + (-y + x * (3 * z + 1)) / (z - 2);

    __asm {
        mov al, z       ; Помещаем z в AL
        mov bl, 3       ; Помещаем множитель 3 в BL
        imul bl         ; Вычисляем AX = z * 3
        add ax, 1       ; Вычисляем AX = 3z + 1

        imul x          ; Вычисляем DX:AX = AX * x = x * (3z + 1)

        mov bx, ax      ; Сохраняем младшее слово
        mov cx, dx      ; Сохраняем старшее слово

        mov al, y       ; Помещаем y в AL
        cbw             ; Расширяем байт AL до слова AX
        cwd             ; Расширяем слово AX до двойного слова DX:AX

        sub bx, ax      ; Выполняем вычитание. Вычитаем младшие слова
        sbb cx, dx      ; Вычитаем старшие слова с учетом заема

        mov al, z       ; Помещаем z в AL
        cbw             ; Расширяем AL до слова AX
        sub ax, 2       ; AX = z - 2
        
        xchg bx, ax     ; Перемещаем знаменатель в BX (одновременно возвращая младшее слово числителя в AX)
        mov dx, cx      ; Возвращаем результат числителя в DX (старшее слово). Теперь DX:AX = Числитель

        idiv bx         ; Делим DX:AX (Числитель) на BX (Знаменатель). Результат: ч 

        add ax, -4      ; Прибавляем -4

        mov v_as, ax    ; Сохраняем результат
    }

    printf("Expected value (v):  %d (DEC) | %hX (HEX)\n", v, v);
    printf("Result in C (v_c):   %d (DEC) | %hX (HEX)\n", v_c, v_c);
    printf("Result ASM (v_as):   %d (DEC) | %hX (HEX)\n", v_as, v_as);

    return 0;
}
