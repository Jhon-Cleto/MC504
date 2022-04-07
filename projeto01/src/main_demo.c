#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define MOD_N 10

// Cores no Terminal
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Estrutura de dados e tipos utilizadas no projeto
typedef struct matrix matrix_t;
typedef struct thread_data thread_data_t;
typedef unsigned long long uli; // números inteiros positivos de 64 bits

struct matrix
{
    int *mat;
    uli rows, cols;
};

struct thread_data
{
    uli row;
};


// Ponteiros Globais para as matrizes A, B e C
matrix_t *A, *B, *C;

matrix_t *allocata_matrix(uli rows, uli cols)
{
    matrix_t *m;
    m = (matrix_t *) malloc(sizeof(matrix_t));
    m->mat = (int *) malloc (rows*cols*sizeof(int));
    m->rows = rows;
    m->cols = cols;
    return m;
}

void deallocate_matrix(matrix_t *m)
{
    free(m->mat);
    free(m);
}

// Alocar as matrizes A, B e C 
// e inserir os valores gerados utilizando a semente
void initialize_matrices(uli ra, uli ca, uli rb, uli cb, uli seed)
{
    // Definindo a semente para geração de numeros aleatórios
    srand(seed);

    // Alocar as estruturas
    A = allocata_matrix(ra, ca);
    B = allocata_matrix(rb, cb);
    C = allocata_matrix(ra, cb);

    // Inserir os elementos da matriz A
    for (uli i = 0; i < ra*ca; i++)
        A->mat[i] = rand() % MOD_N;

    // Inserir os elementos da matriz B
    for (uli i = 0; i < rb*cb; i++)
        B->mat[i] = rand() % MOD_N;

    // Iniciar todas as posições da matriz C em 0
    memset(C->mat, 0, ra*cb*sizeof(int));
}

// Função que imprime uma matriz na tela
void print_matrix(matrix_t *m)
{
    uli rows = m->rows, cols = m->cols;
    int* mat = m->mat;

    for (uli i = 0; i < rows; i++)
    {
        for (uli j = 0; j < cols; j++)
            printf(" %03d", mat[i*cols + j]);
        printf("\n");
    }
}

// Função que calcula a multiplicação
// para todos os elementos em uma linha da matriz C
void *multiply(void *arg)
{
    thread_data_t t_data = *((thread_data_t *) arg);
    uli row = t_data.row, a_cols = A->cols, b_cols = B->cols;
    int *a_mat = A->mat, *b_mat = B->mat, *c_mat = C->mat;
    int sum;

    for (uli j = 0; j < b_cols; j++)
    {
        sum = 0;
        for (uli k = 0; k < a_cols; k++)
            sum += a_mat[row*a_cols + k] * b_mat[k*b_cols + j];

        c_mat[row*b_cols + j] = sum;
        sleep((rand() % 3));
    }

    return NULL;
}

void *show_result_matrix(void *arg)
{
    uli rows = C->rows, cols = C->cols;
    int* mat = C->mat;
    printf("\e[1;1H\e[2J");

    while(1)
    {
        printf("Matrix C (%u x %u):\n\n", rows, cols);
        for (uli i = 0; i < rows; i++)
        {
            for (uli j = 0; j < cols; j++)
            {
                if (mat[i*cols + j] == 0)
                {
                    if (j != 0 && mat[i*cols + j-1] != 0)
                        printf(ANSI_COLOR_YELLOW " %03d" ANSI_COLOR_RESET, mat[i*cols + j]);
                    else
                        printf(" %03d", mat[i*cols + j]);
                }
                
                else
                    printf(ANSI_COLOR_GREEN " %03d" ANSI_COLOR_RESET, mat[i*cols + j]);

            }

            printf("\n");
        }        
        sleep(1);
        printf("\e[1;1H\e[2J");
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    // Verificar se o nome do arquivo foi informado
    if (argc < 2) {
        fprintf(stderr, "Erro: Número insuficiente de parâmetros!\n");
        return 1;
    }

    // Lendo o arquivo com as configurações da Matriz
    FILE *file;

    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo \'%s\'.\n", argv[1]);
        return 1;
    }
    
    unsigned rows_a, cols_a, rows_b, cols_b, seed;
    fscanf(file, "%u %u", &rows_a, &cols_a);
    fscanf(file, "%u %u", &rows_b, &cols_b);
    fscanf(file, " %u", &seed);
    
    if(cols_a != rows_b) {
        fprintf(stderr, "Erro: Matrizes com dimensões invalidas!\n");
        return 1;
    }

    // Alocar as Matrizes
    initialize_matrices(rows_a, cols_a, rows_b, cols_b, seed);

    // Imprimir as matrizes utilizadas na multiplicação
    printf("Multiplicação de Matrizes: A x B = C\n\n");
    printf("Matriz A (%u x %u):\n", rows_a, cols_a);
    print_matrix(A);
    printf("\n");
    printf("Matriz B (%u x %u):\n", rows_b, cols_b);
    print_matrix(B);
    printf("\n");
    sleep(2);

    // Criação das threads
    pthread_t t_ids[rows_a+1];
    thread_data_t t_data[rows_a];

    // 1 thread por linha da matriz A
    for (uli i = 0; i < rows_a; i++)
    {
        t_data[i].row = i;
        pthread_create(&t_ids[i], NULL, multiply, &t_data[i]);
    }
    
    pthread_create(&t_ids[rows_a], NULL, show_result_matrix, NULL);
    
    // Esperar até que as threads concluam suas multiplicações
    for (uli i = 0; i < rows_a; i++)
        pthread_join(t_ids[i], NULL);
    

    // Liberar o espeço alocado 
    deallocate_matrix(A);
    deallocate_matrix(B);
    deallocate_matrix(C);    

    return 0;
}