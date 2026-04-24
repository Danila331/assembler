#include <stdio.h>
#include <locale.h>

#define N 16

int main() {
    setlocale(LC_ALL, "rus");
    
    short int A[N] = {1, 3, 2, 5, 3, 0, -3, -3, 0, 5, 5, 0, 1, 2, 1, 0};
    short int B[N];
    int C[N];
    int count = 0;
    int i;

    __asm {
        lea esi, A        ; esi = адрес начала массива A
        add esi, 2        ; пропускаем крайний левый элемент, начинаем с A[1]
        lea edi, B        ; edi = адрес начала массива B
        lea ebx, C        ; ebx = адрес начала массива C
        mov ecx, 14       ; счетчик цикла: 16 элементов - 2 крайних = 14 итераций
        xor edx, edx      ; обнуляем edx, он будет счетчиком подходящих элементов

    L_START:
        mov ax, [esi - 2] ; ax = A[i-1] (левый сосед)
        add ax, [esi + 2] ; ax = A[i-1] + A[i+1] (сумма соседей)
        
        cmp ax, [esi]     ; сравниваем сумму соседей с текущим элементом A[i]
        jne NEXT_ELEM     ; если не равны, переходим к следующему элементу

        mov ax, [esi]     ; если элемент равен сумме соседей, загружаем текущий элемент A[i] обратно в ax
        mov [edi], ax     ; помещаем значение в массив B
        add edi, 2        ; сдвигаем указатель массива B на 2 байта
        
        mov [ebx], esi    ; помещаем адрес текущего элемента из esi в массив C
        add ebx, 4        ; сдвигаем указатель массива C на 4 байта
        
        inc edx           ; увеличиваем счетчик найденных элементов

    NEXT_ELEM:
        add esi, 2        ; переходим к следующему элементу массива A (сдвиг на 2 байта)
        loop L_START      ; уменьшаем ecx и если ecx != 0, переходим на L_START

        mov count, edx    ; сохраняем итоговое количество элементов в переменную C
    }

    printf("Исходный массив A:\n");
    for (i = 0; i < N; i++) {
        printf("%d %x\n", A[i], &A[i]);
    }
    printf("\n\nКоличество найденных элементов: %d\n", count);

    if (count > 0) {
        printf("Массив B:\n");
        for (i = 0; i < count; i++) {
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
