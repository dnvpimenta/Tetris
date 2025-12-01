#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================================
// DEFINIÇÕES E CONSTANTES
// ============================================

// Constantes para o tamanho da fila
#define TAMANHO_FILA 5
#define MAX_PECAS 100  // Para controlar IDs únicos

// Struct para representar uma peça do Tetris
typedef struct {
    char nome;      // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;         // Identificador único
} Peca;

// Struct para representar a fila circular
typedef struct {
    Peca elementos[TAMANHO_FILA];
    int frente;     // Índice do primeiro elemento
    int tras;       // Índice do último elemento
    int tamanho;    // Número atual de elementos
} FilaPecas;

// ============================================
// PROTÓTIPOS DAS FUNÇÕES
// ============================================

// Funções de inicialização
void inicializarFila(FilaPecas *fila);
Peca gerarPeca(int id);

// Operações básicas da fila
int jogarPeca(FilaPecas *fila);        // Dequeue
int inserirPeca(FilaPecas *fila);      // Enqueue
int filaCheia(FilaPecas *fila);
int filaVazia(FilaPecas *fila);

// Funções de exibição
void exibirFila(FilaPecas *fila);
void exibirMenu();

// ============================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ============================================

// Inicializa a fila com 5 peças geradas automaticamente
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
    
    // Gera 5 peças iniciais
    for (int i = 0; i < TAMANHO_FILA; i++) {
        inserirPeca(fila);
    }
    
    printf("Fila inicializada com %d peças!\n", TAMANHO_FILA);
}

// Gera uma nova peça com tipo aleatório e ID único
Peca gerarPeca(int id) {
    Peca novaPeca;
    novaPeca.id = id;
    
    // Tipos de peças disponíveis no Tetris
    char tipos[] = {'I', 'O', 'T', 'L'};
    int numTipos = sizeof(tipos) / sizeof(tipos[0]);
    
    // Escolhe um tipo aleatório
    int indice = rand() % numTipos;
    novaPeca.nome = tipos[indice];
    
    return novaPeca;
}

// Verifica se a fila está cheia
int filaCheia(FilaPecas *fila) {
    return fila->tamanho == TAMANHO_FILA;
}

// Verifica se a fila está vazia
int filaVazia(FilaPecas *fila) {
    return fila->tamanho == 0;
}

// Remove e retorna a peça da frente da fila (Dequeue)
int jogarPeca(FilaPecas *fila) {
    if (filaVazia(fila)) {
        printf("ERRO: Não há peças para jogar! A fila está vazia.\n");
        return 0;
    }
    
    // Obtém a peça que será jogada
    Peca pecaJogada = fila->elementos[fila->frente];
    printf("Peça jogada: [%c %d]\n", pecaJogada.nome, pecaJogada.id);
    
    // Atualiza o índice da frente (circular)
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->tamanho--;
    
    return 1;
}

// Adiciona uma nova peça ao final da fila (Enqueue)
int inserirPeca(FilaPecas *fila) {
    if (filaCheia(fila)) {
        printf("ERRO: Não é possível adicionar mais peças! A fila está cheia.\n");
        return 0;
    }
    
    // ID global para garantir unicidade (simplificado)
    static int proximoId = 0;
    
    // Gera a nova peça
    Peca novaPeca = gerarPeca(proximoId++);
    
    // Atualiza o índice do final (circular)
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->elementos[fila->tras] = novaPeca;
    fila->tamanho++;
    
    printf("Nova peça adicionada: [%c %d]\n", novaPeca.nome, novaPeca.id);
    
    return 1;
}

// Exibe o estado atual da fila de peças
void exibirFila(FilaPecas *fila) {
    printf("\n==========================================\n");
    printf("         FILA DE PEÇAS FUTURAS\n");
    printf("==========================================\n");
    
    if (filaVazia(fila)) {
        printf("A fila está vazia!\n");
    } else {
        printf("Posição: ");
        
        // Percorre a fila circular
        for (int i = 0; i < fila->tamanho; i++) {
            int indice = (fila->frente + i) % TAMANHO_FILA;
            Peca peca = fila->elementos[indice];
            printf("[%c %d] ", peca.nome, peca.id);
        }
        
        // Mostra espaços vazios
        for (int i = fila->tamanho; i < TAMANHO_FILA; i++) {
            printf("[   ] ");
        }
        
        printf("\n\nLegenda:\n");
        printf("  I = Peça I (reta)      O = Peça O (quadrado)\n");
        printf("  T = Peça T             L = Peça L\n");
        printf("\nEstatísticas:\n");
        printf("  Peças na fila: %d/%d\n", fila->tamanho, TAMANHO_FILA);
        printf("  Próxima peça: [%c %d]\n", 
               fila->elementos[fila->frente].nome, 
               fila->elementos[fila->frente].id);
    }
    printf("==========================================\n");
}

// Exibe o menu de opções
void exibirMenu() {
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║            TETRIS STACK - MENU           ║\n");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║ 1. Jogar peça (remover da frente)        ║\n");
    printf("║ 2. Inserir nova peça (adicionar ao final)║\n");
    printf("║ 3. Visualizar fila de peças              ║\n");
    printf("║ 0. Sair do jogo                          ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("\nEscolha uma opção (0-3): ");
}

// ============================================
// FUNÇÃO PRINCIPAL
// ============================================

int main() {
    FilaPecas fila;
    int opcao;
    int rodando = 1;
    
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    printf("╔══════════════════════════════════════════╗\n");
    printf("║          BEM-VINDO AO TETRIS STACK!      ║\n");
    printf("║    Sistema de Controle de Peças Futuras  ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("\nDesenvolvido por: ByteBros Game Studio\n");
    printf("==========================================\n");
    
    // Inicializa a fila com 5 peças
    inicializarFila(&fila);
    exibirFila(&fila);
    
    // Loop principal do programa
    while (rodando) {
        exibirMenu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 0: // Sair
                printf("\nEncerrando o Tetris Stack...\n");
                printf("Obrigado por jogar! Até a próxima! 🎮\n");
                rodando = 0;
                break;
                
            case 1: // Jogar peça (Dequeue)
                printf("\n➤ Ação: Jogar peça\n");
                if (jogarPeca(&fila)) {
                    printf("Peça removida da frente da fila com sucesso!\n");
                }
                exibirFila(&fila);
                break;
                
            case 2: // Inserir nova peça (Enqueue)
                printf("\n➤ Ação: Inserir nova peça\n");
                if (inserirPeca(&fila)) {
                    printf("Peça adicionada ao final da fila com sucesso!\n");
                }
                exibirFila(&fila);
                break;
                
            case 3: // Visualizar fila
                printf("\n➤ Ação: Visualizar fila\n");
                exibirFila(&fila);
                break;
                
            default:
                printf("\n❌ Opção inválida! Por favor, escolha uma opção de 0 a 3.\n");
        }
    }
    
    return 0;
}