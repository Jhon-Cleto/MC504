#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "animation.h"

#define CLEAN "\e[1;1H\e[2J"

typedef unsigned int ui;

typedef struct passenger_data
{
    ui pid;
    ui tickets;
} p_data;

int n, C;
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
    char log[100];
    memset(log, 0, sizeof(log));
    
    while(rides--)
    {
        // load();

        sprintf(log, "O carro está pronto o embarque.\n");
        update_log_message(log);
        
        for (ui i = 0; i < C; i++)
            sem_post(&boardQueue);

        sem_wait(&allAboard);

        // run();
        sprintf(log, "O carro está se movimentando.\n");
        update_log_message(log);
        move_car_scene();

        // unload();
        sprintf(log, "O carro está pronto o desembarque.\n");
        update_log_message(log);
        for (ui i = 0; i < C; i++)
            sem_post(&unboardQueue);

        // Avisar aos passageiros sobre a última corrida
        if (rides == 1)
        {
            pthread_mutex_lock(&lr_mutex);
            sprintf(log, "O carro está pronto para a última viagem.\n");
            update_log_message(log);
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
    char log[100];
    memset(log, 0, sizeof(log));

    while (tickets--)
    {
        sleep(rand()%5); // delay na chegada dos passageiros
        sem_wait(&boardQueue);
        
        // board();
        sem_wait(&mutex1);
        boarders++;
        sprintf(log, "O passageiro %d embarcou.\n", id);
        update_log_message(log);
        boarding_scene();
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
        sprintf(log, "O passageiro %d desembarcou.\n", id);
        update_log_message(log);
        unboarding_scene();
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
            sprintf(log, "O passageiro %d conseguiu um novo ticket para embarcar.\n", id);
            update_log_message(log);
            new_boarding_scene();
        }
        pthread_mutex_unlock(&lr_mutex);
    }

    return NULL;
}

int main(int argc, char *argv[])
{

    // Dados Iniciais
    printf(CLEAN);   
    printf("The Roller Coaster Problem\n");
    printf("Digite o número total de passageiros:\n");
    scanf(" %d", &n);
    printf("Digire o número de assentos no carrinho:\n");
    scanf(" %d", &C);

    if (n < C)
    {
        printf("O número de passageiros é insuficiente!\n");
        printf("A Montanha Russa não abrirá hoje!\n");
        return 0;
    }

    pthread_t thr_psgrs[n], thr_car;
    p_data p_infos[n];

    start_animation(n);


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
        pthread_create(&thr_psgrs[i], NULL, passenger, (void *)&p_infos[i]);
    }

    pthread_create(&thr_car, NULL, car, NULL);

    for (ui i = 0; i < n; i++)
        pthread_join(thr_psgrs[i], NULL);

    pthread_join(thr_car, NULL);

    return 0;
}
