#ifndef ANIMATION_H
#define ANIMATION_H 1

#include <semaphore.h>

// sem_t boardingArea_mutex;
// sem_t unboardingArea_mutex;

// Iniciar o ambiente de animação
void start_animation(int numberOfPassengers);

// Atualiza a tela
void printScreen();

// Animação para a cena se embarque do passageiro
void boarding_scene();

// Animação para iniciar o movimento do carrinho
void move_car_scene();

// Animação para a cena de desembarque do passageiro
void unboarding_scene();

// Animação para os passageiros retornarem para o embarque
void new_boarding_scene();

// Mostrar Mensagens de Log na Tela
void update_log_message(char message[]);

// // Animação para a cena de chegada do passageiro a zona de embarque
// void arrival_scene();

#endif