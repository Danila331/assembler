#include <stdio.h>
#include <locale.h>

int main() {
    
    int i;
    char x, z;
    short int y, v, v_c, v_as;
    setlocale(LC_ALL, "rus");

    printf("Enter test number: ");
    scanf("%d", &i);
    switch (i) {
        case 1:
            x = -0x1;
            y = 0x13;
            z = -0x2;
            v = -0x6;
            break;
        case 2:
            x = -0x7E;
            y = 0x4FBA;
            z = 0x7F;
            v = 0x8F;
            break;
    }

    v_c = -5 + ((y + 7 * (x - 2)) / (z + 4)) - 1;

    __asm {
        mov al, x       ; Помещаем x в AL
        cbw             ; Расширяем байт AL до слова AX
        sub ax, 2       ; Вычисляем AX = x - 2

        mov bx, 7       ; Помещаем множитель 7 в BX
        imul bx         ; Вычисляем DX:AX = 7 * (x - 2)

        mov bx, ax      ; Сохраняем младшее слово
        mov cx, dx      ; Сохраняем старшее слово

        mov ax, y       ; Помещаем y в AX
        cwd             ; Расширяем слово AX до двойного слова DX:AX

        add bx, ax      ; Выполняем сложение. Складываем младшие слова
        adc cx, dx      ; Складываем старшие слова с учетом переноса

        mov al, z       ; Помещаем z в AL
        cbw             ; Расширяем AL до слова AX
        add ax, 4       ; AX = z + 4
        
        xchg bx, ax     ; Перемещаем знаменатель в BX (одновременно возвращая младшее слово числителя в AX)
        mov dx, cx      ; Возвращаем результат числителя в DX (старшее слово). Теперь DX:AX = Числитель

        idiv bx         ; Делим DX:AX (Числитель) на BX (Знаменатель). Результат: ч 

        add ax, -5      ; Прибавляем -5
        sub ax, 1       ; Вычитаем 1

        mov v_as, ax    ; Сохраняем результат
    }

    printf("Expected value (v):  %d (DEC) | %hX (HEX)\n", v, v);
    printf("Result in C (v_c):   %d (DEC) | %hX (HEX)\n", v_c, v_c);
    printf("Result ASM (v_as):   %d (DEC) | %hX (HEX)\n", v_as, v_as);

    return 0;
}
