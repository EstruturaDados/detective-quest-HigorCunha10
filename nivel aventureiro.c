#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para os nos da arvore BST de pistas
typedef struct PistaNode {
    char conteudo[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Estrutura para os comodos da mansao (Arvore Binaria)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Cria dinamicamente um no de pista para a BST
PistaNode* criarNoPista(char *conteudo) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    strcpy(novo->conteudo, conteudo);
    novo->esquerda = novo->direita = NULL;
    return novo;
}

// Insere uma pista na BST respeitando a ordem alfabetica
PistaNode* inserirPista(PistaNode *raiz, char *conteudo) {
    if (raiz == NULL) return criarNoPista(conteudo);

    if (strcmp(conteudo, raiz->conteudo) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    } else if (strcmp(conteudo, raiz->conteudo) > 0) {
        raiz->direita = inserirPista(raiz->direita, conteudo);
    }
    return raiz;
}

// Cria dinamicamente uma sala com nome e pista associada
Sala* criarSala(char *nome, char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

// Exibe as pistas da BST em ordem alfabetica (In-order)
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->conteudo);
        exibirPistas(raiz->direita);
    }
}

// Controla a navegacao do jogador e coleta automatica de pistas
PistaNode* explorarMansao(Sala *mapa, PistaNode *colecao) {
    Sala *atual = mapa;
    char escolha;

    while (atual != NULL) {
        printf("\nVoce entrou no(a): %s\n", atual->nome);
        
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: [%s]\n", atual->pista);
            colecao = inserirPista(colecao, atual->pista);
        } else {
            printf("Nao ha pistas neste comodo.\n");
        }

        printf("Para onde ir? Esquerda (e), Direita (d) ou Sair e ver pistas (s): ");
        scanf(" %c", &escolha);

        if (escolha == 's') break;
        
        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else {
            printf("Caminho sem saida ou opcao invalida!\n");
        }
    }
    return colecao;
}

// Libera memoria das duas arvores
void liberarMemoria(Sala *s, PistaNode *p) {
    if (s != NULL) {
        liberarMemoria(s->esquerda, NULL);
        liberarMemoria(s->direita, NULL);
        free(s);
    }
    if (p != NULL) {
        liberarMemoria(NULL, p->esquerda);
        liberarMemoria(NULL, p->direita);
        free(p);
    }
}

int main() {
    PistaNode *minhasPistas = NULL;

    // Montagem do mapa fixo da mansao
    Sala *hall = criarSala("Hall de Entrada", "Chave enferrujada");
    hall->esquerda = criarSala("Cozinha", "Faca de prata");
    hall->direita = criarSala("Biblioteca", "Bilhete anonimo");
    hall->esquerda->esquerda = criarSala("Despensa", "Pegada de lama");
    hall->direita->direita = criarSala("Escritorio", "Agenda secreta");

    // Inicio do jogo
    minhasPistas = explorarMansao(hall, minhasPistas);

    printf("\n--- RELATORIO FINAL DO DETETIVE ---\n");
    if (minhasPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        printf("Pistas organizadas alfabeticamente:\n");
        exibirPistas(minhasPistas);
    }

    liberarMemoria(hall, minhasPistas);
    return 0;
};
