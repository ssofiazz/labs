#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LABS 10
#define MAX_FILENAME 100
#define MAX_ARGS 10
#define MAX_ARG_LENGTH 100

typedef struct {
    char filename[MAX_FILENAME];
    int number;
    char name[50];
    char args_prompt[200];  // Подсказка для аргументов
    int needs_args;         // Нужны ли аргументы (0/1)
} LabWork;

// Функция для сканирования доступных лаб
int scan_labs(LabWork labs[]) {
    // Список известных файлов лаб с их описаниями
    char *lab_files[] = {
        "alg1.c",
        "alg2.c", 
        "algi3.c",
        "algi4.c",
        "algi52.c",
        NULL
    };
    
    char *lab_names[] = {
        "Алгоритм 1",
        "Алгоритм 2",
        "Алгоритм 3", 
        "Алгоритм 4",
        "Алгоритм 5.2"
    };
    
    // Подсказки для аргументов каждой лабы
    char *args_prompts[] = {
        "Введите имя файла: ",
        "Введите аргументы (если есть) или нажмите Enter: ",
        "Введите аргументы (если есть) или нажмите Enter: ",
        "Введите аргументы (если есть) или нажмите Enter: ",
        "Введите аргументы (если есть) или нажмите Enter: "
    };
    
    // Флаги нужности аргументов (1 - нужны, 0 - не нужны)
    int needs_args[] = {1, 0, 0, 0, 0};
    
    int count = 0;
    for (int i = 0; lab_files[i] != NULL && i < MAX_LABS; i++) {
        // Проверяем существование файла
        if (access(lab_files[i], F_OK) != -1) {
            strcpy(labs[count].filename, lab_files[i]);
            labs[count].number = count + 1;
            strcpy(labs[count].name, lab_names[i]);
            strcpy(labs[count].args_prompt, args_prompts[i]);
            labs[count].needs_args = needs_args[i];
            count++;
        }
    }
    
    return count;
}

// Функция для чтения аргументов от пользователя
void get_arguments(char args[][MAX_ARG_LENGTH], int *arg_count, LabWork lab) {
    char input[200];
    *arg_count = 0;
    
    if (lab.needs_args) {
        printf("%s", lab.args_prompt);
        
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // Убираем символ новой строки
            input[strcspn(input, "\n")] = 0;
            
            // Если ввод не пустой
            if (strlen(input) > 0) {
                // Разбиваем строку на аргументы
                char *token = strtok(input, " ");
                while (token != NULL && *arg_count < MAX_ARGS) {
                    strncpy(args[*arg_count], token, MAX_ARG_LENGTH - 1);
                    args[*arg_count][MAX_ARG_LENGTH - 1] = '\0';
                    (*arg_count)++;
                    token = strtok(NULL, " ");
                }
            }
        }
    }
}

// Функция для компиляции и запуска лабы с аргументами
void compile_and_run(LabWork lab) {
    char executable[100];
    char compile_cmd[200];
    char run_cmd[500];  // Увеличили для аргументов
    
    // Запрашиваем аргументы, если нужны
    char arguments[MAX_ARGS][MAX_ARG_LENGTH];
    int arg_count = 0;
    
    if (lab.needs_args) {
        get_arguments(arguments, &arg_count, lab);
    }
    
    // Создаем имя исполняемого файла (без расширения .c)
    strcpy(executable, lab.filename);
    char *dot = strrchr(executable, '.');
    if (dot) *dot = '\0';
    
    printf("\n=== Компиляция %s ===\n", lab.filename);
    
    // Формируем команду компиляции
    snprintf(compile_cmd, sizeof(compile_cmd), 
             "gcc -o %s %s -lm", executable, lab.filename);
    
    // Компилируем
    int compile_result = system(compile_cmd);
    
    if (compile_result != 0) {
        printf("Ошибка компиляции!\n");
        return;
    }
    
    printf("Компиляция успешна!\n");
    printf("\n=== Запуск %s ===\n", lab.name);
    
    // Формируем команду запуска с аргументами
    if (arg_count > 0) {
        printf("Аргументы: ");
        for (int i = 0; i < arg_count; i++) {
            printf("%s ", arguments[i]);
        }
        printf("\n");
    }
    printf("================================\n");
    
    // Формируем команду запуска с аргументами
    strcpy(run_cmd, "./");
    strcat(run_cmd, executable);
    
    for (int i = 0; i < arg_count; i++) {
        strcat(run_cmd, " ");
        strcat(run_cmd, arguments[i]);
    }
    
    system(run_cmd);
    
    printf("================================\n");
    printf("Программа завершена.\n");
}

// Функция для компиляции и запуска лабы без запроса аргументов (для всех лаб сразу)
void compile_and_run_auto(LabWork lab, char *default_args) {
    char executable[100];
    char compile_cmd[200];
    char run_cmd[500];
    
    // Создаем имя исполняемого файла (без расширения .c)
    strcpy(executable, lab.filename);
    char *dot = strrchr(executable, '.');
    if (dot) *dot = '\0';
    
    printf("\nКомпиляция %s... ", lab.filename);
    
    // Формируем команду компиляции
    snprintf(compile_cmd, sizeof(compile_cmd), 
             "gcc -o %s %s -lm 2>/dev/null", executable, lab.filename);
    
    // Компилируем
    int compile_result = system(compile_cmd);
    
    if (compile_result != 0) {
        printf("Ошибка!\n");
        return;
    }
    
    printf("Успешно\n");
    printf("Запуск %s...\n", lab.name);
    printf("--------------------------------\n");
    
    // Формируем команду запуска
    strcpy(run_cmd, "./");
    strcat(run_cmd, executable);
    
    // Если есть аргументы по умолчанию, добавляем их
    if (default_args != NULL && strlen(default_args) > 0 && lab.needs_args) {
        strcat(run_cmd, " ");
        strcat(run_cmd, default_args);
    }
    
    system(run_cmd);
    
    printf("--------------------------------\n");
}

void print_menu(LabWork labs[], int count) {
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║      МЕНЕДЖЕР ЛАБОРАТОРНЫХ РАБОТ         ║\n");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║ Доступные лабораторные работы:           ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    
    printf("┌─────┬─────────────────┬─────────────┬────────────┐\n");
    printf("│ №   │ Название        │ Файл        │ Аргументы  │\n");
    printf("├─────┼─────────────────┼─────────────┼────────────┤\n");
    
    for (int i = 0; i < count; i++) {
        char args_indicator[12];
        if (labs[i].needs_args) {
            strcpy(args_indicator, "Нужны");
        } else {
            strcpy(args_indicator, "Опц.");
        }
        
        printf("│ %-3d │ %-15s │ %-11s │ %-10s │\n", 
               labs[i].number, labs[i].name, labs[i].filename, args_indicator);
    }
    
    printf("└─────┴─────────────────┴─────────────┴────────────┘\n");
    printf("\nКоманды:\n");
    printf("  <номер>  - Запустить лабораторную работу\n");
    printf("  a        - Запустить все лабораторные работы\n");
    printf("  c        - Проверить все файлы на компилируемость\n");
    printf("  q        - Выход\n");
    printf("\nВыберите действие: ");
}

// Функция для проверки компилируемости всех файлов
void check_all_compilation(LabWork labs[], int count) {
    printf("\n=== Проверка компилируемости всех файлов ===\n");
    
    for (int i = 0; i < count; i++) {
        printf("\nПроверка %s (%s):\n", labs[i].filename, labs[i].name);
        
        char compile_cmd[200];
        snprintf(compile_cmd, sizeof(compile_cmd),
                 "gcc -c %s -o /tmp/test.o 2>&1", labs[i].filename);
        
        FILE *pipe = popen(compile_cmd, "r");
        if (pipe) {
            char buffer[256];
            int has_errors = 0;
            int has_warnings = 0;
            
            printf("Результат: ");
            while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
                if (strstr(buffer, "error:") || strstr(buffer, "Error:")) {
                    has_errors = 1;
                }
                if (strstr(buffer, "warning:")) {
                    has_warnings = 1;
                }
            }
            
            pclose(pipe);
            
            if (has_errors) {
                printf("✗ Ошибки компиляции\n");
            } else if (has_warnings) {
                printf("⚠ Компилируется с предупреждениями\n");
            } else {
                printf("✓ Компилируется успешно\n");
                // Удаляем временный файл
                system("rm -f /tmp/test.o");
            }
        }
    }
    printf("\n=== Проверка завершена ===\n");
}

// Функция для запуска всех лабораторных работ
void run_all_labs(LabWork labs[], int count) {
    printf("\n=== Запуск всех лабораторных работ ===\n");
    printf("Для лаб, требующих аргументы, можно задать аргументы по умолчанию.\n");
    printf("Введите аргументы по умолчанию (или нажмите Enter для пропуска): ");
    
    char default_args[100];
    if (fgets(default_args, sizeof(default_args), stdin) != NULL) {
        default_args[strcspn(default_args, "\n")] = 0;
    }
    
    for (int i = 0; i < count; i++) {
        printf("\n╔══════════════════════════════════════════════════╗\n");
        printf("║ Лабораторная %d/%d: %-25s ║\n", 
               i + 1, count, labs[i].name);
        printf("╚══════════════════════════════════════════════════╝\n");
        
        compile_and_run_auto(labs[i], 
                           (labs[i].needs_args && strlen(default_args) > 0) ? default_args : NULL);
        
        if (i < count - 1) {
            printf("\nНажмите Enter для продолжения...");
            getchar();
        }
    }
}

int main() {
    LabWork labs[MAX_LABS];
    int lab_count = scan_labs(labs);
    
    if (lab_count == 0) {
        printf("Не найдено файлов лабораторных работ!\n");
        printf("Убедитесь, что файлы находятся в текущей директории.\n");
        printf("Ожидаемые файлы: alg1.c, alg2.c, algi3.c, algi4.c, algi52.c\n");
        return 1;
    }
    
    printf("Найдено лабораторных работ: %d\n", lab_count);
    
    char input[100];
    int running = 1;
    
    while (running) {
        print_menu(labs, lab_count);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        // Убираем символ новой строки
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0) {
            printf("Выход из программы.\n");
            running = 0;
        } 
        else if (strcmp(input, "a") == 0 || strcmp(input, "A") == 0) {
            run_all_labs(labs, lab_count);
        }
        else if (strcmp(input, "c") == 0 || strcmp(input, "C") == 0) {
            check_all_compilation(labs, lab_count);
        }
        else {
            // Проверяем, является ли ввод числом
            int choice = atoi(input);
            if (choice >= 1 && choice <= lab_count) {
                compile_and_run(labs[choice - 1]);
            } else {
                printf("Неверный выбор. Попробуйте снова.\n");
            }
        }
        
        if (running) {
            printf("\nНажмите Enter для возврата в меню...");
            getchar();
        }
    }
    
    // Очистка: удаление скомпилированных файлов
    printf("\nУдаление скомпилированных файлов...\n");
    for (int i = 0; i < lab_count; i++) {
        char executable[100];
        strcpy(executable, labs[i].filename);
        char *dot = strrchr(executable, '.');
        if (dot) *dot = '\0';
        
        char rm_cmd[100];
        snprintf(rm_cmd, sizeof(rm_cmd), "rm -f %s 2>/dev/null", executable);
        system(rm_cmd);
    }
    
    return 0;
}
