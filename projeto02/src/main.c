#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define n 10
#define C 5

typedef unsigned int ui;

ui boarders, unboarders;
sem_t mutex1, mutex2;
sem_t boardQueue, unboardQueue;
sem_t allAboard, allAshore;

void *car()
{
    int rounds = (int)ceil((double)n / (double)C);

    for (int i = 0; i < rounds; i++)
    {
        // load();
        printf("O carro está pronto o embarque.\n");

        for (ui i = 0; i < C; i++)
            sem_post(&boardQueue);

        sem_wait(&allAboard);

        // run();
        printf("O carro está se movimentando.\n");

        // unload();
        printf("O carro está pronto o desembarque.\n");

        for (ui i = 0; i < C; i++)
            sem_post(&unboardQueue);

        sem_wait(&allAshore);
    }

    return NULL;
}

void *passenger(void *p)
{
    ui id = *(ui *)p;

    sem_wait(&boardQueue);
    // board();

    sem_wait(&mutex1);
    boarders++;
    printf("O passageiro %d embarcou.\n", id);
    if (boarders == C)
    {
        sem_post(&allAboard);
        boarders = 0;
    }
    sem_post(&mutex1);

    sem_wait(&unboardQueue);
    // unboard();

    sem_wait(&mutex2);
    unboarders++;
    printf("O passageiro %d desembarcou.\n", id);
    if (unboarders == C)
    {
        sem_post(&allAshore);
        unboarders = 0;
    }
    sem_post(&mutex2);

    return NULL;
}

int main(int argc, char *argv[])
{
    ui id_psgr[n];
    pthread_t thr_psgrs[n], thr_car;

    sem_init(&mutex1, 0, 1);
    sem_init(&mutex2, 0, 1);
    sem_init(&boardQueue, 0, 0);
    sem_init(&unboardQueue, 0, 0);
    sem_init(&allAboard, 0, 0);
    sem_init(&allAshore, 0, 0);

    for (ui i = 0; i < n; i++)
    {
        id_psgr[i] = i;
        pthread_create(&thr_psgrs[i], NULL, passenger, (void *)&id_psgr[i]);
    }

    pthread_create(&thr_car, NULL, car, NULL);

    for (ui i = 0; i < n; i++)
        pthread_join(thr_psgrs[i], NULL);

    pthread_join(thr_car, NULL);

    return 0;
}
