#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void selection_sort(int massive[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < n; j++) {
            if (massive[j] < massive[min_index]) {
                min_index = j;
            }
        }
        int temp = massive[i];
        massive[i] = massive[min_index];
        massive[min_index] = temp;
    }
}

void kycha(int massive[], int n, int i) {
    int largest = i; 
    int left = 2 * i + 1; 
    int right = 2 * i + 2; 

    if (left < n && massive[left] > massive[largest])
        largest = left;

    if (right < n && massive[right] > massive[largest])
        largest = right;


    if (largest != i) {
        int temp = massive[i];
        massive[i] = massive[largest];
        massive[largest] = temp;

        kycha(massive, n, largest);
    }
}

// heapsort
void heap_sort(int massive[], int low, int high) {
    int n = high - low + 1;  
    
    if (n <= 1) return;
    
    int temp[n];
    for (int i = 0; i < n; i++) {
        temp[i] = massive[low + i];
    }
        for (int i = n / 2 - 1; i >= 0; i--)
        kycha(temp, n, i);

    for (int i = n - 1; i >= 0; i--) {
        int swap_temp = temp[0];
        temp[0] = temp[i];
        temp[i] = swap_temp;

        kycha(temp, i, 0);
    }
        for (int i = 0; i < n; i++) {
        massive[low + i] = temp[i];
    }
}

void print_massive(int massive[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", massive[i]);
    printf("\n");
}

int* file_massive(const char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Невозможно открыть файл");
        return NULL;
    }
    fscanf(file, "%d", size);
    int* massive = malloc(*size * sizeof(int));
    for (int i = 0; i < *size; i++) {
        fscanf(file, "%d", &massive[i]);
    }
    fclose(file);
    return massive;
}

int* random_massive(int size) {
    int* massive = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        massive[i] = rand() % 1000; 
    }
    return massive;
}

int main() {
    srand(time(0));
    int choice;
    int method;
    int size;
    int* massive;
    
    printf("Выберите метод сортировки:\n1. Сортировка выбором\n2. Heapsort\n");
    printf("--- Выбранный метод: ");
    scanf("%d", &method);
    printf("\n");
    
    printf("Выберите способ задания массива:\n1. Ввод с клавиатуры\n2. Генерация случайных чисел\n3. Чтение из файла\n");
    printf("--- Выбранный способ: ");
    scanf("%d", &choice);
    printf("\n");
    
    if (choice == 1) {
        printf("Введите размер массива: ");
        scanf("%d", &size);
        printf("\n");
        massive = malloc(size * sizeof(int));
        printf("Введите элементы массива:\n");
        for (int i = 0; i < size; i++) {
            scanf("%d", &massive[i]);
        }
    } else if (choice == 2) {
        printf("Введите размер массива: ");
        scanf("%d", &size);
        printf("\n");
        massive = random_massive(size);
    } else if (choice == 3) {
        char filename[256];
        printf("Введите имя файла: ");
        scanf("%s", filename);
        printf("\n");
        massive = file_massive(filename, &size);
        if (!massive) return 1;
    } else {
        printf("Неверный выбор.\n");
        return 1;
    }
    
    printf("Исходный массив:\n");
    print_massive(massive, size);
    
    if (method == 1) {
        selection_sort(massive, size);
        printf("Отсортированный массив:\n");
    } else if (method == 2) {
        heap_sort(massive, 0, size - 1);
        printf("Отсортированный массив:\n");
    } else {
        printf("Неверный выбор метода сортировки.\n");
        free(massive);
        return 1;
    }
    
    print_massive(massive, size);
    free(massive);
    return 0;
}

