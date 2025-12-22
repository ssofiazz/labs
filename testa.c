
#include <stdio.h>
#include <string.h>

int main() {
    int counter = 0;
    float total_sum = 0.0;
    char *message = "Hello";
    
    for(int i = 0; i < 5; i++) {
        counter++;
        total_sum += i * 2.5;
    }
    
    printf("%s: counter=%d, sum=%.2f\n", message, counter, total_sum);
    return 0;
}

// комментарий с int и float
int main() {
    int number = 42;
    float pi = 3.14;
    char message[] = "Hello World with int variable";
    
    /* Многострочный комментарий
       с упоминанием char и double */
    double big_number = 123.456;
    
    // проверка идентификаторов
    int my_var1 = 10;
    int _special_var = 20;
    int normalVar = 30;
    
    // строка с "кавычками и словом int внутри строки"
    char* text = "Это строка с int внутри";
    
    // проверка ключевых слов в разных контекстах
    if (number > 0) {
        for (int i = 0; i < 10; i++) {
            while (normalVar < 50) {
                normalVar++;
            }
        }
    }
    
    // Смешанный случай: комментарий // с int словом
    struct Point {
        int x;
        int y;
    };
    
    return 0;
}

/* Еще один многострочный
   комментарий в конце файла */
