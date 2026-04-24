#include <stdio.h>
#include <string.h>
#include <locale.h>

void process_string(char* str) {
    int i;
    
    // Проверка длины
    if (strlen(str) != 4) {
        printf(" ОШИБКА: Введена строка неправильной длины (нужно 4 символа).\n");
        return;
    }

    // Проверка, что все символы - цифры
    for (i = 0; i < 4; i++) {
        // Если символ меньше '0' или больше '9', это не цифра
        if (str[i] < '0' || str[i] > '9') {
            printf(" ОШИБКА: Символ '%c' не является десятичной цифрой.\n", str[i]);
            return;
        }
    }

    // Вычитаем код '0', чтобы работать с числами 0..9
    str[0] -= '0';
    str[1] -= '0';
    str[2] -= '0';
    str[3] -= '0';

    __asm {
        mov ebx, str     ; Загружаем адрес строки в регистр ebx

        mov al, [ebx]    ; al = 1-я цифра
        mov cl, [ebx+1]  ; cl = 2-я цифра
        mov dl, [ebx+3]  ; dl = 4-я цифра

        cmp al, cl       ; Сравнить 1-ю и 2-ю
        je ELSE          ; Если равны, идем в ELSE

        cmp al, dl       ; Сравнить 1-ю и 4-ю
        jle ELSE         ; Если 1 <= 4, идем в ELSE

        cmp cl, dl       ; Сравнить 2-ю и 4-ю
        jle ELSE         ; Если 2 <= 4, идем в ELSE

        mov al, [ebx]    ; Все условия выполнены. al = 1-я цифра
        add al, [ebx+2]  ; al = 1-я + 3-я
        cmp al, 9        ; Проверяем, влезла ли сумма в цифру
        ja EXIT          ; Если сумма > 9, ничего не меняем, выход

        mov [ebx+1], al  ; Иначе записываем сумму во вторую позицию
        jmp EXIT         ; Конец обработки

        ELSE:
        mov al, 9        ; Кладем 9 в регистр al
        mov [ebx+2], al  ; al в 3-ю ячейку
        mov [ebx+3], al  ; al в 4-ю ячейку

        EXIT:
        nop
    }

    // Обратное преобразование в символы
    str[0] += '0';
    str[1] += '0';
    str[2] += '0';
    str[3] += '0';

    printf(" Результат: %s\n", str);
}

int main() {
    setlocale(LC_ALL, "rus");

    char auto_tests[][6] = {
        "8602", // Корректный -> 8802
        "5481", // Корректный -> 5481
        "4412", // Нарушение: 1-я==2-я -> 4499
        "2513", // Нарушение: 1-я < 4-я -> 2599
        "5213", // Нарушение: 2-я < 4-я -> 5299
        "12a3", // Ошибка ввода (буква)
        "12",   // Ошибка ввода (короткая)
        "12345" // Ошибка ввода (длинная)
    };
    int count = sizeof(auto_tests) / sizeof(auto_tests[0]);

    printf("Тесты:\n");
    for (int i = 0; i < count; i++) {
        printf("Тест #%d. Вход: '%s' ->", i + 1, auto_tests[i]);
        process_string(auto_tests[i]);
    }


    printf("Ручной ввод:\n");

    while (1) {
        char user_input[100];

        printf("Введите строку из 4 цифр (или '0' для выхода): ");
        scanf("%s", user_input);

        if (strcmp(user_input, "0") == 0) {
            break;
        }

        process_string(user_input);
    }

    return 0;
}
