#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

//Atualiza tela
void printScreen(){
  system("clear"); 
  for (int i=0; i<HEIGHT; i++ )
      for (int j=0; j<WIDTH; j++ ){
          printf ("%c", screen[i][j]);
        }
}

//Move o carrinho
void movieCar(int lineStart, int lengthCar, int columnStart, int columnEnd){
  int i, j, k;
  for(k=0;k<HEIGHT;k++){
    for(j=columnStart; j<=columnEnd; j++){
      for(i=0;i<lengthCar;i++)
        screen[((lineStart+k-i+1)%(HEIGHT+1))][j]= screen[((lineStart+k-i)%(HEIGHT+1))][j]; 
    }
   usleep(200000);    //Define framehate
   printScreen();
  }
 
}

int main(){	
  
  printScreen();
  movieCar(22, 7, 21, 23);

  return 0;
}