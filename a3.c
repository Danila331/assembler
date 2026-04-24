#include <stdio.h>
#include <string.h>
#include <locale.h>

void process_string(char* str) {
    int i;
    
    // Проверка длины
    if (strlen(str) != 3) {
        printf(" ОШИБКА: Введена строка неправильной длины (нужно 3 символа).\n");
        return;
    }

    // Проверка, что все символы - цифры
    for (i = 0; i < 3; i++) {
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

    __asm {
        mov ebx, str     ; Загружаем адрес строки в регистр ebx

        mov al, [ebx]    ; al = 1-я цифра
        mov cl, [ebx+1]  ; cl = 2-я цифра
        mov dl, [ebx+2]  ; dl = 3-я цифра

        cmp al, dl       ; Сравнить 1-ю и 3-ю
        jne SWAP         ; Если не равны, меняем 1-ю и 3-ю

        cmp cl, dl       ; Сравнить 2-ю и 3-ю
        jne SWAP         ; Если не равны, меняем 1-ю и 3-ю

        mov al, [ebx+1]  ; Все цифры равны. al = 2-я цифра
        sub al, 2        ; Вычитаем 2
        cmp al, 0        ; Проверяем, не ушли ли ниже 0
        jge ELSE         ; Если результат >= 0, записываем его

        add al, 10       ; Для 0 и 1 получаем 8 и 9
        ELSE:
        mov [ebx+1], al  ; Записываем результат во вторую позицию
        jmp EXIT         ; Конец обработки

        SWAP:
        mov [ebx], dl    ; Меняем местами 1-ю и 3-ю цифры
        mov [ebx+2], al

        EXIT:
        nop
    }

    // Обратное преобразование в символы
    str[0] += '0';
    str[1] += '0';
    str[2] += '0';

    printf(" Результат: %s\n", str);
}

int main() {
    setlocale(LC_ALL, "rus");

    char auto_tests[][6] = {
        "123",  // 1-я и 2-я не совпадают с 3-й -> 321
        "223",  // 1-я и 2-я не совпадают с 3-й -> 322
        "121",  // 2-я не совпадает с 3-й -> 121
        "111",  // Все цифры совпадают -> 191
        "000",  // Все цифры совпадают -> 080
        "222",  // Все цифры совпадают -> 202
        "12a",  // Ошибка ввода (буква)
        "12",   // Ошибка ввода (короткая)
        "1234"  // Ошибка ввода (длинная)
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

        printf("Введите строку из 3 цифр (или '0' для выхода): ");
        scanf("%s", user_input);

        if (strcmp(user_input, "0") == 0) {
            break;
        }

        process_string(user_input);
    }

    return 0;
}
