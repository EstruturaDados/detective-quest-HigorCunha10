#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 13

// Estrutura da Tabela Hash (Associação Pista -> Suspeito)
typedef struct NoHash {
    char pista[50];
    char suspeito[50];
    struct NoHash* proximo;
} NoHash;

// Estrutura da BST (Coleção de Pistas do Jogador)
typedef struct NoBST {
    char pista[50];
    struct NoBST *esq, *dir;
} NoBST;

// Estrutura da Arvore Binaria (Mapa da Mansao)
typedef struct Sala {
    char nome[50];
    char pista[50];
    struct Sala *esq, *dir;
} Sala;

// Funcao Hash simples (Soma ponderada)
int funcaoHash(const char* str) {
    int hash = 0;
    for (int i = 0; str[i] != '\0'; i++) hash += str[i] * (i + 1);
    return hash % TAM_HASH;
}

// insereNaHash() - Associa uma pista a um suspeito na tabela
void inserirNaHash(NoHash* tabela[], char* pista, char* suspeito) {
    int indice = funcaoHash(pista);
    NoHash* novo = (NoHash*) malloc(sizeof(NoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabela[indice];
    tabela[indice] = novo;
}

// encontrarSuspeito() - Retorna o suspeito dono da pista
char* encontrarSuspeito(NoHash* tabela[], char* pista) {
    int indice = funcaoHash(pista);
    NoHash* atual = tabela[indice];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0) return atual->suspeito;
        atual = atual->proximo;
    }
    return "Desconhecido";
}

// criarSala() - Cria dinamicamente um comodo do mapa
Sala* criarSala(char* nome, char* pista) {
    Sala* s = (Sala*) malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    strcpy(s->pista, pista);
    s->esq = s->dir = NULL;
    return s;
}

// adicionarPista() - Insere pista coletada na BST (Ordem Alfabetica)
NoBST* adicionarPista(NoBST* raiz, char* pista) {
    if (raiz == NULL) {
        NoBST* novo = (NoBST*) malloc(sizeof(NoBST));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0) raiz->esq = adicionarPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0) raiz->dir = adicionarPista(raiz->dir, pista);
    return raiz;
}

// Conta quantas pistas na BST pertencem ao suspeito acusado
int contarPistasSuspeito(NoBST* raiz, NoHash* tabela[], char* acusado) {
    if (raiz == NULL) return 0;
    int cont = 0;
    if (strcmp(encontrarSuspeito(tabela, raiz->pista), acusado) == 0) cont = 1;
    return cont + contarPistasSuspeito(raiz->esq, tabela, acusado) + 
                  contarPistasSuspeito(raiz->dir, tabela, acusado);
}

// explorarSalas() - Navegacao interativa e coleta de pistas
NoBST* explorarSalas(Sala* raiz, NoBST* colecao) {
    Sala* atual = raiz;
    char opt;
    while (atual) {
        printf("\nLocal: %s", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("\n[PISTA ENCONTRADA]: %s", atual->pista);
            colecao = adicionarPista(colecao, atual->pista);
        }
        printf("\nComandos: (e)sq, (d)ir, (s)air: ");
        scanf(" %c", &opt);
        if (opt == 's') break;
        if (opt == 'e') atual = atual->esq;
        else if (opt == 'd') atual = atual->dir;
    }
    return colecao;
}

int main() {
    NoHash* tabelaSuspeitos[TAM_HASH] = {NULL};
    NoBST* pistasColetadas = NULL;

    // Cadastro de Suspeitos na Hash
    inserirNaHash(tabelaSuspeitos, "Veneno", "Mordomo");
    inserirNaHash(tabelaSuspeitos, "Luva Branca", "Mordomo");
    inserirNaHash(tabelaSuspeitos, "Relogio de Ouro", "Cozinheira");
    inserirNaHash(tabelaSuspeitos, "Faca de Carne", "Cozinheira");

    // Montagem do Mapa (Arvore Binaria)
    Sala* mansao = criarSala("Hall", "");
    mansao->esq = criarSala("Jardim", "Veneno");
    mansao->dir = criarSala("Cozinha", "Faca de Carne");
    mansao->esq->esq = criarSala("Garagem", "Luva Branca");

    // Fase de Exploracao
    pistasColetadas = explorarSalas(mansao, pistasColetadas);

    // Julgamento Final
    char acusado[50];
    printf("\n--- O JULGAMENTO ---\nQuem voce acusa? ");
    scanf(" %[^\n]s", acusado);

    int total = contarPistasSuspeito(pistasColetadas, tabelaSuspeitos, acusado);
    printf("Pistas que incriminam %s: %d\n", acusado, total);

    if (total >= 2) printf("JUSTICA FEITA! %s foi preso(a) com provas suficientes.\n", acusado);
    else printf("ERRO JUDICIARIO! Nao ha provas suficientes contra %s.\n", acusado);

    return 0;
};

