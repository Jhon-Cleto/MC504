#ifndef ANIMATION_H
#define ANIMATION_H 1

#include <semaphore.h>

sem_t boardingArea_mutex;
sem_t unboardingArea_mutex;

// Iniciar o ambiente de animação
void start_animation();

// Animação para a cena se embarque do passageiro
void boarding_scene();

// Animação para a cena de desembarque do passageiro
void unboarding_scene();

// Animação para a cena de chegada do passageiro a zona de embarque
void arrival_scene();

#endif