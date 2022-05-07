#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "animation.h"

//Define tamanho da tela
#define HEIGHT 38
#define WIDTH 47

//Estado inicial da tela
char screen[HEIGHT][WIDTH]= {
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "~~~~~~~~~~~~~~~~~~ |     | ~~~~~~~~~~~~~~~~~~\n",
  "~                ~ |     | ~                ~\n",
  "~                ~ |     | ~                ~\n",
  "~                ~ |     | ~                ~\n",
  "~                D |     | ~                ~\n",
  "~                E |     | E                ~\n",
  "~                S |     | M                ~\n",
  "~                E | [+] | B                ~\n",
  "~                M | [+] | A                ~\n",
  "~                B | [+] | R                ~\n",
  "~                A | [+] | Q                ~\n",
  "~                R | [+] | U                ~\n",
  "~                Q | \\#/ | E                ~\n",
  "~                U |     | ~                ~\n",
  "~                E |     | ~                ~\n",
  "~                ~ |     | ~                ~\n",
  "~                ~ |     | ~                ~\n",
  "~~~~~~~~~~~~~~~~~~ |     | ~~~~~~~~~~~~~~~~~~\n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
  "                   |     |                   \n",
};	

//Variaveis usadas para posições
int boardingPositionX = 29;
int boardingPositionY = 12; 
int unboardPositionX = 3;
int unboardPositionY = 12;

//Atualiza tela
void printScreen(){
  system("clear"); 
  for (int i=0; i<HEIGHT; i++ )
      for (int j=0; j<WIDTH; j++ ){
          printf ("%c", screen[i][j]);
        }
  usleep(100000);
}

//Adiciona passageiros na area de embarque 
void addPassenger(){
  screen[boardingPositionY][boardingPositionX] = 'P';
  if(boardingPositionX>41){
    boardingPositionY+=1;
    boardingPositionX=29;
  }
  else{
    boardingPositionX+=1;    
  }
}

// Inicia animação
void start_animation(int numberOfPassengers){
  for(int i=0;i<numberOfPassengers;i++){
    addPassenger();
  }
  printScreen();
}

//Embarca passageiros
void boarding_scene(){
  if(boardingPositionX<30){
    boardingPositionY-=1;
    boardingPositionX=42;
  }
  else{
    boardingPositionX-=1;
  }
  screen[boardingPositionY][boardingPositionX] = ' ';
  printScreen();
}

//Move o carrinho
void move_car_scene(){
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

//Desembarca passageiros
void unboarding_scene(){
  screen[unboardPositionY][unboardPositionX] = 'P';
  if(unboardPositionX>14){
    unboardPositionY+=1;
    unboardPositionX=3;
  }
  else{
    unboardPositionX+=1;
  }
  printScreen();
}

//Retirar passageiros do desembarque e adiciona no embarque
void new_boarding_scene(){
  if(unboardPositionX<3){
    unboardPositionY-=1;
    unboardPositionX=14;
  }
  else{
    unboardPositionX-=1;
  }
  screen[unboardPositionY][unboardPositionX] = ' ';
  addPassenger();
}