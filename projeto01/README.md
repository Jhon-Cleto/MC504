# Projeto 01 - Aplicação Multithread

## Integrantes
+ Cristiano Sampaio Pinheiro, RA 256352
+ George Gigilas Junior, RA 216741
+ Jhonatan Cléto, RA 256444
+ Mylena Roberta dos Santos, RA 222687


## Apresentação
[Arquivo PDF](slides/projeto01-presentation.pdf)  
[Google Apresentações](https://docs.google.com/presentation/d/1a8jA22gtdZ8qHbpCR8TWgbk6Ggq_6HVi/edit?usp=sharing&ouid=105323096868420923292&rtpof=true&sd=true)


## Tema
Visando o desenvolvimento de uma aplicação utilizando *pthreads* sem a necessidade do uso mecanismos de sincronização, implementamos um programa que realiza a multiplicação de duas matrizes e calcula a matriz resultante. O processamento foi dividido de modo que cada *thread* calcula os valores dos elementos de uma dada linha da matriz resultante.


## Arquivos
### Diretório *bin*
Armazena os arquivos binários do projeto.

### Diretório *src*
Armazena os códigos fontes do projeto. São eles:
+ `Makefile`: arquivo para compilar o programa.
+ `main.c`: código que realiza a multiplicação de matrizes e imprime os resultados. Dadas as dimensões de duas matrizes e uma *seed*, o programa imprime as matrizes de entrada e a matriz resultante.
+ `main_demo.c`: código que realiza a multiplicação de matrizes e imprime os resultados. Dadas as dimensões de duas matrizes e uma *seed*, o programa imprime as matrizes de entrada e a matriz resultante. A última é impressa com uma animação que demonstra passo-a-passo a execução das *threads*.

### Diretório *tests*
Armazena os casos de teste (arquivos `.in`), as saídas esperadas para cada um (arquivos `.out`) e um *script* (`runTests.sh`) que executa o programa e compara as saídas obtidas com aquelas previstas.


## Estruturas de Dados
### **struct *matrix***
Possui um apontador para inteiros, que aponta para a matriz em si, e duas variáveis do tipo *unsigned long long*, uma para guardar o número de linhas e outra para o número de colunas da matriz.  
Sua finalidade é de facilitar a passagem das matrizes por parâmetros das funções contidas no código.

### **struct *thread_data***
Possui uma variável do tipo *unsigned long long* que guarda o índice da linha da matriz resultado pela qual a thread é responsável.  
Sua finalidade é de condensar, em uma única estrutura, o parâmetro necessário a ser passado para a função que será executada pelas *threads*.


## Funções
### ***allocate_matrix***
+ **Parâmetros**: número de linhas e de colunas da matriz a ser criada, respectivamente.
+ **Retorno**: apontador para `matrix`.
+ **Funcionamento**: essa função é responsável por alocar a memória para uma estrutura do tipo `matrix`.

### ***deallocate_matrix***
+ **Parâmetros**: apontador para `matrix`.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por liberar a memória alocada para uma estrutura do tipo `matrix`.

### ***initialize_matrices***
+ **Parâmetros**: número de linhas da matriz A, número de colunas da matriz A, número de linhas da matriz B, número de colunas da matriz B e uma *seed* para gerar números pseudo-aleatórios, respectivamente.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por inicializar as matrizes de entrada e a matriz resultante. Para isso, ela chama a função `allocate_matrix` internamente e preenche os elementos das matrizes de entrada com números pseudo-aleatórios gerados a partir de uma *seed*. Além disso, ela inicializa todos os elementos da matriz resultante com 0.

### ***print_matrix***
+ **Parâmetros**: apontador para `matrix`.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela impressão de uma matriz no terminal.

### ***multiply***
+ **Parâmetros**: apontador para `void` (é esperado que seja um apontador para `thread_data`).
+ **Retorno**: apontador para `void`.
+ **Funcionamento**: essa função é responsável pelos cálculos da multiplicação das matrizes de entrada e pela atualização dos elementos da matriz resultante. No código, decidimos criar uma *thread* para cada linha da matriz resultante, e essa é a função executada por essas *threads*.

### ***show_result_matrix***
+ **Parâmetros**: apontador para `void`.
+ **Retorno**: apontador para `void`.
+ **Funcionamento**: essa função é responsável pela impressão da animação que demonstra passo-a-passo o preenchimento da matriz resultante pelas *threads*. Essa função é de responsabilidade total de uma *thread* diferente das que realizam a multiplicação e está contida exclusivamente em `main_demo.c`.
