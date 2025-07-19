#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define NINGUEM_PEDIU_TRUCO -1   // Indica que nenhum time pediu truco na rodada.
#define TIME_INICIANTE_PARTIDA 1 // Identificador para o Time 1.
#define TIME_ADVERSARIO 2        // Identificador para o Time 2.

static int gerador_semeado = 0;                                                  // Flag para garantir que o gerador de números aleatórios seja semeado apenas uma vez.
const char NAIPES[] = {'O', 'E', 'C', 'P'};                                      // Array de caracteres representando os naipes das cartas (Ouros, Espadas, Copas, Paus).
const char SIMBOLOS[] = {'0', '4', '5', '6', '7', 'Q', 'J', 'K', 'A', '2', '3'}; // Array de caracteres representando os símbolos das cartas (valores).

// ---

struct carta
{
    int numero; // O valor numérico da carta, correspondendo ao índice em SIMBOLOS.
    int naipe;  // O naipe da carta, correspondendo ao índice em NAIPES.
};

// ---

struct jogador
{
    char nome[50];            // Nome do jogador.
    struct carta mao[3];      // As 3 cartas que o jogador possui na mão.
    int qtd_cartas_restantes; // Quantidade de cartas que o jogador ainda possui na mão.
};

// Protótipos das Funções

/**
 * @brief Gera um número aleatório dentro de um intervalo específico, semeando o gerador uma vez.
 * @param max O valor máximo inclusivo.
 * @return Um número inteiro aleatório entre 0 e 'max'.
 */
int gerar_numero_aleatorio(int max);

/**
 * @brief Atribui um número aleatório à carta.
 * @param carta Um ponteiro para a estrutura 'carta'.
 */
void sorteio_numero_carta(struct carta *carta);

/**
 * @brief Atribui um naipe aleatório à carta.
 * @param carta Um ponteiro para a estrutura 'carta'.
 */
void sorteio_naipe(struct carta *carta);

/**
 * @brief Cria uma nova carta com número e naipe aleatórios.
 * @return A estrutura 'carta' criada.
 */
struct carta criar_carta_aleatoria(void);

/**
 * @brief Solicita ao usuário o nome de um jogador e o armazena.
 * @param jogador Um ponteiro para a estrutura 'jogador'.
 */
void adicionar_nome_jogador(struct jogador *jogador);

/**
 * @brief Distribui 3 cartas aleatórias para a mão de um jogador.
 * @param jogador Um ponteiro para a estrutura 'jogador'.
 * @param cartas_em_jogo Um array das cartas já distribuídas ou em uso no jogo, para evitar repetições.
 * @param j Um ponteiro para o índice da próxima carta disponível em 'cartas_em_jogo'.
 */
void distribuir_cartas_jogador(struct jogador *jogador, struct carta cartas_em_jogo[], int *j);

/**
 * @brief Adiciona jogadores a uma equipe, obtém seus nomes e distribui as cartas iniciais.
 * @param time Um array de estruturas 'jogador'.
 * @param quantidade_jogadores O número de jogadores na equipe.
 */
void adicionar_equipe(struct jogador time[], int quantidade_jogadores);

/**
 * @brief Exibe uma única carta no formato "númeroNaipe".
 * @param carta A estrutura 'carta' a ser exibida.
 */
void exibir_carta(struct carta carta);

/**
 * @brief Exibe todas as cartas na mão de um jogador.
 * @param mao Um array de estruturas 'carta' representando a mão.
 * @param qtd_cartas O número de cartas na mão.
 */
void exibir_mao(struct carta mao[], int qtd_cartas);

/**
 * @brief Exibe informações detalhadas de todos os jogadores em uma equipe.
 * @param time Um array de estruturas 'jogador'.
 * @param quantidade_jogadores O número de jogadores na equipe.
 */
void exibir_time(struct jogador time[], int quantidade_jogadores);

/**
 * @brief Exibe a equipe vencedora.
 * @param time Um array de estruturas 'jogador' representando a equipe vencedora.
 * @param quantidade_jogadores O número de jogadores na equipe vencedora.
 */
void exibir_ganhador(struct jogador time[], int quantidade_jogadores);

/**
 * @brief Exibe as pontuações finais de ambas as equipes.
 * @param pontuacao_time_1 A pontuação final da Equipe 1.
 * @param pontuacao_time_2 A pontuação final da Equipe 2.
 */
void exibir_pontuacao_final(int pontuacao_time_1, int pontuacao_time_2);

/**
 * @brief Exibe as pontuações atuais de ambas as equipes durante a partida.
 * @param pontuacao_time_1 A pontuação atual da Equipe 1.
 * @param pontuacao_time_2 A pontuacao atual da Equipe 2.
 */
void exibir_pontuacao_partida(int pontuacao_time_1, int pontuacao_time_2);

/**
 * @brief Gerencia a conclusão do jogo, anunciando o vencedor e as pontuações finais.
 * @param time_1 Array de 'jogador' para a Equipe 1.
 * @param time_2 Array de 'jogador' para a Equipe 2.
 * @param pontuacao_time_1 Pontuação final da Equipe 1.
 * @param pontuacao_time_2 Pontuação final da Equipe 2.
 * @param qtd_jagadores_cada_time Número de jogadores por equipe.
 */
void finalizar_jogo(struct jogador time_1[], struct jogador time_2[], int pontuacao_time_1, int pontuacao_time_2, int qtd_jagadores_cada_time);

/**
 * @brief Gerencia a lógica principal de uma rodada de Truco.
 *
 * Esta função orquestra todas as etapas de uma rodada de Truco, desde a distribuição
 * das cartas até a determinação do vencedor e a atualização das pontuações.
 *
 * @param time_1 Array de estruturas 'jogador' representando a Equipe 1.
 * @param time_2 Array de estruturas 'jogador' representando a Equipe 2.
 * @param qtd_jogadores_cada_time Número de jogadores em cada equipe (e.g., 1 para manilha, 2 para truco de dupla).
 * @param qtd_pontos_time1 Ponteiro para a pontuação atual da Equipe 1.
 * @param qtd_pontos_time2 Ponteiro para a pontuação atual da Equipe 2.
 * @param valor_partida Ponteiro para o valor atual em pontos da mão (e.g., 1, 3, 6, 9, 12).
 * @param vira Ponteiro para a carta "vira" da rodada, que define as manilhas.
 * @param aceitou_truco Ponteiro para um booleano que indica se a aposta de truco foi aceita.
 * @param time_que_pediu_truco Ponteiro para o identificador do time que pediu o truco (1 ou 2).
 * @param pontos_valendo Ponteiro para o número de pontos que a rodada está valendo no momento.
 * @param time_ganhador Ponteiro para o identificador do time que ganhou a rodada (1, 2 ou 0 para empate/não definido).
 */
void rodada_truco(struct jogador time_1[], struct jogador time_2[], int qtd_jogadores_cada_time, int *qtd_pontos_time1, int *qtd_pontos_time2, int *valor_partida, struct carta *vira, bool *aceitou_truco, int *time_que_pediu_truco, int *time_ganhador, int *pontos_valendo);

/**
 * @brief Solicita a um jogador que escolha uma carta para jogar.
 * @param jogador A estrutura 'jogador' do jogador atual.
 */
void pedir_carta_jogar(struct jogador jogador);

/**
 * @brief Retira uma carta da mão do jogador após ela ter sido jogada.
 * @param jogador Um ponteiro para a estrutura 'jogador'.
 * @param posicao_carta_jogada A posição (índice) da carta jogada na mão do jogador.
 */
void retirar_carta_jogada(struct jogador *jogador, int posicao_carta_jogada);

/**
 * @brief Lida com a jogada de uma carta por um jogador, incluindo a validação da entrada.
 * @param jogador Um ponteiro para a estrutura 'jogador'.
 * @return A estrutura 'carta' jogada pelo jogador.
 */
struct carta jogar_carta(struct jogador *jogador);

/**
 * @brief Lógica para o pedido de 'truco', atualizando o valor da rodada.
 * @param qtd_pontos_valendo Ponteiro para o valor atual dos pontos da rodada.
 */
void pedir_truco(int *qtd_pontos_valendo);

/**
 * @brief Lógica para aceitar um pedido de truco.
 * @param aceitou_truco Ponteiro para um booleano que indica se o truco foi aceito.
 */
void aceitar_truco(bool *aceitou_truco, struct jogador jogador_que_responde);

/**
 * @brief Permite que um jogador escolha entre jogar uma carta ou pedir truco.
 * @param jogador Um ponteiro para a estrutura 'jogador' do jogador atual.
 * @param valor_partida Ponteiro para o valor atual dos pontos em jogo na rodada.
 * @param vira A carta "vira" da rodada.
 * @param aceitou_truco Ponteiro para um booleano que indica se o truco foi aceito.
 * @param time_que_pediu_truco Ponteiro para o identificador do time que pediu truco (1 ou 2).
 * @param time_atual O identificador do time do jogador atual (1 ou 2).
 * @param proximo_jogador A estrutura 'jogador' do próximo jogador a jogar, utilizada para algumas lógicas de decisão.
 * @return A carta jogada pelo jogador, se ele escolher jogar.
 */
struct carta escolher_acao(struct jogador *jogador, int *valor_partida, struct carta vira, bool *aceitou_truco, int *time_que_pediu_truco, int time_atual, struct jogador proximo_jogador);

/**
 * @brief Aumenta o valor da aposta do truco na partida.
 * @param valor_partida Ponteiro para o valor atual dos pontos em jogo na rodada, que será incrementado.
 */
void aumentar_truco(int *valor_partida, struct jogador jogador_que_responde, bool *aceitou_truco);

/**
 * @brief Embaralha e distribui as cartas para os jogadores e define a carta "vira".
 * @param time_1 Array de 'jogador' para a Equipe 1.
 * @param time_2 Array de 'jogador' para a Equipe 2.
 * @param numero_jogadores_cada_time O número de jogadores em cada equipe.
 * @param cartas_em_jogo Um array para armazenar as cartas que estão em uso no jogo.
 * @param qtd_cartas O número total de cartas no baralho que será usado.
 * @param vira Ponteiro para a carta "vira" da rodada.
 */
void embaralhar(struct jogador time_1[], struct jogador time_2[], int numero_jogadores_cada_time, struct carta cartas_em_jogo[], int qtd_cartas, struct carta *vira);

/**
 * @brief Compara o valor de duas cartas para determinar qual é a maior, considerando a carta "vira".
 * @param carta_maior A carta atualmente considerada a maior.
 * @param carta_jogada A carta que está sendo comparada.
 * @param vira A carta "vira" da rodada.
 * @return Um caractere indicando o resultado da comparação ('M' se carta_jogada for maior, 'm' se carta_maior for maior, 'E' se forem iguais).
 */
char comparar_cartas(struct carta carta_maior, struct carta carta_jogada, struct carta vira);

void trocar_comeca(struct jogador time1[], struct jogador time2[], int posicao_nova, int time_novo, int qtd_jogadores);

/**
 * @brief Gera uma nova carta aleatória e a retorna se ela já não estiver presente no array de cartas em jogo.
 * @param nova_carta A carta que se deseja verificar e potencialmente substituir.
 * @param cartas_em_jogo Um array das cartas já distribuídas ou em uso no jogo.
 * @return Uma nova estrutura 'carta' que não é repetida.
 */
struct carta troca_repetida(struct carta nova_carta, struct carta cartas_em_jogo[]);

/**
 * @brief Verifica se uma dada carta já existe no array de cartas em jogo.
 * @param nova_carta A carta a ser verificada.
 * @param cartas_em_jogo Um array das cartas já distribuídas ou em uso no jogo.
 * @return Verdadeiro se a carta for repetida, Falso caso contrário.
 */
bool eh_repetida(struct carta nova_carta, struct carta cartas_em_jogo[]);

/**
 * @brief Zera (limpa) um array de cartas, geralmente usado para reiniciar as cartas em jogo.
 * @param cartas Um ponteiro para o início do array de estruturas 'carta'.
 * @param qtd O número de cartas no array a serem zeradas.
 */
void zerar_cartas_em_jogo(struct carta *cartas, int qtd);

/**
 * @brief Compara duas estruturas 'carta' para verificar se são idênticas (mesmo número e naipe).
 * @param primeira_carta A primeira estrutura 'carta' para comparação.
 * @param segunda_carta A segunda estrutura 'carta' para comparação.
 * @return Verdadeiro se as cartas forem iguais, Falso caso contrário.
 */
bool cartas_iguais(struct carta primeira_carta, struct carta segunda_carta);

void copiar_time(struct jogador timea[], struct jogador timeb[], int quantidade_de_jogadoores);

/**
 * @brief Função principal do programa.
 */
int main(void)
{
    int pontuacao_time_1 = 0;
    int pontuacao_time_2 = 0;
    int numero_jogadores = 0;
    int qtd_jogadores_cada_time;

    setlocale(LC_ALL, "Portuguese");
    printf("Bem-vindo ao jogo de Truco!\n");
    printf("Por favor, informe o número de jogadores (2, 4 ou 6): ");
    char entrada[3];
    while (numero_jogadores != 2 && numero_jogadores != 4 && numero_jogadores != 6)
    {
        fgets(entrada, sizeof entrada, stdin);
        sscanf(entrada, "%d", &numero_jogadores);
        if (numero_jogadores != 2 && numero_jogadores != 4 && numero_jogadores != 6)
        {
            printf("Valor inválido!!!");
            printf("Por favor, informe o número de jogadores (2, 4 ou 6): ");
        }
    }

    qtd_jogadores_cada_time = numero_jogadores / 2;
    int qtd_carta = numero_jogadores * 3 + 1;
    struct carta cartas_em_jogo[qtd_carta];
    struct carta vira;
    int rodada = 1;
    int proximo_time = 1;

    struct jogador time_1_jogadores[qtd_jogadores_cada_time];
    struct jogador time_2_jogadores[qtd_jogadores_cada_time];

    printf("\n--- Configurando o Time 1 ---\n");
    adicionar_equipe(time_1_jogadores, qtd_jogadores_cada_time);

    printf("\n--- Configurando o Time 2 ---\n");
    adicionar_equipe(time_2_jogadores, qtd_jogadores_cada_time);

    struct jogador time_1_jogadores_original[qtd_jogadores_cada_time];
    struct jogador time_2_jogadores_original[qtd_jogadores_cada_time];

    copiar_time(time_1_jogadores_original, time_1_jogadores, qtd_jogadores_cada_time);
    copiar_time(time_2_jogadores_original, time_2_jogadores, qtd_jogadores_cada_time);

    // Loop principal do jogo (atualmente comentado, pois a lógica central do jogo não está implementada)
    while (pontuacao_time_1 < 12 && pontuacao_time_2 < 12)
    {
        printf("--------Geral--------");
        exibir_pontuacao_final(pontuacao_time_1, pontuacao_time_2);

        int valor_round = 1;
        int pontos_valendo = 1;
        int fez_primeira = 0;
        int vitorias_time1 = 0;
        int vitorias_time2 = 0;
        int rodadas_jogadas = 1;

        embaralhar(time_1_jogadores, time_2_jogadores, qtd_jogadores_cada_time, cartas_em_jogo, qtd_carta, &vira);

        bool aceitou_truco = false;

        int time_que_pediu_truco = NINGUEM_PEDIU_TRUCO;

        while (vitorias_time1 < 2 && vitorias_time2 < 2)
        {
            printf("-------------- Inicio da rodada interna %d-------------\n", rodadas_jogadas);
            rodada_truco(time_1_jogadores, time_2_jogadores, qtd_jogadores_cada_time, &vitorias_time1, &vitorias_time2, &valor_round, &vira, &aceitou_truco, &time_que_pediu_truco, &proximo_time, &pontos_valendo);

            exibir_pontuacao_partida(vitorias_time1, vitorias_time2);

            if (vitorias_time1 == 2 && vitorias_time2 == 2)
            {
                if (fez_primeira == 1)
                    pontuacao_time_1 += valor_round;
                else
                    pontuacao_time_2 += valor_round;
            }
            else if (vitorias_time1 == 2)
                pontuacao_time_1 += valor_round;
            else if (vitorias_time2 == 2)
                pontuacao_time_2 += valor_round;

            if (rodadas_jogadas == 1)
            {
                if (vitorias_time1 == 1)
                    fez_primeira = 1;
                if (vitorias_time2 == 1)
                    fez_primeira = 2;
            }
            rodadas_jogadas++;
        }
        rodada++;
        if (rodada % 2 == 0)
        {
            proximo_time = 2;
            trocar_comeca(time_1_jogadores_original, time_2_jogadores_original, 0, 2, qtd_jogadores_cada_time);
        }
        else
        {
            proximo_time = 1;
            trocar_comeca(time_2_jogadores_original, time_1_jogadores_original, 0, 2, qtd_jogadores_cada_time);
        }
        copiar_time(time_1_jogadores, time_1_jogadores_original, qtd_jogadores_cada_time);
        copiar_time(time_2_jogadores, time_2_jogadores_original, qtd_jogadores_cada_time);
    }

    finalizar_jogo(time_1_jogadores, time_2_jogadores, pontuacao_time_1, pontuacao_time_2, qtd_jogadores_cada_time);

    printf("\n--- Fim do jogo ---\n");

    return 0;
}

int gerar_numero_aleatorio(int max)
{
    if (!gerador_semeado)
    {
        srand((unsigned int)time(NULL));
        gerador_semeado = 1;
    }
    return (rand() % (max + 1));
}

void sorteio_numero_carta(struct carta *carta)
{
    carta->numero = gerar_numero_aleatorio(9) + 1;
}

void sorteio_naipe(struct carta *carta)
{
    carta->naipe = gerar_numero_aleatorio(3);
}

struct carta criar_carta_aleatoria(void)
{
    struct carta nova_carta;
    sorteio_numero_carta(&nova_carta);
    sorteio_naipe(&nova_carta);
    return nova_carta;
}

void adicionar_nome_jogador(struct jogador *jogador)
{
    printf("Digite o nome do jogador: ");
    fgets(jogador->nome, sizeof(jogador->nome), stdin);
    jogador->nome[strcspn(jogador->nome, "\n")] = '\0';
}

void distribuir_cartas_jogador(struct jogador *jogador, struct carta cartas_em_jogo[], int *j)
{
    for (int i = 0; i < 3; i++)
    {
        struct carta nova = criar_carta_aleatoria();
        nova = troca_repetida(nova, cartas_em_jogo);
        jogador->mao[i] = nova;
        cartas_em_jogo[*j] = nova;
        (*j)++;
    }
    jogador->qtd_cartas_restantes = 3;
}

void adicionar_equipe(struct jogador time[], int quantidade_jogadores)
{
    for (int i = 0; i < quantidade_jogadores; i++)
    {
        adicionar_nome_jogador(&time[i]);
    }
}

void exibir_carta(struct carta carta)
{
    printf("%c%c ", SIMBOLOS[carta.numero], NAIPES[carta.naipe]);
}

void exibir_mao(struct carta mao[], int qtd_cartas)
{
    printf("Mão: ");
    for (int i = 0; i < qtd_cartas; i++)
    {
        exibir_carta(mao[i]);
    }
    printf("\n");
}

void exibir_time(struct jogador time[], int quantidade_jogadores)
{
    for (int i = 0; i < quantidade_jogadores; i++)
    {
        printf("--- Jogador: %s ---\n", time[i].nome);
    }
}

void exibir_ganhador(struct jogador time[], int quantidade_jogadores)
{
    printf("\n--- Ganhador do Jogo ---\n");
    exibir_time(time, quantidade_jogadores);
}

void exibir_pontuacao_final(int pontuacao_time_1, int pontuacao_time_2)
{
    printf("\n------ Pontuação Global ------\n");
    printf("Time 1: %d pontos\n", pontuacao_time_1);
    printf("Time 2: %d pontos\n", pontuacao_time_2);
    printf("------------------------\n\n");
}

void exibir_pontuacao_partida(int pontuacao_time_1, int pontuacao_time_2)
{
    printf("\n------ Pontuação da Partida ------\n");
    printf("Time 1: %d pontos\n", pontuacao_time_1);
    printf("Time 2: %d pontos\n", pontuacao_time_2);
    printf("------------------------\n\n");
}

void finalizar_jogo(struct jogador time_1[], struct jogador time_2[], int pontuacao_time_1, int pontuacao_time_2, int qtd_jagadores_cada_time)
{
    if (pontuacao_time_1 > pontuacao_time_2)
        exibir_ganhador(time_1, qtd_jagadores_cada_time);
    else
        exibir_ganhador(time_2, qtd_jagadores_cada_time);

    exibir_pontuacao_final(pontuacao_time_1, pontuacao_time_2);
}

void rodada_truco(struct jogador time_1[], struct jogador time_2[], int qtd_jogadores_cada_time, int *qtd_pontos_time1, int *qtd_pontos_time2, int *valor_partida, struct carta *vira, bool *aceitou_truco, int *time_que_pediu_truco, int *time_ganhador, int *pontos_valendo)
{

    struct carta carta_maior_1, carta_maior_2;
    struct carta atual_1, atual_2;
    int maior_posicao_1 = 0, maior_posicao_2 = 0;

    // Jogadores jogam suas cartas
    if (*time_ganhador == 2)
    {
        // Time 2 começa (ganhou a rodada anterior)
        for (int i = 0; i < qtd_jogadores_cada_time; i++)
        {
            atual_2 = escolher_acao(&time_2[i], valor_partida, *vira, aceitou_truco, time_que_pediu_truco, TIME_ADVERSARIO, i + 1 != qtd_jogadores_cada_time ? time_1[i] : time_1[0]);
            if (*time_que_pediu_truco == NINGUEM_PEDIU_TRUCO || (*time_que_pediu_truco == TIME_INICIANTE_PARTIDA && *aceitou_truco))
                atual_1 = escolher_acao(&time_1[i], valor_partida, *vira, aceitou_truco, time_que_pediu_truco, 1, i + 1 != qtd_jogadores_cada_time ? time_2[i] : time_2[0]);
            if (*aceitou_truco || (!(*aceitou_truco) && *valor_partida == 1 && *time_que_pediu_truco == NINGUEM_PEDIU_TRUCO))
            {

                // Verifica maior carta do time 2
                if (i == 0 || comparar_cartas(atual_2, carta_maior_2, *vira) == '>')
                {
                    carta_maior_2 = atual_2;
                    maior_posicao_2 = i;
                }

                // Verifica maior carta do time 1
                if (i == 0 || comparar_cartas(atual_1, carta_maior_1, *vira) == '>')
                {
                    carta_maior_1 = atual_1;
                    maior_posicao_1 = i;
                }
            }
            else if (*time_que_pediu_truco == 1)
                *qtd_pontos_time1 = 2;
            else
                *qtd_pontos_time2 = 2;
        }
    }
    else
    {
        // Time 1 começa (padrão ou ganhou a rodada anterior)
        for (int i = 0; i < qtd_jogadores_cada_time; i++)
        {
            atual_1 = escolher_acao(&time_1[i], valor_partida, *vira, aceitou_truco, time_que_pediu_truco, 1, i + 1 != qtd_jogadores_cada_time ? time_2[i] : time_2[0]);
            if (*time_que_pediu_truco == NINGUEM_PEDIU_TRUCO || (*time_que_pediu_truco == TIME_INICIANTE_PARTIDA && *aceitou_truco))
                atual_2 = escolher_acao(&time_2[i], valor_partida, *vira, aceitou_truco, time_que_pediu_truco, TIME_ADVERSARIO, i + 1 != qtd_jogadores_cada_time ? time_1[i] : time_1[0]);
            if (*aceitou_truco || (!(*aceitou_truco) && *valor_partida == 1 && *time_que_pediu_truco == NINGUEM_PEDIU_TRUCO))
            {

                // Verifica maior carta do time 1
                if (i == 0 || comparar_cartas(atual_1, carta_maior_1, *vira) == '>')
                {
                    carta_maior_1 = atual_1;
                    maior_posicao_1 = i;
                }

                // Verifica maior carta do time 2
                if (i == 0 || comparar_cartas(atual_2, carta_maior_2, *vira) == '>')
                {
                    carta_maior_2 = atual_2;
                    maior_posicao_2 = i;
                }
            }
            else if (*time_que_pediu_truco == 1)
                *qtd_pontos_time1 = 2;
            else
                *qtd_pontos_time2 = 2;
        }
    }

    if (*time_que_pediu_truco == 1 && !(*aceitou_truco))
        *qtd_pontos_time1 = 2;
    if (*time_que_pediu_truco == 2 && !(*aceitou_truco))
        *qtd_pontos_time2 = 2;

    if (*aceitou_truco || (!(*aceitou_truco) && *valor_partida == 1 && *time_que_pediu_truco == NINGUEM_PEDIU_TRUCO))
    {
        int resultado = comparar_cartas(carta_maior_1, carta_maior_2, *vira);
        if (resultado == '=')
        {
            if (*qtd_pontos_time1 == 0 && *qtd_pontos_time2 == 0)
            {
                *pontos_valendo = 2; // Empate na primeira rodada vale 2 pontos
            }
            else
            {
                (*qtd_pontos_time1)++;
                (*qtd_pontos_time2)++;
            }
            printf("\n\n--------------------------------------------\n");
            printf("   Empate entre %s (%c%c) e %s (%c%c)\n",
                   time_1[maior_posicao_1].nome, SIMBOLOS[carta_maior_1.numero], NAIPES[carta_maior_1.naipe],
                   time_2[maior_posicao_2].nome, SIMBOLOS[carta_maior_2.numero], NAIPES[carta_maior_2.naipe]);
            printf("--------------------------------------------\n");
        }
        else if (resultado == '>')
        {
            *qtd_pontos_time1 += *pontos_valendo;
            // Time 1 ganhou
            printf("\n\n----------------------------------\n");
            printf("   Vitória de %s (%c%c)\n",
                   time_1[maior_posicao_1].nome, SIMBOLOS[carta_maior_1.numero], NAIPES[carta_maior_1.naipe]);
            printf("----------------------------------\n");

            // Atualiza times originais e troca ordem
            if (*time_ganhador == 1)
                trocar_comeca(time_1, time_2, maior_posicao_1, 1, qtd_jogadores_cada_time);
            else
                trocar_comeca(time_2, time_1, maior_posicao_1, 2, qtd_jogadores_cada_time);
            *time_ganhador = 1;
        }
        else
        {
            *qtd_pontos_time2 += *pontos_valendo;
            // Time 2 ganhou
            printf("\n\n----------------------------------\n");
            printf("   Vitória de %s (%c%c)\n",
                   time_2[maior_posicao_2].nome, SIMBOLOS[carta_maior_2.numero], NAIPES[carta_maior_2.naipe]);
            printf("----------------------------------\n");

            // Atualiza times originais e troca ordem
            if (*time_ganhador == 2)
                trocar_comeca(time_2, time_1, maior_posicao_2, 1, qtd_jogadores_cada_time);
            else
                trocar_comeca(time_1, time_2, maior_posicao_2, 2, qtd_jogadores_cada_time);
            *time_ganhador = 2;
        }
    }
}

void pedir_carta_jogar(struct jogador jogador)
{
    printf("Qual carta deseja jogar?: \n");
    for (int i = 0; i < jogador.qtd_cartas_restantes; i++)
        printf("Para carta %c%c digite - %d\n", SIMBOLOS[jogador.mao[i].numero], NAIPES[jogador.mao[i].naipe], i + 1);
}

void retirar_carta_jogada(struct jogador *jogador, int posicao_carta_jogada)
{
    if (posicao_carta_jogada >= 0 && posicao_carta_jogada < jogador->qtd_cartas_restantes)
    {
        // Passo 1: Trocar o valor da posição a ser removida
        // pelo valor do último elemento válido.
        // Isso simula a "remoção" sem deixar uma lacuna e sem mudar a ordem dos demais.
        jogador->mao[posicao_carta_jogada] = jogador->mao[jogador->qtd_cartas_restantes - 1];

        // Passo 2: Reduzir o tamanho lógico do array.
        // O último elemento agora está na posição removida e não precisamos mais do duplicado.
        jogador->qtd_cartas_restantes--;
    }
}

struct carta jogar_carta(struct jogador *jogador)
{
    int posicao_carta;
    do
    {
        pedir_carta_jogar(*jogador); // Exibe as opções
        scanf("%d", &posicao_carta);
        if (posicao_carta < 1 || posicao_carta > jogador->qtd_cartas_restantes)
        {
            printf("Valor inválido, digite novamente se atentando a ele.\n");
            // Limpa o buffer de entrada
            while (getchar() != '\n')
                ;
        }
    } while (posicao_carta < 1 || posicao_carta > jogador->qtd_cartas_restantes);

    struct carta carta_jogada = jogador->mao[posicao_carta - 1];

    retirar_carta_jogada(jogador, posicao_carta - 1);

    return carta_jogada; // Ajusta para índice de array baseado em 0
}

void pedir_truco(int *qtd_pontos_valendo)
{
    *qtd_pontos_valendo = 3;
}

void aceitar_truco(bool *aceitou_truco, struct jogador jogador_que_responde)
{
    printf("\n\n--------------------------------------\n");
    printf("----------Vez de %s----------\n", jogador_que_responde.nome);
    printf("Deseja aceitar o truco? (S ou N):\n");
    char resposta = 'A';
    scanf(" %c", &resposta);
    while (resposta != 'S' && resposta != 'N' && resposta != 's' && resposta != 'n')
    {
        printf("Valor indesejado, digite novamente!!\n");
        printf("Deseja aceitar o truco? (S ou N):\n");
        scanf("%c", &resposta);
    }
    *aceitou_truco = (resposta == 'S' || resposta == 's');
}

void aumentar_truco(int *valor_partida, struct jogador jogador_que_responde, bool *aceitou_truco)
{
    if (*valor_partida == 12)
        printf("Não é possível pedir pontuação maior que 12\n");
    else
    {
        printf("\n\n--------------------------------------\n");
        printf("----------Vez de %s----------\n", jogador_que_responde.nome);
        printf("Deseja aceitar o truco? (S ou N):\n");
        char resposta = 'A';
        scanf(" %c", &resposta);
        while (resposta != 'S' && resposta != 'N' && resposta != 's' && resposta != 'n')
        {
            printf("Valor indesejado, digite novamente!!\n");
            printf("Deseja aceitar o truco? (S ou N):\n");
            scanf("%c", &resposta);
        }
        *aceitou_truco = (resposta == 'S' || resposta == 's');
        if ((*aceitou_truco))
        {
            *valor_partida += 3;
            printf("Agora a partida vale %d\n", *valor_partida);
        }
    }
}

struct carta escolher_acao(struct jogador *jogador, int *valor_partida, struct carta vira, bool *aceitou_truco, int *time_que_pediu_truco, int time_atual, struct jogador proximo_jogador)
{
    int opcao = 0;
    struct carta carta_jogada;
    while (opcao != 2)
    {
        if (*valor_partida != 1 && !(*aceitou_truco))
        {
            aceitar_truco(aceitou_truco, proximo_jogador);
            *valor_partida = !(*aceitou_truco) ? 1 : (*valor_partida);
            if (!(*aceitou_truco))
                opcao = 2;
            else
                printf("Agora a partida vale %d\n", *valor_partida);
        }
        else
        {
            printf("\n\n--------------------------------------\n");
            printf("      Vira da rodada: %c%c\n", SIMBOLOS[vira.numero], NAIPES[vira.naipe]);
            printf("--------------------------------------\n");
            printf("----------Vez de %s----------\n", jogador->nome);
            printf("O que você deseja fazer:\n 1:Exibir suas cartas\n 2:Jogar alguma carta\n ");
            if (*aceitou_truco)
                printf("3:Aumentar valor do truco\n");
            else
                printf("3:Pedir truco\n");
            scanf("%d", &opcao);
            switch (opcao)
            {
            case 1:
            {
                exibir_mao(jogador->mao, jogador->qtd_cartas_restantes);
                break;
            }
            case 2:
            {
                carta_jogada = jogar_carta(jogador);
                printf("%s jogou: ", jogador->nome);
                exibir_carta(carta_jogada);
                printf("\n");
                break;
            }
            case 3:
                if (!(*aceitou_truco))
                    pedir_truco(valor_partida);
                else
                    aumentar_truco(valor_partida, proximo_jogador, aceitou_truco);
                *time_que_pediu_truco = time_atual;
                break;
            default:
                printf("Opção inválida!!!");
            }
        }
    }
    return carta_jogada;
}

void embaralhar(struct jogador time_1[], struct jogador time_2[], int numero_jogadores_cada_time, struct carta cartas_em_jogo[], int qtd_cartas, struct carta *vira)
{
    int j = 0;
    zerar_cartas_em_jogo(cartas_em_jogo, qtd_cartas);
    for (int i = 0; i < numero_jogadores_cada_time; i++)
    {
        distribuir_cartas_jogador(&time_1[i], cartas_em_jogo, &j);
        distribuir_cartas_jogador(&time_2[i], cartas_em_jogo, &j);
    }
    *vira = cartas_em_jogo[j + 1] = troca_repetida(criar_carta_aleatoria(), cartas_em_jogo);
}

char comparar_cartas(struct carta a, struct carta b, struct carta vira)
{
    struct carta manilha;
    if (vira.numero == 10)
    {
        manilha.numero = 1;
    }
    else
    {
        manilha.numero = vira.numero + 1;
    }

    char resultado;
    if (a.numero == manilha.numero && b.numero == manilha.numero)
    {
        if (a.naipe > b.naipe)
        {
            resultado = '>';
        }
        else
        {
            resultado = '<';
        }
    }
    else if (a.numero == manilha.numero)
    {
        resultado = '>';
    }
    else if (b.numero == manilha.numero)
    {
        resultado = '<';
    }
    else if (a.numero > b.numero)
    {
        resultado = '>';
    }
    else if (b.numero > a.numero)
    {
        resultado = '<';
    }
    else
    {
        resultado = '=';
    }
    return resultado;
}

struct carta troca_repetida(struct carta nova_carta, struct carta cartas_em_jogo[])
{
    while (eh_repetida(nova_carta, cartas_em_jogo))
    {
        nova_carta = criar_carta_aleatoria();
    }
    return nova_carta;
}

bool eh_repetida(struct carta nova_carta, struct carta cartas_em_jogo[])
{
    int i = 0;
    while (cartas_em_jogo[i].numero != -1)
    {
        if (cartas_iguais(nova_carta, cartas_em_jogo[i]))
        {
            return true;
        }
        i++;
    }
    return false;
}

void zerar_cartas_em_jogo(struct carta *cartas, int qtd)
{
    for (int i = 0; i < qtd; i++)
    {
        cartas[i].numero = -1;
    }
}

bool cartas_iguais(struct carta primeira_carta, struct carta segunda_carta)
{
    return primeira_carta.numero == segunda_carta.numero && primeira_carta.naipe == segunda_carta.naipe;
}

void copiar_time(struct jogador timea[], struct jogador timeb[], int qtd_jogadores)
{
    for (int i = 0; i < qtd_jogadores; i++)
    {
        timea[i] = timeb[i];
    }
}

void trocar_comeca(struct jogador time1[], struct jogador time2[], int posicao_nova, int time_novo, int qtd_jogadores)
{

    struct jogador temp[qtd_jogadores];
    int i, j = 0;
    // Garante que posicao_nova está dentro dos limites
    if (posicao_nova >= qtd_jogadores)
    {
        posicao_nova = qtd_jogadores - 1;
    }

    // Sempre troca o jogador inicial do time vencedor para a posição 0
    if (time_novo == 1)
    {

        // Copia de posicao_escolhida até o fim
        for (i = posicao_nova; i < qtd_jogadores; i++)
        {
            temp[j++] = time1[i];
        }

        // Depois, do início até posicao_escolhida - 1
        for (i = 0; i < posicao_nova; i++)
        {
            temp[j++] = time1[i];
        }

        // Copia de volta para o vetor original
        copiar_time(time1, temp, qtd_jogadores);

        j = 0;

        // Copia de posicao_escolhida até o fim
        for (i = posicao_nova; i < qtd_jogadores; i++)
        {
            temp[j++] = time2[i];
        }

        // Depois, do início até posicao_escolhida - 1
        for (i = 0; i < posicao_nova; i++)
        {
            temp[j++] = time2[i];
        }

        // Copia de volta para o vetor original
        copiar_time(time2, temp, qtd_jogadores);
    }
    else
    { // time_novo == 2

        // Copia de posicao_escolhida até o fim
        for (i = posicao_nova; i < qtd_jogadores; i++)
        {
            temp[j++] = time2[i];
        }

        // Depois, do início até posicao_escolhida - 1
        for (i = 0; i < posicao_nova; i++)
        {
            temp[j++] = time2[i];
        }

        // Copia de volta para o vetor original
        copiar_time(time2, temp, qtd_jogadores);

        j = 0;

        if (posicao_nova == qtd_jogadores)
            posicao_nova = 0;
        else
            posicao_nova++;

        // Copia de posicao_escolhida até o fim
        for (i = posicao_nova; i < qtd_jogadores; i++)
        {
            temp[j++] = time1[i];
        }

        // Depois, do início até posicao_escolhida - 1
        for (i = 0; i < posicao_nova; i++)
        {
            temp[j++] = time1[i];
        }

        // Copia de volta para o vetor original
        copiar_time(time1, temp, qtd_jogadores);
    }
}