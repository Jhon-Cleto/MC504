# Projeto 1 - Trabalhando com Threads em C

## Integrantes
+ Cristiano Sampaio Pinheiro, RA 256352
+ George Gigilas Junior, RA 216741
+ Jhonatan Cléto, RA 256444
+ Mylena Roberta dos Santos, RA 222687

## Tema
Visando o desenvolvimento de uma aplicação utilizando pthreads sem a necessidade do uso de mecanismos de sincronização, implementamos um programa que realiza a multiplicação de duas matrizes. O esforço foi dividido de modo que cada thread calcula os valores de uma linha da matriz resultante.

## Arquivos
### Diretório bin
Armazena os arquivos binários do projeto.

### Diretório src
Armazena os códigos fontes do projeto. São eles:
+ **Makefile**: arquivo para compilar o programa.
+ **main.c**: código que realiza a multiplicação de matrizes. Dadas as dimensões de duas matrizes e uma seed, o programa imprime as matrizes de entrada e a matriz resultante.
+ **main_demo.c**: código que realiza a multiplicação de matrizes e imprime os resultados. Dadas as dimensões de duas matrizes e uma seed, o programa imprime as matrizes de entrada e a matriz resultante. A última é impressa com uma animação que demonstra passo-a-passo o que cada thread está executando.
### Diretório tests
Armazena os casos de teste, as saídas esperadas para cada caso e um script que executa o programa e compara a saída obtida com a saída esperada.
## Structs
### Matrix
Possui um apontador para inteiros (que aponta para a matriz em si) e duas variáveis do tipo unsigned long long: para guardar o número de linhas e de colunas da matriz.

Sua finalidade é de facilitar a passagem de matrizes por parâmetros nas funções.

### Thread_data
Possui uma variável do tipo unsigned long long que guarda a linha da matriz resultado pela qual a thread é responsável.

Sua finalidade é de condensar, em uma única estrutura, os parâmetros necessários a serem passados para a função que será executada pelas threads.

## Funções
### Allocate_matrix
+ **Parâmetros**: número de linhas e de colunas da matriz a ser criada, respectivamente.
+ **Retorno**: apontador para matrix.
+ **Funcionamento**: essa função é responsável por alocar a memória para uma estrutura do tipo matrix.
### Deallocate_matrix
+ **Parâmetros**: apontador para matrix.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por liberar a memória alocada para uma estrutura do tipo matrix.
### Initialize_matrices
+ **Parâmetros**: número de linhas da matriz A, número de colunas da matriz A, número de linhas da matriz B, número de colunas da matri B e uma seed para gerar números pseudo-aleatórios, respectivamente.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por inicializar as matrizes de entrada e a matriz resultante. Para isso, ela chama a função allocate_matrix internamente e preenche os elementos das matrizes de entrada com números pseudo-aleatórios gerados a partir de uma seed. Além disso, ela inicializa todas os elementos da matriz resultante como sendo 0.
### Print_matrix
+ **Parâmetros**: apontador para matrix.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela impressão no terminal de uma matriz.
### Multiply
+ **Parâmetros**: apontador para void (é esperado que seja um apontador para thread_data).
+ **Retorno**: apontador para void.
+ **Funcionamento**: essa função é responsável pelos cálculos da multiplicação das matrizes de entrada e pela atualização dos elementos da matriz resultante. No código, decidimos criar uma thread para cada linha da matriz resultante, e essa é a função pelas quais essas threads são responsáveis.
### Show_result_matrix
+ **Parâmetros**: apontador para void.
+ **Retorno**: apontador para void.
+ **Funcionamento**: essa função é responsável pela impressão da animação que demonstra passo-a-passo o preenchimento da matriz resultante pelas threads. Essa função é de responsabilidade total de uma thread diferente das que realizam a multiplicação.
