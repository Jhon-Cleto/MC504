#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "animation.h"

//Define tamanho da tela
#define HEIGHT 38
#define S_HEIGHT 41
#define S_WIDTH 100

#define CLEAR "\e[1;1H\e[2J"

#define EMPTY_SYMBOL '+'
#define FILLED_SYMBOL 'P'
#define VOID_SYMBOL ' '

//Estado inicial da tela
char screen[S_HEIGHT][S_WIDTH]= {
  "                         |     |                         \n",
  "                         |     |                         \n",
  "                         |     |                         \n",
  "                         |     |                         \n",
  "                         |     |                         \n",
  "                         |     |                         \n",
  "                         |     |                         \n",
  "                         |     |                         \n",
  "                         |     |                         \n",
  "                         |     |                         \n",
  "      ~~~~~~~~~~~~~~~~~~ |     | ~~~~~~~~~~~~~~~~~~      \n",
  "      ~                ~ |     | ~                ~      \n",
  "      ~                ~ |     | ~                ~      \n",
  "      ~                ~ |     | ~                ~      \n",
  "      ~                D |     | ~                ~      \n",
  "      ~                E |     | E                ~      \n",
  "      ~                S |     | M                ~      \n",
  "      ~                E | [+] | B                ~      \n",
  "      ~                M | [+] | A                ~      \n",
  "      ~                B | [+] | R                ~      \n",
  "      ~                A | [+] | Q                ~      \n",
  "      ~                R | [+] | U                ~      \n",
  "      ~                Q | \\#/ | E                ~     \n",
  "      ~                U |     | ~                ~      \n",
  "      ~                E |     | ~                ~      \n",
  "      ~                ~ |     | ~                ~      \n",
  "      ~                ~ |     | ~                ~      \n",
  "      ~~~~~~~~~~~~~~~~~~ |     | ~~~~~~~~~~~~~~~~~~      \n",
  "                         |     |                         \n",
  "                         |     |       Dados:            \n",
  "                         |     |       n:                \n",
  "                         |     |       C:                \n",
  "                         |     |       Corrida:          \n",
  "                         |     |                         \n",
  "                         |     |                         \n",
  "                         |     |                         \n",
  "                         |     |                         \n",
  "                         |     |                         \n",
  "---------------------------------------------------------\n",
  "\n",
  "---------------------------------------------------------\n",
};	

//Variaveis usadas para posições
int boardingPositionX = 35;
int boardingPositionY = 12; 
int unboardPositionX = 9;
int unboardPositionY = 12;
int count = 0;
pthread_mutex_t log_lock, add_lock;

//Atualiza tela
void printScreen(){
  // system("clear"); 
  printf(CLEAR);
  for (int i = 0; i < S_HEIGHT; i++)
    printf("%s", screen[i]);

  usleep(100000);
}

void update_log_message(char message[])
{
  pthread_mutex_lock(&log_lock);
  int logPosition = 39;
  screen[logPosition][0] = 'L';
  screen[logPosition][1] = 'O';
  screen[logPosition][2] = 'G';
  screen[logPosition][3] = ':';
  screen[logPosition][4] = ' ';
  int cnt = 5;
  for (int i = 0; i < S_WIDTH-5; i++)
    screen[logPosition][cnt++] = message[i];
  pthread_mutex_unlock(&log_lock);
}

void update_ride_log(unsigned int ride)
{
  int ridePositionY = 32, ridePositionX = 48;

  char rs[2];
  sprintf(rs, "%02d", ride);
  int cnt = 0;
  for (int i = ridePositionX; i <= ridePositionX+1; i++)
    screen[ridePositionY][i] = rs[cnt++];

}

char intToChar(int number){
  char charNumber[2];
  sprintf(charNumber, "%d", number);
  return *charNumber;
}

void updateCount(){
  screen[17][28] = intToChar(count/10);
  screen[18][28] = intToChar(count%10);
  usleep(0.3e6);
}

//Adiciona passageiros na area de embarque 
void addPassenger(){
  pthread_mutex_lock(&add_lock);
  screen[boardingPositionY][boardingPositionX] = FILLED_SYMBOL;
  if(boardingPositionX>47){
    boardingPositionY+=1;
    boardingPositionX=35;
  }
  else{
    boardingPositionX+=1;    
  }
  pthread_mutex_unlock(&add_lock);
}

void arrival_scene(int pid)
{
  char log[100];
  memset(log, 0, sizeof(log));
  sprintf(log, "O passageiro %d entrou na zona de embarque.\n", pid);
  update_log_message(log);
  addPassenger();
}

// Inicia animação
void start_animation(int numberOfPassengers, int C) {
  
  int nPositionY = 30, cPositionY = 31;
  int xPosition = 42;

  char str[2];
  
  sprintf(str, "%02d", numberOfPassengers);
  int cnt = 0;
  for (int i = xPosition; i <= xPosition+1; i++)
    screen[nPositionY][i] = str[cnt++];  
  
  sprintf(str, "%02d", C);
  cnt = 0;
  for (int i = xPosition; i <= xPosition+1; i++)
    screen[cPositionY][i] = str[cnt++];   

  // for(int i=0;i<numberOfPassengers;i++){
  //   addPassenger();
  // }
  printScreen();

}

//Embarca passageiros
void boarding_scene(){
  if(boardingPositionX<36){
    boardingPositionY-=1;
    boardingPositionX=48;
  }
  else{
    boardingPositionX-=1;
  }
  screen[boardingPositionY][boardingPositionX] = VOID_SYMBOL;
  count++;
  updateCount();

  int carHead = 22, symCol = 28;

  
  for (int i = carHead-1; i >= carHead-5; i--)
  {
    if (screen[i][symCol] == EMPTY_SYMBOL)
    {
      usleep(0.3e6);
      screen[i][symCol] = FILLED_SYMBOL;
      break;
    }
    
  }
  
  printScreen();
}

//Move o carrinho
void old_move_car_scene(){
  int lineStart = 22;
  int lengthCar = 7;
  int columnStart = 21;
  int columnEnd = 23;
  int i, j, k; 
  for(k=0;k<HEIGHT+1;k++){
    for(j=columnStart; j<=columnEnd; j++){
      for(i=0;i<lengthCar;i++)
        screen[((lineStart+k-i+1)%(HEIGHT+1))][j]= screen[((lineStart+k-i)%(HEIGHT+1))][j]; 
    }
  //  usleep(100000);    //Define framehate
   printScreen();
  }
}

// Move o carrinho uma posição para baixo
void move_one_step(int carHead, int carTail)
{
  int columnStart = 27;
  int columnEnd = 29;

  for (int i = carHead; i >= carTail-1; i--)
  {
    for (int j = columnStart; j <= columnEnd; j++)
    {
      if(i < HEIGHT-1)
        screen[i+1][j] = screen[i][j];
    }
  }
}

// Move o carrinho uma posição para baixo
void move_one_step2(int carHead, int carTail)
{
  int columnStart = 27;

  for (int i = carHead; i >= 0 ; i--)
  {
    screen[i][columnStart] = '[';
    if(i==1){
      screen[i][columnStart+1] = intToChar(count/10);
    }
    else if(i==2){
      screen[i][columnStart+1] = intToChar(count%10);
    }
    else{
      screen[i][columnStart+1] = FILLED_SYMBOL;
    }
    screen[i][columnStart+2] = ']';
  }

  screen[carHead+1][columnStart] = '\\';
  screen[carHead+1][columnStart+1] = '#';
  screen[carHead+1][columnStart+2] = '/'; 

  if (carHead+1 == 6)
  {
    screen[0][columnStart] = VOID_SYMBOL;
    screen[0][columnStart+1] = VOID_SYMBOL;
    screen[0][columnStart+2] = VOID_SYMBOL;   
  }

}

// Move o Carro
void move_car_scene()
{
  int head_0 = 22, tail_0 = 17;

  // Carro saindo até desaparecer da tela
  int carHead = head_0, carTail = tail_0;
  int carLength = 6, carWidth = 3;

  while(carTail < HEIGHT)
  {
    move_one_step(carHead++, carTail++);
    printScreen();
  }

  // Carro viajando pela Montanha Russa
  sleep(2);

  // Carro aparece em cima até a posição de Desembarque
  carHead = -1;
  carTail = carHead-5;

  while (carHead < 6)
  {
    move_one_step2(carHead++, carTail++);
    printScreen();
  }
  
  while(carHead < head_0)
  {
    move_one_step(carHead++, carTail++);
    printScreen();
  }

}

//Desembarca passageiros
void unboarding_scene(){

  int carHead = 22, symCol = 28;

  for (int i = carHead-1; i >= carHead-5; i--)
  {
    if (screen[i][symCol] == FILLED_SYMBOL)
    {
      screen[i][symCol] = EMPTY_SYMBOL;
      usleep(0.3e6);
      break;
    }
  }

  screen[unboardPositionY][unboardPositionX] = FILLED_SYMBOL;
  count--;
  updateCount();
  if(unboardPositionX>20){
    unboardPositionY+=1;
    unboardPositionX=9;
  }

  else
    unboardPositionX+=1;

  printScreen();
}

//Retirar passageiros do desembarque e adiciona no embarque
void new_boarding_scene(){
  if(unboardPositionX<9){
    unboardPositionY-=1;
    unboardPositionX=20;
  }
  else{
    unboardPositionX-=1;
  }
  screen[unboardPositionY][unboardPositionX] = VOID_SYMBOL;
  addPassenger();
}