#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================================
// DEFINIÇÕES E CONSTANTES
// ============================================

// Constantes para tamanhos das estruturas
#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3
#define MAX_PECAS 1000  // Para IDs únicos

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

// Struct para representar a pilha (LIFO)
typedef struct {
    Peca elementos[TAMANHO_PILHA];
    int topo;       // Índice do topo da pilha
    int tamanho;    // Número atual de elementos
} PilhaPecas;

// ============================================
// PROTÓTIPOS DAS FUNÇÕES
// ============================================

// Funções de inicialização
void inicializarFila(FilaPecas *fila);
void inicializarPilha(PilhaPecas *pilha);
Peca gerarPeca(int id);

// Funções para fila
int filaCheia(FilaPecas *fila);
int filaVazia(FilaPecas *fila);
int inserirNaFila(FilaPecas *fila, Peca peca);
Peca removerDaFila(FilaPecas *fila);
void manterFilaCheia(FilaPecas *fila);

// Funções para pilha
int pilhaCheia(PilhaPecas *pilha);
int pilhaVazia(PilhaPecas *pilha);
int empilhar(PilhaPecas *pilha, Peca peca);
Peca desempilhar(PilhaPecas *pilha);

// Operações do jogo
void jogarPeca(FilaPecas *fila);
void reservarPeca(FilaPecas *fila, PilhaPecas *pilha);
void usarPecaReservada(PilhaPecas *pilha);
void trocarPecaUnica(FilaPecas *fila, PilhaPecas *pilha);
void trocaMultipla(FilaPecas *fila, PilhaPecas *pilha);

// Funções de exibição
void exibirEstado(FilaPecas *fila, PilhaPecas *pilha);
void exibirMenu();

// ============================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ============================================

// Inicializa o gerador de números aleatórios e as estruturas
void inicializarSistema() {
    srand(time(NULL));
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

// ========== IMPLEMENTAÇÃO DA FILA ==========

// Inicializa a fila com 5 peças geradas automaticamente
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
    
    // ID global para garantir unicidade
    static int proximoId = 0;
    
    // Gera e insere 5 peças iniciais
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Peca novaPeca = gerarPeca(proximoId++);
        inserirNaFila(fila, novaPeca);
    }
}

// Verifica se a fila está cheia
int filaCheia(FilaPecas *fila) {
    return fila->tamanho == TAMANHO_FILA;
}

// Verifica se a fila está vazia
int filaVazia(FilaPecas *fila) {
    return fila->tamanho == 0;
}

// Insere uma peça no final da fila
int inserirNaFila(FilaPecas *fila, Peca peca) {
    if (filaCheia(fila)) {
        return 0; // Falha
    }
    
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->elementos[fila->tras] = peca;
    fila->tamanho++;
    
    return 1; // Sucesso
}

// Remove e retorna a peça da frente da fila
Peca removerDaFila(FilaPecas *fila) {
    Peca pecaVazia = {'?', -1}; // Peça inválida para erro
    
    if (filaVazia(fila)) {
        return pecaVazia;
    }
    
    Peca pecaRemovida = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->tamanho--;
    
    return pecaRemovida;
}

// Mantém a fila sempre cheia (adiciona novas peças se necessário)
void manterFilaCheia(FilaPecas *fila) {
    static int proximoId = TAMANHO_FILA; // Continuação dos IDs
    
    while (!filaCheia(fila)) {
        Peca novaPeca = gerarPeca(proximoId++);
        inserirNaFila(fila, novaPeca);
    }
}

// ========== IMPLEMENTAÇÃO DA PILHA ==========

// Inicializa a pilha vazia
void inicializarPilha(PilhaPecas *pilha) {
    pilha->topo = -1;
    pilha->tamanho = 0;
}

// Verifica se a pilha está cheia
int pilhaCheia(PilhaPecas *pilha) {
    return pilha->tamanho == TAMANHO_PILHA;
}

// Verifica se a pilha está vazia
int pilhaVazia(PilhaPecas *pilha) {
    return pilha->tamanho == 0;
}

// Empilha uma peça (insere no topo)
int empilhar(PilhaPecas *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        return 0; // Falha
    }
    
    pilha->topo++;
    pilha->elementos[pilha->topo] = peca;
    pilha->tamanho++;
    
    return 1; // Sucesso
}

// Desempilha e retorna a peça do topo
Peca desempilhar(PilhaPecas *pilha) {
    Peca pecaVazia = {'?', -1}; // Peça inválida para erro
    
    if (pilhaVazia(pilha)) {
        return pecaVazia;
    }
    
    Peca pecaRemovida = pilha->elementos[pilha->topo];
    pilha->topo--;
    pilha->tamanho--;
    
    return pecaRemovida;
}

// ========== OPERAÇÕES DO JOGO ==========

// Joga a peça da frente da fila
void jogarPeca(FilaPecas *fila) {
    printf("\n➤ Ação: Jogar peça da frente da fila\n");
    
    if (filaVazia(fila)) {
        printf("❌ ERRO: Fila vazia! Nenhuma peça para jogar.\n");
        return;
    }
    
    Peca pecaJogada = removerDaFila(fila);
    printf("✅ Peça [%c %d] jogada!\n", pecaJogada.nome, pecaJogada.id);
    
    // Mantém a fila cheia
    manterFilaCheia(fila);
}

// Reserva a peça da frente da fila (move para a pilha)
void reservarPeca(FilaPecas *fila, PilhaPecas *pilha) {
    printf("\n➤ Ação: Reservar peça da fila\n");
    
    if (filaVazia(fila)) {
        printf("❌ ERRO: Fila vazia! Nenhuma peça para reservar.\n");
        return;
    }
    
    if (pilhaCheia(pilha)) {
        printf("❌ ERRO: Pilha cheia! Não é possível reservar mais peças.\n");
        printf("   Use uma peça reservada primeiro ou faça uma troca.\n");
        return;
    }
    
    // Remove da fila e empilha
    Peca pecaReservada = removerDaFila(fila);
    empilhar(pilha, pecaReservada);
    
    printf("✅ Peça [%c %d] reservada na pilha!\n", 
           pecaReservada.nome, pecaReservada.id);
    
    // Mantém a fila cheia
    manterFilaCheia(fila);
}

// Usa a peça do topo da pilha
void usarPecaReservada(PilhaPecas *pilha) {
    printf("\n➤ Ação: Usar peça da pilha de reserva\n");
    
    if (pilhaVazia(pilha)) {
        printf("❌ ERRO: Pilha vazia! Nenhuma peça reservada para usar.\n");
        return;
    }
    
    Peca pecaUsada = desempilhar(pilha);
    printf("✅ Peça reservada [%c %d] usada!\n", pecaUsada.nome, pecaUsada.id);
}

// Troca a peça da frente da fila com o topo da pilha
void trocarPecaUnica(FilaPecas *fila, PilhaPecas *pilha) {
    printf("\n➤ Ação: Trocar peça da frente da fila com o topo da pilha\n");
    
    if (filaVazia(fila)) {
        printf("❌ ERRO: Fila vazia! Não é possível fazer troca.\n");
        return;
    }
    
    if (pilhaVazia(pilha)) {
        printf("❌ ERRO: Pilha vazia! Não é possível fazer troca.\n");
        return;
    }
    
    // Remove as peças temporariamente
    Peca pecaFila = removerDaFila(fila);
    Peca pecaPilha = desempilhar(pilha);
    
    printf("🔁 Trocando [%c %d] (fila) ↔ [%c %d] (pilha)\n",
           pecaFila.nome, pecaFila.id,
           pecaPilha.nome, pecaPilha.id);
    
    // Insere de volta nas posições trocadas
    empilhar(pilha, pecaFila);
    inserirNaFila(fila, pecaPilha);
    
    // Ajusta a fila para manter a ordem
    // (a peça inserida vai para o final, precisamos movê-la para a frente)
    Peca temp = fila->elementos[fila->frente];
    fila->elementos[fila->frente] = fila->elementos[fila->tras];
    fila->elementos[fila->tras] = temp;
    
    printf("✅ Troca realizada com sucesso!\n");
}

// Troca múltipla: 3 primeiras da fila com 3 da pilha
void trocaMultipla(FilaPecas *fila, PilhaPecas *pilha) {
    printf("\n➤ Ação: Troca múltipla (3 peças)\n");
    
    if (fila->tamanho < 3) {
        printf("❌ ERRO: Fila precisa ter pelo menos 3 peças para troca múltipla.\n");
        return;
    }
    
    if (pilha->tamanho < 3) {
        printf("❌ ERRO: Pilha precisa ter pelo menos 3 peças para troca múltipla.\n");
        return;
    }
    
    printf("🔄 Realizando troca em bloco...\n");
    
    // Cria arrays temporários para as peças
    Peca tempFila[3];
    Peca tempPilha[3];
    
    // Remove 3 peças da fila (frente)
    for (int i = 0; i < 3; i++) {
        tempFila[i] = removerDaFila(fila);
    }
    
    // Remove 3 peças da pilha (topo)
    for (int i = 0; i < 3; i++) {
        tempPilha[i] = desempilhar(pilha);
    }
    
    // Mostra o que está sendo trocado
    printf("Fila → Pilha: ");
    for (int i = 0; i < 3; i++) {
        printf("[%c %d] ", tempFila[i].nome, tempFila[i].id);
    }
    printf("\nPilha → Fila: ");
    for (int i = 0; i < 3; i++) {
        printf("[%c %d] ", tempPilha[i].nome, tempPilha[i].id);
    }
    printf("\n");
    
    // Insere as peças da pilha na fila (na frente)
    // Precisamos inseri-las na ordem inversa para manter a ordem original
    for (int i = 2; i >= 0; i--) {
        // Insere na frente da fila (manualmente)
        fila->frente = (fila->frente - 1 + TAMANHO_FILA) % TAMANHO_FILA;
        fila->elementos[fila->frente] = tempPilha[i];
        fila->tamanho++;
    }
    
    // Empilha as peças da fila (na ordem original)
    for (int i = 0; i < 3; i++) {
        empilhar(pilha, tempFila[i]);
    }
    
    printf("✅ Troca múltipla realizada com sucesso!\n");
}

// ========== FUNÇÕES DE EXIBIÇÃO ==========

// Exibe o estado atual da fila e da pilha
void exibirEstado(FilaPecas *fila, PilhaPecas *pilha) {
    printf("\n═══════════════════════════════════════════════════════════\n");
    printf("                 ESTADO ATUAL DO JOGO\n");
    printf("═══════════════════════════════════════════════════════════\n\n");
    
    // Exibe a fila de peças
    printf("📊 FILA DE PEÇAS (%d/%d):\n", fila->tamanho, TAMANHO_FILA);
    printf("   ");
    
    if (filaVazia(fila)) {
        printf("[ Fila vazia ]");
    } else {
        // Percorre a fila circular
        for (int i = 0; i < fila->tamanho; i++) {
            int indice = (fila->frente + i) % TAMANHO_FILA;
            Peca peca = fila->elementos[indice];
            
            // Destaca a peça da frente
            if (i == 0) {
                printf("▶[%c %d]◀ ", peca.nome, peca.id);
            } else {
                printf("[%c %d] ", peca.nome, peca.id);
            }
        }
        
        // Mostra espaços vazios
        for (int i = fila->tamanho; i < TAMANHO_FILA; i++) {
            printf("[   ] ");
        }
    }
    printf("\n\n");
    
    // Exibe a pilha de reserva
    printf("🗃️  PILHA DE RESERVA (%d/%d):\n", pilha->tamanho, TAMANHO_PILHA);
    printf("   (Topo → Base): ");
    
    if (pilhaVazia(pilha)) {
        printf("[ Pilha vazia ]");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            Peca peca = pilha->elementos[i];
            
            // Destaca o topo da pilha
            if (i == pilha->topo) {
                printf("▶[%c %d]◀ ", peca.nome, peca.id);
            } else {
                printf("[%c %d] ", peca.nome, peca.id);
            }
        }
    }
    printf("\n");
    
    // Legenda
    printf("\n📋 LEGENDA:\n");
    printf("   ▶◀ = Posição ativa (frente da fila / topo da pilha)\n");
    printf("   I = Peça I    O = Peça O    T = Peça T    L = Peça L\n");
    
    printf("\n═══════════════════════════════════════════════════════════\n");
}

// Exibe o menu de opções
void exibirMenu() {
    printf("\n╔═══════════════════════════════════════════════════════╗\n");
    printf("║                TETRIS STACK - MENU                    ║\n");
    printf("╠═══════════════════════════════════════════════════════╣\n");
    printf("║ 1. Jogar peça da frente da fila                      ║\n");
    printf("║ 2. Reservar peça (fila → pilha)                      ║\n");
    printf("║ 3. Usar peça da pilha de reserva                     ║\n");
    printf("║ 4. Trocar peça (frente fila ↔ topo pilha)            ║\n");
    printf("║ 5. Troca múltipla (3 fila ↔ 3 pilha)                 ║\n");
    printf("║ 6. Visualizar estado atual                           ║\n");
    printf("║ 0. Sair do jogo                                      ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    printf("\nEscolha uma opção (0-6): ");
}

// ============================================
// FUNÇÃO PRINCIPAL
// ============================================

int main() {
    FilaPecas fila;
    PilhaPecas pilha;
    int opcao;
    int rodando = 1;
    
    // Inicializa o sistema
    inicializarSistema();
    
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║        TETRIS STACK - GERENCIADOR DE PEÇAS           ║\n");
    printf("║         Fila Circular + Pilha de Reserva             ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n");
    printf("\n📌 REGRAS DO JOGO:\n");
    printf("   • Fila: %d peças, sempre mantida cheia\n", TAMANHO_FILA);
    printf("   • Pilha: %d peças máximo (LIFO)\n", TAMANHO_PILHA);
    printf("   • Peças removidas não retornam ao jogo\n");
    printf("═══════════════════════════════════════════════════════════\n");
    
    // Inicializa as estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    // Exibe o estado inicial
    exibirEstado(&fila, &pilha);
    
    // Loop principal do programa
    while (rodando) {
        exibirMenu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 0: // Sair
                printf("\n🎮 Encerrando o Tetris Stack...\n");
                printf("Obrigado por jogar! Até a próxima! 🚀\n");
                rodando = 0;
                break;
                
            case 1: // Jogar peça
                jogarPeca(&fila);
                exibirEstado(&fila, &pilha);
                break;
                
            case 2: // Reservar peça
                reservarPeca(&fila, &pilha);
                exibirEstado(&fila, &pilha);
                break;
                
            case 3: // Usar peça reservada
                usarPecaReservada(&pilha);
                exibirEstado(&fila, &pilha);
                break;
                
            case 4: // Trocar peça única
                trocarPecaUnica(&fila, &pilha);
                exibirEstado(&fila, &pilha);
                break;
                
            case 5: // Troca múltipla
                trocaMultipla(&fila, &pilha);
                exibirEstado(&fila, &pilha);
                break;
                
            case 6: // Visualizar estado
                exibirEstado(&fila, &pilha);
                break;
                
            default:
                printf("\n❌ Opção inválida! Por favor, escolha uma opção de 0 a 6.\n");
        }
    }
    
    return 0;
}