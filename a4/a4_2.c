#include <stdio.h>
#include <locale.h>

#define N 16

int main() {
    setlocale(LC_ALL, "rus");
    
    char A[N] = {1, 3, 5, 2, 3, 0, -3, -3, 0, 5, 5, 0, 2, 1, 1, 0};
    char B[N];
    int C[N];
    int count = 0;
    int i;

    __asm {
        lea esi, A        ; esi = адрес начала массива A
        lea edi, B        ; edi = адрес начала массива B
        lea ebx, C        ; ebx = адрес начала массива C
        mov ecx, 8        ; счетчик цикла: 16 байтов / 2 = 8 слов
        xor edx, edx      ; обнуляем edx, он будет счетчиком подходящих слов

    L_START:
        mov al, [esi + 1] ; al = старший байт слова
        
        cmp al, [esi]     ; сравниваем старший байт с младшим байтом как знаковые числа
        jge NEXT_WORD     ; если старший байт >= младшего, переходим к следующему слову

        mov ax, [esi]     ; если старший байт меньше младшего, копируем слово
        mov [edi], ax     ; помещаем слово в массив B
        add edi, 2        ; сдвигаем указатель массива B на 2 байта
        
        mov [ebx], esi    ; помещаем адрес текущего слова из esi в массив C
        add ebx, 4        ; сдвигаем указатель массива C на 4 байта

        mov word ptr [esi], 0 ; заменяем найденное слово нулем
        
        inc edx           ; увеличиваем счетчик найденных слов

    NEXT_WORD:
        add esi, 2        ; переходим к следующему слову массива A (сдвиг на 2 байта)
        loop L_START      ; уменьшаем ecx и если ecx != 0, переходим на L_START

        mov count, edx    ; сохраняем итоговое количество слов в переменную count
    }

    printf("Массив A после обработки:\n");
    for (i = 0; i < N; i++) {
        printf("%d %x\n", A[i], &A[i]);
    }
    printf("\n\nКоличество найденных слов: %d\n", count);

    if (count > 0) {
        printf("Массив B:\n");
        for (i = 0; i < count * 2; i++) {
            printf("%d ", B[i]);
        }
        
        printf("\n\nМассив C:\n");
        for (i = 0; i < count; i++) {
            printf("%x ", C[i]);
        }
        printf("\n");
    }

    return 0;
}
