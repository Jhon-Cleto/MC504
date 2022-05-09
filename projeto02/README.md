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

Os passageiros devem andar *pelo menos uma vez* no carro, além de realizar o embarque e o desembarque. Adicionalmente, os passageiros *não podem embarcar sem tickets* e sem que o carro tenha sinalizado que está pronto para receber passageiros. É importante ressaltar que, por questões de segurança, os passageiros não podem desembarcar sem que o carro tenha sinalizado que os passageiros podem desembarcar.

O carro *não pode partir* até que C passageiros tenham embarcado. Ademais, o carro deve sinalizar que está pronto para receber passageiros, andar e sinalizar que os passageiros podem desembarcar.

Um *ticket* é distribuído para cada passageiro no início do funcionamento da montanha-russa. Caso haja *assentos livres* para a última corrida, os passageiros sem ticket competem para conseguir um ticket grátis e *andar novamente* no carro.


## Arquivos
### Diretório *bin*
Armazena os arquivos binários do projeto.

### Diretório *src*
Armazena os códigos fontes do projeto. São eles:
+ `Makefile`: arquivo para compilar o programa.
+ `main.c`: código que faz a *implementação* e a *sincronização* das threads dos passageiros e do carrinho, além da *distribuição* dos tickets. Dados os valores de C e n, esse código determina como serão feitas as viagens do carrinho.
+ `animation.c`: código que implementa a *animação* da montanha-russa no terminal. A animação compreende o embarque, o desembarque e a movimentação do carrinho.
+ `animation.h`: arquivo header que contém as funções do arquivo `animation.c`.


## Estruturas de Dados
### **struct *passenger_data***
Possui duas variáveis do tipo `unsigned int`: uma para guardar o id do passageiro e outra para o número de tickets que ele possui.  
Sua finalidade é de condensar, em uma única estrutura, o parâmetro necessário a ser passado para a função que será executada pelas *threads de passageiro*.

## Semáforos
### mutex1
Esse semáforo está presente no código para garantir a exclusão mútua com relação à variável **boarders**, que conta quantos passageiros embarcaram. Ele é importante para garantir a contagem correta e que não mais do que **C** passageiros embarquem no carro. Esse semáforo está presente na função da *thread* do passageiro.

### mutex2
Esse semáforo está presente no código para garantir a exclusão mútua com relação à variável **unboarders**, que conta quantos passageiros desembarcaram. Ele é importante para garantir a contagem correta de passageiros que desembarcaram do carro. Esse semáforo está presente na função da *thread* do passageiro.

### boardQueue
Esse semáforo é responsável por realizar corretamente a sinalização, aos passageiros, de que o carro está pronto para *receber* passageiros. Além disso, ele garante exclusão mútua, fazendo com que exatamente **C** passageiros sejam avisados. O *post* desse semáforo é feito **C** vezes pela *thread* do carro, e o *wait* é feito uma vez para cada *thread* de passageiro.

### unboardQueue
Esse semáforo é responsável por realizar corretamente a sinalização, aos passageiros, de que o carro está pronto para *desembarcar*. Além disso, ele garante exclusão mútua, fazendo com que exatamente **C** passageiros sejam avisados. O *post* desse semáforo é feito **C** vezes pela *thread* do carro, e o *wait* é feito uma vez para cada *thread* de passageiro.

### allAboard
Esse semáforo é responsável por sinalizar ao carro que **C** passageiros já embarcaram. O *post* desse semáforo é feito pela *thread* do último passageiro a embarcar, e o *wait* é feito pela *thread* do carro, sendo a condição para começar o percurso. Dessa forma, a *thread* do carro não precisa ficar contando sempre que um passageiro novo embarcou.

### allAshore
Esse semáforo é responsável por sinalizar ao carro que todos os **C** passageiros já desembarcaram. O *post* desse semáforo é feito pela *thread* do último passageiro a desembarcar, e o *wait* é feito pela *thread* do carro, sendo a condição para sinalizar aos passageiros de que está pronto para recebê-los. Dessa forma, a *thread* do carro não precisa ficar contando sempre que um passageiro novo desembarcou.

## Mutex
### lr_mutex
Esse mutex está presente para garantir que todos os passageiros, que estão aguardando, sejam avisados quando eles puderem competir por um *ticket*, além da exclusão mútua para que apenas o número correto de passageiros peguem os *tickets*. Assim, a *thread* do carro trava o mutex antes de iniciar a competição, avisa as *threads* dos passageiros que a competição vai começar e, então, libera o mutex. A partir disso, as primeiras *threads* que conseguirem pegar o mutex ganharão a competição, garantindo que não serão distribuídos *tickets* a mais.

### lr_cond
Apesar de não ser um mutex, essa *condição* está inserida nessa seção pois ela libera o mutex **lr_mutex** até que ela seja atendida e o tome novamente, estando, assim, diretamente relacionada a um mutex. No nosso código, ela corresponde à condição de ser a última corrida do carro, a qual os passageiros aguardam para poderem competir pelos *tickets*.

### log_lock
Esse mutex está presente para garantir que apenas uma *thread* (o carro ou algum passageiro) esteja alterando a mensagem do log na animação. Ele garante exclusão mútua à alteração da mensagem.

### add_lock
Esse mutex está presente para garantir que apenas uma *thread* (de passageiro) esteja adicionando um passageiro à matriz da tela. Ele garante exclusão mútua à posição da matriz destinada ao passageiro, evitando que duas *threads* adicionem o mesmo símbolo P, o que acarretaria em uma contagem errada de passageiros na matriz da tela.


## Funções
### ***car***
+ **Parâmetros**: apontador para `void` (apenas para atender ao cabeçalho de função de thread, não será utilizado).
+ **Retorno**: apontador para `void`.
+ **Funcionamento**: essa função é responsável pelo funcionamento do carro, que compreende: sinalizar que está pronto para receber passageiros, andar e sinalizar que os passageiros podem desembarcar. Além disso, ela é responsável por avisar os passageiros quando for a última corrida, para distribuição de *tickets*, quando aplicável. Essa é a função executada pela *thread* de carro. Ao longo de sua execução, ela chama as funções **update_log_message()**, **move_car_scene()** e **update_ride_log()**. 

### ***passenger***
+ **Parâmetros**: apontador para `void` (é esperado que esse parâmetro seja um apontador para `passenger_data`).
+ **Retorno**: apontador para `void`.
+ **Funcionamento**: essa função é responsável por um passageiro desde seu embarque, até seu desembarque, verificação se ele pode concorrer por mais *tickets* e, eventualmente, até a competição por *tickets* extras. Essa é a função executada por cada *thread* de passageiro. Ao longo de sua execução, ela chama as funções **update_log_message()**, **boarding_scene()**, **unboarding_scene()**, **new_boarding_scene()** e **arrival_scene()**. 

### ***printScreen***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por atualizar a tela do terminal, dando continuidade à animação. Ela imprime o *estado atual* da matriz da tela.

### ***update_log_message***
+ **Parâmetros**: um vetor de `char` contendo a mensagem para o log.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por atualizar a mensagem presente na região de log da matriz tela. Essa mensagem descreve o que o estado atual da animação está mostrando.

### ***intToChar***
+ **Parâmetros**: uma variável do tipo `inteiro`.
+ **Retorno**: uma variável do tipo `char`.
+ **Funcionamento**: essa função é responsável por transformar um número representado como `inteiro` em um número representado como `char`.

### ***updateCount***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por atualizar o contador de passageiros na matriz da tela.

### ***addPassenger***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela *adição* dos passageiros na área de *embarque*. Ela atualiza a região de embarque da matriz da tela.

### ***start_animation***
+ **Parâmetros**: duas variáveis do tipo `inteiro`, uma que indica o número de passageiros e outra que indica o número de passageiros que cabem no carrinho (**C**).
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pelo início do ambiente de animação. Ela adiciona os passageiros na matriz da tela, chamando a função **addPassenger()** em um *loop*, até ter adicionado todos. Além disso, ela chama a função **printScreen()** para imprimir o estado inicial da montanha-russa no terminal.

### ***boarding_scene***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela animação de *embarque* dos passageiros. Ela atualiza a região de embarque da matriz e chama a função **printScreen()** para atualizar o estado atual da montanha-russa no terminal.

### ***move_one_step***
+ **Parâmetros**: dois `inteiros`, um que indica a posição da frente do carro e outra que indica a posição da traseira do carro.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por mover o carro na matriz da tela, de cima para baixo, em uma posição. Ela considera que a traseira do carro está em uma posição válida da região da matriz que corresponde ao trilho do carro.

### ***move_one_step2***
+ **Parâmetros**: dois `inteiros`, um que indica a posição da frente do carro e outra que indica a posição da traseira do carro.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por mover o carro na matriz da tela, de cima para baixo, em uma posição. Ela considera que a traseira do carro está em uma posição inválida da região da matriz que corresponde ao trilho do carro.

### ***move_car_scene***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela animação do *movimento do carro*. Ela atualiza a região dos trilhos do carro na matriz da tela chamando duas funções: **move_one_step()** e **move_one_step2()**, de acordo com o que for mais adequado no estado atual. Após isso, ela chama a função **printScreen()** para atualizar o estado atual da montanha-russa no terminal. Por fim, ela chama a função **updateCount()** para atualizar o contador de passageiros na matriz da tela.

### ***unboarding_scene***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela animação de *desembarque* dos passageiros. Ela atualiza a região de desembarque da matriz da tela e chama a função **printScreen()** para atualizar o estado atual da montanha-russa no terminal. Por fim, ela chama a função **updateCount()** para atualizar o contador de passageiros na matriz da tela.

### ***new_boarding_scene***
+ **Parâmetros**: nenhum.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela animação de quando os passageiros deixam a área de desembarque e *retornam* para a área de *embarque*. Ela atualiza a região de desembarque da matriz da tela e chama a função **addPassenger()** para adicionar o passageiro na região de embarque.

### ***update_ride_log***
+ **Parâmetros**: uma variável do tipo `unsigned int` que indica o número identificador da corrida.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável por atualizar o número identificador da corrida na matriz da tela.

### ***arrival_scene***
+ **Parâmetros**: uma variável do tipo `int`.
+ **Retorno**: nenhum.
+ **Funcionamento**: essa função é responsável pela animação de quando os passageiros chegam à área de embarque, incluindo a mensagem do log e a chamada da função **addPassenger()**.
