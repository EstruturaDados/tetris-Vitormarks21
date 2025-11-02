/*
 * tetris.c
 *
 * Simulação simples da fila de peças (futuras) do jogo "Tetris Stack".
 *
 * Regras implementadas:
 * - Fila inicializada com um número fixo de peças (QUEUE_CAPACITY).
 * - Opções: jogar peça (dequeue), inserir nova peça (enqueue se houver espaço), sair.
 * - Peças geradas automaticamente pela função gerarPeca (tipo 'I','O','T','L' e id único).
 *
 * Compilar: gcc -std=c11 -O2 -Wall -Wextra -o tetris tetris.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define QUEUE_CAPACITY 5   /* Capacidade fixa da fila inicial */
#define INPUT_BUFFER 64

/* Estrutura que representa uma peça */
typedef struct {
    char nome; /* 'I', 'O', 'T', 'L' */
    int id;    /* identificador único */
} Peca;

/* Estrutura da fila circular de peças */
typedef struct {
    Peca buffer[QUEUE_CAPACITY];
    int head;   /* índice do elemento da frente */
    int size;   /* número de elementos atuais */
} FilaPecas;

/* Gera uma peça com tipo aleatório e id fornecido */
Peca gerarPeca(int id) {
    const char tipos[] = {'I','O','T','L'};
    Peca p;
    p.nome = tipos[rand() % (sizeof(tipos) / sizeof(tipos[0]))];
    p.id = id;
    return p;
}

/* Inicializa a fila vazia */
void inicializarFila(FilaPecas *fila) {
    fila->head = 0;
    fila->size = 0;
}

/* Verifica se a fila está vazia */
bool filaVazia(const FilaPecas *fila) {
    return fila->size == 0;
}

/* Verifica se a fila está cheia */
bool filaCheia(const FilaPecas *fila) {
    return fila->size == QUEUE_CAPACITY;
}

/* Enfileira uma peça; retorna true se sucesso */
bool enqueue(FilaPecas *fila, Peca p) {
    if (filaCheia(fila)) return false;
    int pos = (fila->head + fila->size) % QUEUE_CAPACITY;
    fila->buffer[pos] = p;
    fila->size++;
    return true;
}

/* Desenfileira a peça da frente; retorna true se sucesso e armazena em out */
bool dequeue(FilaPecas *fila, Peca *out) {
    if (filaVazia(fila)) return false;
    *out = fila->buffer[fila->head];
    fila->head = (fila->head + 1) % QUEUE_CAPACITY;
    fila->size--;
    return true;
}

/* Exibe o estado atual da fila de forma clara e legível */
void mostrarFila(const FilaPecas *fila) {
    printf("\nEstado atual da fila (capacidade %d):\n", QUEUE_CAPACITY);
    if (filaVazia(fila)) {
        printf("  [vazia]\n\n");
        return;
    }
    printf("  Índice  Tipo  ID\n");
    for (int i = 0; i < fila->size; ++i) {
        int idx = (fila->head + i) % QUEUE_CAPACITY;
        printf("  %5d   %4c  %4d\n", i, fila->buffer[idx].nome, fila->buffer[idx].id);
    }
    printf("\n");
}

/* Lê uma opção do usuário com tratamento simples de entrada */
int lerOpcao(void) {
    char linha[INPUT_BUFFER];
    if (!fgets(linha, sizeof(linha), stdin)) return -1;
    int opc = 0;
    if (sscanf(linha, "%d", &opc) != 1) return -1;
    return opc;
}

int main(void) {
    /* Semente para geração aleatória de tipos */
    srand((unsigned int) time(NULL));

    FilaPecas fila;
    inicializarFila(&fila);

    int proximoId = 1;

    /* Inicializa a fila com QUEUE_CAPACITY peças geradas automaticamente */
    for (int i = 0; i < QUEUE_CAPACITY; ++i) {
        Peca p = gerarPeca(proximoId++);
        enqueue(&fila, p);
    }

    printf("Tetris Stack - Simulador da fila de peças\n");
    printf("Fila inicializada com %d peças.\n", QUEUE_CAPACITY);
    mostrarFila(&fila);

    /* Loop do menu */
    while (1) {
        printf("Menu:\n");
        printf("  1 - Jogar peça (remover da frente)\n");
        printf("  2 - Inserir nova peça ao final (se houver espaço)\n");
        printf("  3 - Sair\n");
        printf("Escolha uma opção: ");

        int opc = lerOpcao();
        if (opc == -1) {
            printf("Entrada inválida. Tente novamente.\n\n");
            continue;
        }

        if (opc == 1) {
            Peca removida;
            if (dequeue(&fila, &removida)) {
                printf("\nPeça jogada: Tipo '%c'  ID %d\n", removida.nome, removida.id);
            } else {
                printf("\nA fila está vazia. Não há peça para jogar.\n");
            }
            mostrarFila(&fila);
        } else if (opc == 2) {
            if (filaCheia(&fila)) {
                printf("\nA fila está cheia. Não é possível inserir nova peça.\n");
            } else {
                Peca nova = gerarPeca(proximoId++);
                enqueue(&fila, nova);
                printf("\nNova peça inserida: Tipo '%c'  ID %d\n", nova.nome, nova.id);
            }
            mostrarFila(&fila);
        } else if (opc == 3) {
            printf("\nSaindo. Até logo.\n");
            break;
        } else {
            printf("Opção inválida. Escolha 1, 2 ou 3.\n\n");
        }
    }

    return 0;
}