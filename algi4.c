#include <stdio.h>
#include <stdlib.h>

// структура узла дерева
struct Node {
    int key;            
    struct Node* left;  // левый 
    struct Node* right; // правый 
};

struct Trunk {
    struct Trunk* prev;
    char* str;
};

void showTrunks(struct Trunk* p) {
    if (p == NULL) {
        return;
    }
    showTrunks(p->prev);
    printf("%s", p->str);
}

void printTree(struct Node* root, struct Trunk* prev, int isLeft) {
    if (root == NULL) {
        return;
    }

    char* prev_str = "     ";
    struct Trunk* trunk = (struct Trunk*)malloc(sizeof(struct Trunk));
    trunk->prev = prev;
    trunk->str = prev_str;

    printTree(root->right, trunk, 1);

    if (!prev) {
        trunk->str = "-----";
    }
    else if (isLeft) {
        trunk->str = ".----";
        prev_str = "    |";
    }
    else {
        trunk->str = "`----";
        prev->str = prev_str;
    }

    showTrunks(trunk);
    printf(" %d\n", root->key);

    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "    |";

    printTree(root->left, trunk, 0);
    free(trunk);
}

// создание нового узла
struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode) {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }
    newNode->key = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// вставка в бинарное дерево поиска
struct Node* insert(struct Node* root, int value) {
    if (!root) {
        return createNode(value);
    }
    
    if (value < root->key) {
        root->left = insert(root->left, value);
    } else if (value > root->key) {
        root->right = insert(root->right, value);
    }
    
    return root;
}

// функция для подсчета количества узлов (не листьев)
int countNonLeafNodes(struct Node* node) {
    if (node == NULL) {
        return 0;
    }
    
    if (node->left == NULL && node->right == NULL) {      // узел является листом, если у него нет потомков
        return 0;
    }
        return 1 + countNonLeafNodes(node->left) + countNonLeafNodes(node->right);
}

// подсчет общего количества узлов
int countTotalNodes(struct Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countTotalNodes(root->left) + countTotalNodes(root->right);
}

void freeTree(struct Node* node) {
    if (node) {
        freeTree(node->left);
        freeTree(node->right);
        free(node);
    }
}

int main() {
    FILE* file = fopen("xxx.txt", "r");
    if (!file) {
        printf("Не удалось открыть файл\n");
        return 1;
    }
    
    struct Node* root = NULL;
    int value;
    int count = 0;
    
    printf("Числа из файла:\n");
    while (fscanf(file, "%d", &value) == 1) {
        printf("Вставляем: %d\n", value);
        root = insert(root, value);
        count++;
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("Файл не содержит чисел!\n");
        return 1;
    }
    
    printf("\nБинарное дерево поиска:\n");
    printTree(root, NULL, 0);
    
    int nonLeafCount = countNonLeafNodes(root);
    int totalCount = countTotalNodes(root);
    
    printf("\nСтатистика дерева:\n");
    printf("Количество узлов (не листьев) --- %d\n", nonLeafCount);
    printf("Общее количество узлов --- %d\n", totalCount);
    printf("Количество листьев --- %d\n", totalCount - nonLeafCount);
    
    freeTree(root);
    return 0;
}