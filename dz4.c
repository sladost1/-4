#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h> 

int n;

void print(FILE *file, int** g) {               // Функция изображения графа
    bool b = 0;
    fprintf(file, "graph table {\n");

    for (int i = 0; i < n; i++) {
        b = 1;
        for (int j = 0; j <= i; j++)
        if (g[i][j] > 0) {
            b = 0;
            for (int k = 0; k < g[i][j]; k++)
                fprintf(file, "%c -- %c;\n", (char)(65 + i), (char)(65 + j));
        }
        if (b) fprintf(file, "%c\n", (char)(65 + i));
    }

    fprintf(file, "}");
}

void insert2(int arr[], int step) {
    for (int j = 0; j < step; j++) {
        for (int i = j; i < n; i += step)
        {
            int k = i;
            while (k >= step && arr[k - step] > arr[k])
            {
                int per = arr[k - step];
                arr[k - step] = arr[k];
                arr[k] = per;
                k -= step;
            }
        }
    }
}

void Shell(int arr[]) {
    int step = n / 2;
    while (step > 0) {
        insert2(arr, step);
        step /= 2;
    }
}

int main(void) {
    int** g;                                    // Массив для матрицы смежности

    if (scanf("%d", &n) == 0)
        return -1;

    g = (int**)malloc(n * sizeof(int*));        // Выделение памяти под указатели на строки
    for (int i = 0; i < n; i++) {               // Ввод элементов массива (цикл по строкам)

    g[i] = (int*)malloc(n * sizeof(int));       // Выделение памяти под хранение строк
        for (int j = 0; j < n; j++) {           // Ввод элементов массива (цикл по столбцам)
            printf("a[%d][%d] = ", i, j);
            if (!scanf("%d", &g[i][j])) {
                free(g);
                return -1;
            }
        }
    }

    for (int i = 0; i < n; i++) {               // Проверка на корретность матрицы
        for (int j = 0; j <= i; j++) {
            if (g[i][j] != g[j][i]) {
                printf("Некорретная матрица\n");
                free(g);
                return -1;
            }
        }
    }

    FILE* file = fopen("f.txt", "w");           //Открытие файла
    print(file, g);
    fclose(file);

    system ("dot -Tpng f.txt -o f.png");
    system ("wslview f.png");

    int t1 = -1, t2 = -1;
    printf("Между какими вершинами удалить ребро: ");
    if ((scanf("%d %d", &t1, &t2) != 2) || (t1 > n) || (t2 > n)) {
        printf("Некорректные данные\n");
        free(g);
        return -1;
    }
    printf("Выбрано ребро между %d и %d\n", t1, t2);

    if (g[t1][t2] == 1) {
        g[t1][t2] = 0;
        g[t2][t1] = 0;
    } else if (g[t1][t2] > 1) {
        int num = 0;
        printf("Данные вершины имеют несколько связей в количестве: %d\nСколько из них вы хотите удалить? ", g[t1][t2]);
        if (scanf("%d", &num) == 0) {
            free(g);
            return -1;
        }
        if (num > g[t1][t2] || num < 1)
            printf("Некорректное значение\n");
        else {
            g[t1][t2] = g[t1][t2] - num;
            g[t2][t1] = g[t1][t2];
        }
    } else
        printf("Нет ребра между выбранными вершинами\n");

    file = fopen("f.txt", "w");                 //Открытие файла
    print(file, g);
    fclose(file);

    system ("dot -Tpng f.txt -o f.png");
    system ("wslview f.png");

    int top1[n];                                // Массив для сортировки степеней вершин
    int top2[n];                                // Массив для сортировки степеней вершин

    for (int i = 0; i < n; i++) {
        top1[i] = 0;
        top2[i] = 0;
    }

    for (int i = 0; i < n; i++) {               // Заполнение массива для сортировки степеней вершин
        for (int j = 0; j < n; j++) {
            if (g[i][j] > 0) {
                if (i == j)
                    top1[i] += 2 * g[i][j];
                else
                    top1[i] += g[i][j];
            }
        }
    }

    for (int i = 0; i < n; i++) {                    // Копирование массива
        top2[i] = top1[i];
    }

    Shell(top1);
    for (int i = n - 1; i >= 0; i--) {               // Сопоставление отсортированных значений и вершин
        for (int j = 0; j <  n; j++) {
            if ((top1[i] == top2[j]) && (top2[j] != -1)) {
                printf("%c --- степень= %d\n", (char)(65 + j), top1[i]);
                top2[j] = -1;
            }
        }
    }

    free(g);
    return 0;
}
