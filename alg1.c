#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 8192
#define BLUE_COLOR "\033[1;34m"
#define RESET_COLOR "\033[0m"

const char *keywords[] = {
    "char", "const", "double", "float", "int", "long", "short",
    "signed", "static", "struct", "unsigned", "void", "enum", 
    "extern", "register", "restrict", "sizeof", "volatile", 
    "typedef", "union", "break", "continue", "for", "else", 
    "if", "do", "goto", "default", "return", "switch", "while", 
    NULL
};

int in_comment = 0;      
int in_string = 0;      

int is_keyword(const char* word) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(word, keywords[i]) == 0) return 1;
    }
    return 0;
}

int is_valid_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

int is_valid_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

void color_identifiers(const char* line) {
    int inSingleLineComment = 0;
    char token[512];
    int tokenIndex = 0;
    
    for (int i = 0; line[i] != '\0'; i++) {
        if (!in_comment && !in_string && line[i] == '/' && line[i + 1] == '/') {
            inSingleLineComment = 1;
        }
        
        if (!in_string && !inSingleLineComment) {
            if (line[i] == '/' && line[i + 1] == '*') {
                in_comment = 1;
                putchar(line[i]);
                i++;
                continue;
            }
            if (in_comment && line[i] == '*' && line[i + 1] == '/') {
                in_comment = 0;
                putchar(line[i]);
                i++;
                continue;
            }
        }
        
        if (!in_comment && !inSingleLineComment && line[i] == '\"' && 
            (i == 0 || line[i-1] != '\\')) {
            in_string = !in_string;
            putchar(line[i]);
            continue;
        }
        
        if (in_comment || in_string || inSingleLineComment) {
            putchar(line[i]);
            continue;
        }
        
        if (is_valid_identifier_start(line[i])) {
            tokenIndex = 0;
            token[tokenIndex++] = line[i];
            
            int j = i + 1;
            while (line[j] != '\0' && is_valid_identifier_char(line[j])) {
                if (tokenIndex < sizeof(token) - 1) {
                    token[tokenIndex++] = line[j];
                }
                j++;
            }
            
            token[tokenIndex] = '\0';
            
            if (is_keyword(token)) {
                printf("%s", token);
            } else {
                printf(BLUE_COLOR "%s" RESET_COLOR, token);
            }
            
            i = j - 1;
        } else {
            putchar(line[i]);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return 1;
    }

    in_comment = 0;
    in_string = 0;
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        color_identifiers(line);
    }
    
    fclose(file);
    return 0;
}