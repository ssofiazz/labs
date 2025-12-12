#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#define INF INT_MAX
#define V 9 

struct Node {
    int vertex;
    int weight;       
    struct Node* next; 
};

struct Graph {
    struct Node** adjList;  // Массив списков смежности
};

struct Path {
    int* vertices;
    int length;
    int totalWeight;
};

struct Node* createNode(int vertex, int weight) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->vertex = vertex;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Создание пустого графа
struct Graph* createEmptyGraph() {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    
    // Выделяем память для массива списков
    graph->adjList = (struct Node**)malloc(V * sizeof(struct Node*));
    
    // Инициализируем все списки как пустые
    for (int i = 0; i < V; i++) {
        graph->adjList[i] = NULL;
    }
    
    return graph;
}

// Добавление ребра в граф (ориентированный граф)
void addEdge(struct Graph* graph, int src, int dest, int weight) {
    // Создаем новый узел для вершины назначения
    struct Node* newNode = createNode(dest, weight);
    
    // Добавляем в начало списка для вершины-источника
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;
}

// Автоматическое чтение графа из файла graph_data.txt
struct Graph* loadGraphFromFile() {
    const char* filename = "graph_data.txt";
    FILE* file = fopen(filename, "r");
    
    if (!file) {
        printf("ОШИБКА: Файл %s не найден!\n", filename);
        printf("Создайте файл graph_data.txt с данными графа.\n");
        return NULL;
    }
    
    struct Graph* graph = createEmptyGraph();
 
    char line[256];
    int edges_count = 0;
    
    while (fgets(line, sizeof(line), file)) {
        // Пропускаем пустые строки и комментарии
        if (line[0] == '\n' || line[0] == '#' || line[0] == '\r') {
            continue;
        }

        // Парсим строку
        char* token = strtok(line, " \t");
        int tokens[3];
        int count = 0;
        
        while (token != NULL && count < 3) {
            tokens[count] = atoi(token);
            token = strtok(NULL, " \t");
            count++;
        }
        
        if (count == 3) {
            // Формат: исходная_вершина целевая_вершина вес
            int src = tokens[0] - 1;  // Преобразуем в 0-based индекс
            int dest = tokens[1] - 1;
            int weight = tokens[2];
            
            if (src >= 0 && src < V && dest >= 0 && dest < V) {
                if (weight > 0) {  // Положительный вес - добавляем ребро
                    addEdge(graph, src, dest, weight);
                    edges_count++;
                } else if (weight == 0) {
                    // Вес 0 может быть (ребро нулевой стоимости)
                    addEdge(graph, src, dest, 0);
                    edges_count++;
                }
                // weight < 0 игнорируем (отсутствие ребра)
            } else {
                printf("Предупреждение: неверные номера вершин в строке: %s\n", line);
            }
        }
    }
    
    fclose(file);
            
    return graph;
}

// Преобразование списка смежности в матрицу для алгоритмов
int** convertToMatrix(struct Graph* graph) {
    int** matrix = (int**)malloc(V * sizeof(int*));
    
    for (int i = 0; i < V; i++) {
        matrix[i] = (int*)malloc(V * sizeof(int));
        
        // Инициализируем всю матрицу как INF
        for (int j = 0; j < V; j++) {
            if (i == j)
                matrix[i][j] = 0;
            else
                matrix[i][j] = INF;
        }
        
        // Заполняем из списка смежности
        struct Node* current = graph->adjList[i];
        while (current != NULL) {
            matrix[i][current->vertex] = current->weight;
            current = current->next;
        }
    }
    
    return matrix;
}

// Вывод матрицы смежности
void printMatrix(int** matrix) {
    printf("\n             МАТРИЦА СМЕЖНОСТИ \n");
    printf("    ");
    for (int i = 0; i < V; i++) {
        printf("%3d ", i + 1);
    }
    printf("\n");
    
    for (int i = 0; i < V + 1; i++) {
        printf("----");
    }
    printf("\n");
    
    for (int i = 0; i < V; i++) {
        printf("%2d |", i + 1);
        for (int j = 0; j < V; j++) {
            if (matrix[i][j] == INF)
                printf(" INF");
            else
                printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

// АЛГОРИТМ ДЕЙКСТРЫ для списка смежности

// Нахождение вершины с минимальным расстоянием
int minDistance(int dist[], bool visited[]) {
    int min = INF, min_index = -1;
    
    for (int v = 0; v < V; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    
    return min_index;
}

// Восстановление пути из массива предков
struct Path* reconstructPath(int parent[], int start, int end, int dist) {
    int length = 0;
    int current = end;
    
    while (current != -1) {
        length++;
        current = parent[current];
    }
    
    struct Path* path = (struct Path*)malloc(sizeof(struct Path));
    path->vertices = (int*)malloc(length * sizeof(int));
    path->length = length;
    path->totalWeight = dist;
    
    current = end;
    for (int i = length - 1; i >= 0; i--) {
        path->vertices[i] = current + 1;
        current = parent[current];
    }
    
    return path;
}

// Вывод пути на экран
void printPath(struct Path* path, int start, int end) {
    printf("Длина пути: %d\n", path->totalWeight);
    
    for (int i = 0; i < path->length; i++) {
        printf("%d", path->vertices[i]);
        if (i < path->length - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
    
    free(path->vertices);
    free(path);
}

// Алгоритм Дейкстры для списка смежности
void dijkstraList(struct Graph* graph, int start, int end) {
    int start_idx = start - 1;
    int end_idx = end - 1;
    
    int dist[V];
    bool visited[V];
    int parent[V];
    
    // Инициализация
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }
    
    dist[start_idx] = 0;
    
    // Основной цикл
    for (int count = 0; count < V - 1; count++) {
        // Выбираем вершину с минимальным расстоянием
        int u = minDistance(dist, visited);
        
        if (u == -1 || u == end_idx) break;
        
        // Помечаем вершину как посещенную
        visited[u] = true;
        
        // Обновляем расстояния до смежных вершин из списка смежности
        struct Node* current = graph->adjList[u];
        while (current != NULL) {
            int v = current->vertex;
            int weight = current->weight;
            
            if (!visited[v] && dist[u] != INF && 
                dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
            
            current = current->next;
        }
    }
    
    // Вывод результата
    printf("\n АЛГОРИТМ ДЕЙКСТРЫ \n");
    printf("Поиск пути из вершины %d в вершину %d\n", start, end);
    
    if (dist[end_idx] == INF) {
        printf("Путь из %d в %d не существует!\n", start, end);
    } else {
        struct Path* path = reconstructPath(parent, start_idx, end_idx, dist[end_idx]);
        printPath(path, start, end);
    }
}

// АЛГОРИТМ ФЛОЙДА-УОРШОЛЛА 

void floydWarshall(int** matrix, int start, int end) {
    int start_idx = start - 1;
    int end_idx = end - 1;
    
    // Создаем копию матрицы
    int** dist = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        dist[i] = (int*)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            dist[i][j] = matrix[i][j];
        }
    }
    
    // Матрица предков для восстановления пути
    int** next = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        next[i] = (int*)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            if (i == j || matrix[i][j] == INF)
                next[i][j] = -1;
            else
                next[i][j] = j;
        }
    }
    
    // Основной алгоритм Флойда - Уоршолла
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
    
    // Вывод результатов для пути из start в end
    printf("\n АЛГОРИТМ ФЛОЙДА-УОРШОЛЛА \n");
    printf("Поиск пути из вершины %d в вершину %d\n", start, end);
    
    if (dist[start_idx][end_idx] == INF) {
        printf("Путь из %d в %d не существует!\n", start, end);
    } else {
        printf("Кратчайшее расстояние: %d\n", dist[start_idx][end_idx]);
        
        // Восстановление и вывод пути
        if (next[start_idx][end_idx] == -1) {
            printf("Прямого пути не найдено!\n");
        } else {
            printf("%d", start);
            int u = start_idx;
            while (u != end_idx) {
                u = next[u][end_idx];
                printf(" -> %d", u + 1); 
            }
            printf("\n");
        }
    }
    
    // Освобождаем память
    for (int i = 0; i < V; i++) {
        free(dist[i]);
        free(next[i]);
    }
    free(dist);
    free(next);
}

// Освобождение памяти графа
void freeGraph(struct Graph* graph) {
    for (int i = 0; i < V; i++) {
        struct Node* current = graph->adjList[i];
        while (current != NULL) {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adjList);
    free(graph);
}

// Освобождение памяти матрицы
void freeMatrix(int** matrix) {
    for (int i = 0; i < V; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    printf("Вариант 7: путь из вершины 4 в вершину 1\n");
    
    struct Graph* graph = loadGraphFromFile();
    
    // Преобразуем в матрицу и выводим ее
    int** matrix = convertToMatrix(graph);
    printMatrix(matrix);
    
    int start_vertex = 4;
    int end_vertex = 1;
    
    dijkstraList(graph, start_vertex, end_vertex);
    floydWarshall(matrix, start_vertex, end_vertex);
    
    freeGraph(graph);
    freeMatrix(matrix);
    
    return 0;
}