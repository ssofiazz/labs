#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для дублирования четных чисел в массиве
void duplicateEvenNumbers(int** array, int* size) {
    // Сначала подсчитаем, сколько четных чисел
    int evenCount = 0;
    for (int i = 0; i < *size; i++) {
        if ((*array)[i] % 2 == 0) {
            evenCount++;
        }
    }
    
    if (evenCount == 0) {
        printf("Четных чисел не найдено.\n");
        return;
    }
    
    // Создаем новый массив большего размера
    int newSize = *size + evenCount;
    int* newArray = (int*)malloc(newSize * sizeof(int));
    if (newArray == NULL) {
        printf("Ошибка выделения памяти!\n");
        return;
    }
    
    // Копируем и дублируем четные числа
    int newIndex = 0;
    for (int i = 0; i < *size; i++) {
        // Копируем оригинальное число
        newArray[newIndex] = (*array)[i];
        newIndex++;
        
        // Если число четное - дублируем его
        if ((*array)[i] % 2 == 0) {
            newArray[newIndex] = (*array)[i];
            newIndex++;
        }
    }
    
    // Освобождаем старый массив и заменяем новым
    free(*array);
    *array = newArray;
    *size = newSize;
}

// Функция для ввода массива чисел
void inputIntArray(int** array, int* size) {
    printf("введите количество элементов: ");
    scanf("%d", size);
    
    if (*size <= 0) {
        printf("не положительное число\n");
        return;
    }
    
    *array = (int*)malloc(*size * sizeof(int));
    if (*array == NULL) {
        printf("ошибка выделения памяти\n");
        return;
    }
    
    printf("введите %d целых чисел:\n", *size);
    for (int i = 0; i < *size; i++) {
        printf("элемент %d: ", i + 1);
        scanf("%d", &(*array)[i]);
    }
}

// Функция для вывода массива чисел
void printIntArray(int* array, int size) {
    if (size == 0) {
        printf("массив пуст\n");
        return;
    }
    
    printf("массив [%d элементов]: ", size);
    for (int i = 0; i < size; i++) {
        printf("%d", array[i]);
        if (array[i] % 2 == 0) {
            printf("(чет)");
        }
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

// Функция для освобождения памяти
void freeIntArray(int** array, int* size) {
    if (*array != NULL) {
        free(*array);
        *array = NULL;
    }
    *size = 0;
}

// Функция для тестирования на различных наборах данных
void testVariousCases() {
    printf("\n=== ТЕСТИРОВАНИЕ НА РАЗЛИЧНЫХ НАБОРАХ ДАННЫХ ===\n");
    
    // Тест 1: Смешанные числа
    printf("\n1. Смешанные числа (1, 2, 3, 4, 5):\n");
    int size1 = 5;
    int* test1 = (int*)malloc(size1 * sizeof(int));
    int test1_data[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < size1; i++) test1[i] = test1_data[i];
    
    printf("Исходный массив: ");
    printIntArray(test1, size1);
    duplicateEvenNumbers(&test1, &size1);
    printf("После дублирования: ");
    printIntArray(test1, size1);
    free(test1);
    
    // Тест 2: Только четные числа
    printf("\n2. Только четные числа (2, 4, 6, 8):\n");
    int size2 = 4;
    int* test2 = (int*)malloc(size2 * sizeof(int));
    int test2_data[] = {2, 4, 6, 8};
    for (int i = 0; i < size2; i++) test2[i] = test2_data[i];
    
    printf("Исходный массив: ");
    printIntArray(test2, size2);
    duplicateEvenNumbers(&test2, &size2);
    printf("После дублирования: ");
    printIntArray(test2, size2);
    free(test2);
    
    // Тест 3: Только нечетные числа
    printf("\n3. Только нечетные числа (1, 3, 5, 7):\n");
    int size3 = 4;
    int* test3 = (int*)malloc(size3 * sizeof(int));
    int test3_data[] = {1, 3, 5, 7};
    for (int i = 0; i < size3; i++) test3[i] = test3_data[i];
    
    printf("Исходный массив: ");
    printIntArray(test3, size3);
    duplicateEvenNumbers(&test3, &size3);
    printf("После дублирования: ");
    printIntArray(test3, size3);
    free(test3);
    
    // Тест 4: Отрицательные числа
    printf("\n4. Отрицательные числа (-2, -1, 0, 3, 4):\n");
    int size4 = 5;
    int* test4 = (int*)malloc(size4 * sizeof(int));
    int test4_data[] = {-2, -1, 0, 3, 4};
    for (int i = 0; i < size4; i++) test4[i] = test4_data[i];
    
    printf("Исходный массив: ");
    printIntArray(test4, size4);
    duplicateEvenNumbers(&test4, &size4);
    printf("После дублирования: ");
    printIntArray(test4, size4);
    free(test4);
}

// Функция для создания тестового массива
void createTestArray(int** array, int* size) {
    *size = 6;
    *array = (int*)malloc(*size * sizeof(int));
    int testData[] = {1, 2, 3, 4, 5, 6};
    
    for (int i = 0; i < *size; i++) {
        (*array)[i] = testData[i];
    }
    printf("Создан тестовый массив: 1, 2, 3, 4, 5, 6\n");
}

int main() {
    int choice;
    int* intArray = NULL;
    int arraySize = 0;
    
    printf("Лабораторная работа: Дублирование четных чисел в массиве\n");
    
    do {
        printf("\n=== МЕНЮ ===\n");
        printf("1. Ввести массив чисел вручную\n");
        printf("2. Создать тестовый массив\n");
        printf("3. Дублировать четные числа\n");
        printf("4. Вывести массив\n");
        printf("5. Очистить массив\n");
        printf("6. Протестировать на различных наборах данных\n");
        printf("0. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                freeIntArray(&intArray, &arraySize);
                inputIntArray(&intArray, &arraySize);
                break;
                
            case 2:
                freeIntArray(&intArray, &arraySize);
                createTestArray(&intArray, &arraySize);
                break;
                
            case 3:
                if (intArray == NULL || arraySize == 0) {
                    printf("Массив пуст! Сначала введите данные.\n");
                } else {
                    duplicateEvenNumbers(&intArray, &arraySize);
                }
                break;
                
            case 4:
                printIntArray(intArray, arraySize);
                break;
                
            case 5:
                freeIntArray(&intArray, &arraySize);
                break;
                
            case 6:
                testVariousCases();
                break;
                
            case 0:
                printf("Выход из программы...\n");
                break;
                
            default:
                printf("Неверный выбор! Попробуйте снова.\n");
        }
    } while (choice != 0);
    
    // Очищаем память перед выходом
    freeIntArray(&intArray, &arraySize);
    
    return 0;
}