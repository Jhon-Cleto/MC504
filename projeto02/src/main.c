#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define n 16
#define C 5

typedef unsigned int ui;

typedef struct passenger_data
{
    ui pid;
    ui tickets;
} p_data;

ui boarders, unboarders, free_tickets;
sem_t mutex1, mutex2;
sem_t boardQueue, unboardQueue;
sem_t allAboard, allAshore;

pthread_mutex_t lr_mutex;
pthread_cond_t lr_cond;
int last_ride = 0;

void *car(void *arg)
{
    ui rides = (n % C) == 0 ? n/C : n/C + 1;

    while(rides--)
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

        // Avisar aos passageiros sobre a última corrida
        if (rides == 1)
        {
            pthread_mutex_lock(&lr_mutex);
            printf("O carro está pronto para a última viagem.\n");
            pthread_cond_broadcast(&lr_cond);
            last_ride = 1;
            pthread_mutex_unlock(&lr_mutex);
        }

        sem_wait(&allAshore);
    }

    return NULL;
}

void *passenger(void *p)
{
    p_data p_info = *((p_data*) p);
    ui id = p_info.pid;
    ui tickets = p_info.tickets;

    while (tickets--)
    {
        sleep(rand()%5); // delay na chegada dos passageiros
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

        pthread_mutex_lock(&lr_mutex);
        if (!last_ride)
            pthread_cond_wait(&lr_cond, &lr_mutex);

        if (free_tickets)
        {
            free_tickets--;
            tickets++;
            printf("O passageiro %d conseguiu um novo ticket para embarcar.\n", id);
        }
        pthread_mutex_unlock(&lr_mutex);
    }

    // printf("O passageiro %d não vai embarcar novamente.\n", id);
    return NULL;
}

int main(int argc, char *argv[])
{
    ui id_psgr[n];
    pthread_t thr_psgrs[n], thr_car;
    p_data p_infos[n];

    sem_init(&mutex1, 0, 1);
    sem_init(&mutex2, 0, 1);
    sem_init(&boardQueue, 0, 0);
    sem_init(&unboardQueue, 0, 0);
    sem_init(&allAboard, 0, 0);
    sem_init(&allAshore, 0, 0);

    free_tickets = (n % C) != 0 ? C - (n % C) : 0;

    for (ui i = 0; i < n; i++)
    {
        p_infos[i].pid = i;
        p_infos[i].tickets = 1;
        id_psgr[i] = i;
        pthread_create(&thr_psgrs[i], NULL, passenger, (void *)&p_infos[i]);
    }

    pthread_create(&thr_car, NULL, car, NULL);

    for (ui i = 0; i < n; i++)
        pthread_join(thr_psgrs[i], NULL);

    pthread_join(thr_car, NULL);

    return 0;
}
