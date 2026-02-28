#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa cada comodo da mansao
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Funcao para criar uma sala dinamicamente na memoria
Sala* criarSala(char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

// Funcao para navegar pela arvore binaria conforme escolha do jogador
void explorarMansao(Sala *raiz) {
    Sala *atual = raiz;
    char escolha;

    printf("Bem-vindo ao Detective Quest!\n");

    while (atual != NULL) {
        printf("\nVoce esta no(a): %s\n", atual->nome);

        // Verifica se e um comodo sem saidas (no-folha)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Voce chegou ao fim do caminho. Exploracao concluida!\n");
            break;
        }

        printf("Caminhos disponiveis:\n");
        if (atual->esquerda != NULL) printf("- Esquerda (e): %s\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("- Direita (d): %s\n", atual->direita->nome);
        printf("- Sair do jogo (s)\n");
        
        printf("Escolha sua direcao: ");
        scanf(" %c", &escolha);

        if (escolha == 's') {
            printf("Saindo da mansao...\n");
            break;
        } else if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else {
            printf("Caminho invalido! Tente novamente.\n");
        }
    }
}

// Funcao para liberar a memoria alocada para a arvore
void liberarMapa(Sala *raiz) {
    if (raiz != NULL) {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}

int main() {
    // Montagem automatica do mapa da mansao
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *jardim = criarSala("Jardim");
    Sala *escritorio = criarSala("Escritorio");

    // Definicao das conexoes (Arestas da arvore)
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    
    biblioteca->esquerda = escritorio;

    // Inicia a exploracao interativa
    explorarMansao(hall);

    // Limpeza da memoria antes de encerrar
    liberarMapa(hall);

    return 0;
};
