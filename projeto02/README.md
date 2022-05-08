# Projeto 02 - Animação Multithread com Semáforos

## Integrantes
+ Cristiano Sampaio Pinheiro, RA 256352
+ George Gigilas Junior, RA 216741
+ Jhonatan Cléto, RA 256444
+ Mylena Roberta dos Santos, RA 222687


## Apresentação
[Arquivo PDF](slides/projeto02-presentation.pdf)  
[Google Apresentações](https://docs.google.com/presentation/d/1tLLXZlp9gshf_Fj9ah5q-S_e5KJBi6ZH/edit?usp=sharing&ouid=107360421335949692714&rtpof=true&sd=true)


## Problema: The roller coaster problem (adaptado)
Esse problema é uma versão modificada do problema homônimo contido em “The Little Book of Semaphores”, de Allen B. Downey.

Suponha que existam n threads de passageiros e uma thread de carro. Os passageiros esperam repetidamente para andar no carro, que comporta C passageiros, onde C < n. O carro só pode dar a volta nos trilhos quando estiver cheio.

Os passageiros devem andar *pelo menos uma vez* no carro, além de invocar os métodos **board()** e **unboard()**. Adicionalmente, os passageiros *não podem embarcar sem tickets* e sem que o carro tenha invocado o método **load()**. É importante ressaltar que, por questões de segurança, os passageiros não podem desembarcar sem que o carro tenha invocado o método **unload()**.

O carro *não pode partir* até que C passageiros tenham embarcado. Ademais, o carro deve invocar os métodos **load()**, **run()** e **unload()**.

Um *ticket* é distribuído para cada passageiro no início do funcionamento da montanha-russa. Caso haja *assentos livres* para a última corrida, os passageiros sem ticket competem para conseguir um ticket grátis e *andar novamente* no carro.


## Arquivos
### Diretório *bin*
Armazena os arquivos binários do projeto.

### Diretório *src*
Armazena os códigos fontes do projeto. São eles:
+ `main.c`: código que faz a *implementação* e a *sincronização* das threads dos passageiros e do carrinho, além da *distribuição* dos tickets. Dados os valores de C e n, esse código determina como serão feitas as viagens do carrinho.
+ `animation.c`: código que implementa a *animação* da montanha-russa no terminal. A animação compreende o embarque, o desembarque e a movimentação do carrinho.
+ `animation.h`: arquivo header que contém as funções do arquivo `animation.c`.

### Diretório *tests*
Armazena os casos de teste (arquivos `.in`), as saídas esperadas para cada um (arquivos `.out`) e um *script* (`runTests.sh`) que executa o programa e compara as saídas obtidas com aquelas previstas.

## Estruturas de Dados
### **struct *passenger_data***
Possui duas variáveis do tipo *unsigned Int*: uma para guardar o id do passageiro e outra para o número de tickets que ele possui.  
Sua finalidade é de condensar, em uma única estrutura, o parâmetro necessário a ser passado para a função que será executada pelas *threads de passageiro*.

## Semáforos
### mutex1

### mutex2

### boardQueue

### unboardQueue

### allAboard

### all

## Mutex
### lr_mutex

### lr_cond

### log_lock

## Funções
### ***car***
+ **Parâmetros**: apontador para `void` (apenas para atender ao cabeçalho de função de thread).
+ **Retorno**: apontador para `void`.
+ **Funcionamento**: essa função é responsável pelo funcionamento do carro. Nela estão contidos os métodos **load()**, **run()** e **unload()**. Além disso, ela é responsável por avisar os passageiros quando for a última corrida, para distribuição de *tickets*, quando aplicável. Essa é a função executada pela *thread* de carro. Ao longo de sua execução, ela chama as funções **update_log_message()** e **move_car_scene()**. 

### ***passenger***
+ **Parâmetros**: apontador para `void` (é esperado que esse parâmetro seja um apontador para `passenger_data`).
+ **Retorno**: apontador para `void`.
+ **Funcionamento**: essa função é responsável por um passageiro desde seu embarque, até seu desembarque e, eventualmente, até a competição por *tickets* extras. Nela estão contidos os métodos **board()** e **unboard()**, além da verificação e da execução da competição pelos *tickets*. Essa é a função executada por cada *thread* de passageiro. Ao longo de sua execução, ela chama as funções **update_log_message()**, **boarding_scene()**, **unboarding_scene()** e **new_boarding_scene()**. 

### ***printScreen***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por atualizar a tela do terminal, dando continuidade à animação. Ela imprime o *estado atual* da matriz da tela.

### ***update_log_message***
+ **Parâmetros**: um vetor de `char`.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por atualizar a mensagem presente na região de log da matriz tela. Essa mensagem descreve o que o estado atual da animação está mostrando.

### ***addPassenger***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela *adição* dos passageiros na área de *embarque*. Ela atualiza a região de embarque da matriz da tela.

### ***start_animation***
+ **Parâmetros**: um `inteiro` que indica o número de passageiros.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pelo início do ambiente de animação. Ela adiciona os passageiros na matriz da tela, chamando a função **addPassenger()** em um *loop*, até ter adicionado todos. Além disso, ela chama a função **printScreen()** para imprimir o estado inicial da montanha-russa no terminal.

### ***boarding_scene***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela animação de *embarque* dos passageiros. Ela atualiza a região de embarque da matriz e chama a função **printScreen()** para atualizar o estado atual da montanha-russa no terminal.

### ***move_one_step***
+ **Parâmetros**: dois `inteiros`: um que indica a posição da frente do carro e outra que indica a posição da traseira do carro.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por mover o carro na matriz da tela, de cima para baixo, em uma posição. Ela considera que a traseira do carro está em uma posição válida da região da matriz que corresponde ao trilho do carro.

### ***move_one_step2***
+ **Parâmetros**: dois `inteiros`: um que indica a posição da frente do carro e outra que indica a posição da traseira do carro.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por mover o carro na matriz da tela, de cima para baixo, em uma posição. Ela considera que a traseira do carro está em uma posição inválida da região da matriz que corresponde ao trilho do carro.

### ***move_car_scene***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela animação do *movimento do carro*. Ela atualiza a região dos trilhos do carro na matriz da tela chamando duas funções: **move_one_step()** e **move_one_step2()**, de acordo com o que for mais adequado no estado atual. Após isso, ela chama a função **printScreen()** para atualizar o estado atual da montanha-russa no terminal.

### ***unboarding_scene***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela animação de *desembarque* dos passageiros. Ela atualiza a região de desembarque da matriz da tela e chama a função **printScreen()** para atualizar o estado atual da montanha-russa no terminal.

### ***new_boarding_scene***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela animação de quando os passageiros deixam a área de desembarque e *retornam* para a área de *embarque*. Ela atualiza a região de desembarque da matriz da tela e chama a função **addPassenger()** para adicionar o passageiro na região de embarque.
