#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stack.h"

#define NUM_NODES 200
#define THREAD_COUNT 4

typedef struct thread
{
    long number;   // Número
    pthread_t tid; // ID da thread
} Thread;

Thread threads[THREAD_COUNT];                      // Vetor de threads
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Inicialização do mutex

clock_t start, end;   // Variáveis para medir o tempo de execução
double cpu_time_used; // Variável para armazenar o tempo de execução

char *color;                      // Variável para armazenar a cor do nó atual
long parent[NUM_NODES];           // Vetor de pais
long visited[NUM_NODES] = {0};    // Vetor de visitados
long graph[NUM_NODES][NUM_NODES]; // Matriz de adjacência

void createGraph()
{
    srand(time(NULL)); // Inicialização do gerador de números aleatórios

    printf("\nGraph is being created... "); // Criação da matriz de adjacência

    for (int i = 0; i < NUM_NODES; i++)
    {
        for (int j = 0; j < NUM_NODES; j++)
        {
            if (i != j) // Não pode haver laço
            {
                graph[i][j] = rand() % 2;  // 0 ou 1
                graph[j][i] = graph[i][j]; // Grafo não direcionado
            }
            else // Laço
            {
                graph[i][j] = 0; // 0
            }
        }
    }

    printf("DONE!\n\n");  // Impressão da matriz de adjacência
    printf("GRAPH:\n\n"); // Impressão da matriz de adjacência
}

void printGraph()
{
    for (long i = 0; i < NUM_NODES; i++) // Impressão da matriz de adjacência
    {
        printf("\033[35m%6ld\033[0m | ", i); // Impressão da matriz de adjacência
        for (long j = 0; j < NUM_NODES; j++) // Impressão da matriz de adjacência
            printf("%ld ", graph[i][j]);     // Impressão da matriz de adjacência
        printf("\n");                        // Impressão da matriz de adjacência
    }
}

void DFS(Stack *globalStack)
{
    long current = -1; // Nó atual
    while (1)          // Loop infinito
    {
        if (current = -1) // Se o nó atual for -1
        {
            pthread_mutex_lock(&mutex); // Lock do mutex

            if (stackIsEmpty(globalStack)) // Se a pilha estiver vazia
            {
                pthread_mutex_unlock(&mutex); // Unlock do mutex
                return;                       // Retorna
            }

            current = pop(globalStack); // Pop da pilha

            /*if (pthread_self() == threads[0].tid)      // Se a thread atual for a 0
                color = "\033[31m";                    // Vermelho
            else if (pthread_self() == threads[1].tid) // Se a thread atual for a 1
                color = "\033[32m";                    // Verde
            else if (pthread_self() == threads[2].tid) // Se a thread atual for a 2
                color = "\033[33m";                    // Amarelo
            else                                       // Se a thread atual for a 3
                color = "\033[34m";                    // Azul

            printf("%s%ld ", color, current); // Impressão do nó atual*/

            if (visited[current]) // Se o nó atual já foi visitado
            {
                pthread_mutex_unlock(&mutex); // Unlock do mutex
                continue;                     // Continua
            }

            visited[current] = 1; // Marca o nó atual como visitado

            pthread_mutex_unlock(&mutex); // Unlock do mutex
        }

        long index = 0;                      // Índice
        long local[NUM_NODES];               // Vetor local
        for (long i = 0; i < NUM_NODES; i++) // Loop para percorrer a linha da matriz de adjacência
        {
            if (graph[current][i] == 1) // Se houver aresta
            {
                pthread_mutex_lock(&mutex); // Lock do mutex
                if (visited[i])             // Se o nó já foi visitado
                {
                    pthread_mutex_unlock(&mutex); // Unlock do mutex
                    continue;                     // Continua
                }
                else // Se o nó ainda não foi visitado
                {
                    if (parent[i] < 0) // Se o nó ainda não tem pai
                    {
                        parent[i] = current; // O pai do nó é o nó atual
                        local[index++] = i;  // Adiciona o nó ao vetor local
                    }
                    pthread_mutex_unlock(&mutex); // Unlock do mutex
                }
            }
        }

        pthread_mutex_lock(&mutex); // Lock do mutex

        for (long i = index - 1; i >= 0; i--) // Loop para percorrer o vetor local
            push(globalStack, local[i]);      // Push do vetor local na pilha global

        if (stackIsEmpty(globalStack)) // Se a pilha estiver vazia
            current = -1;              // O nó atual é -1

        pthread_mutex_unlock(&mutex); // Unlock do mutex
    }
}

int main()
{
    createGraph(); // Criação da matriz de adjacência
    printGraph();  // Impressão da matriz de adjacência

    for (long i = 0; i < NUM_NODES; i++) // Inicialização do vetor de pais
        parent[i] = -1;                  // Inicialização do vetor de pais

    printf("\nStarting DFS using multithreading...\n\n"); // Início da busca em profundidade

    start = clock(); // Início da contagem do tempo de execução

    Stack *globalStack = createStack(); // Criação da pilha global
    push(globalStack, 0);               // Push do nó inicial na pilha global
    parent[0] = 0;                      // O pai do nó inicial é ele mesmo

    for (long i = 0; i < THREAD_COUNT; i++) // Criação das threads
    {
        threads[i].number = i;                                                   // Número da thread
        pthread_create(&threads[i].tid, NULL, (void *)DFS, (void *)globalStack); // Criação da thread
    }
    for (long i = 0; i < THREAD_COUNT; i++) // Join das threads
        pthread_join(threads[i].tid, NULL); // Join das threads

    end = clock(); // Fim da contagem do tempo de execução

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Tempo de execução

    printf("\033[0m\n\nTime elapsed = \033[35m%fs\n\n", cpu_time_used); // Impressão do tempo de execução

    destroyStack(&globalStack);    // Destruição da pilha global
    pthread_mutex_destroy(&mutex); // Destruição do mutex
    pthread_exit(NULL);            // Finalização das threads

    return 0;
}