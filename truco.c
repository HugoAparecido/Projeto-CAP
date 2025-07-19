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
 * @brief Gera um número aleatório no intervalo [0, max], semeando o gerador uma vez.
 * @param max O valor máximo inclusivo.
 * @return Um número inteiro aleatório entre 0 e 'max'.
 */
int gerar_numero_aleatorio(int max);

/**
 * @brief Atribui um número aleatório (1-10) ao campo 'numero' da carta.
 * @param carta Ponteiro para a estrutura 'carta' a ser atualizada.
 */
void sorteio_numero_carta(struct carta *carta);

/**
 * @brief Atribui um naipe aleatório (0-3) ao campo 'naipe' da carta.
 * @param carta Ponteiro para a estrutura 'carta' a ser atualizada.
 */
void sorteio_naipe(struct carta *carta);

/**
 * @brief Cria e retorna uma nova carta com número e naipe aleatórios.
 * @return Uma nova estrutura 'carta' com valores aleatórios.
 */
struct carta criar_carta_aleatoria(void);

/**
 * @brief Solicita e armazena o nome do jogador.
 * @param jogador Ponteiro para a estrutura 'jogador' a ser atualizada.
 */
void adicionar_nome_jogador(struct jogador *jogador);

/**
 * @brief Distribui três cartas aleatórias e únicas para um jogador.
 *
 * Esta função gera três cartas aleatórias para a mão do jogador, garantindo
 * que não haja repetições com as cartas já em jogo. Ela também atualiza
 * a lista global de cartas em jogo e a contagem de cartas restantes do jogador.
 *
 * @param jogador Ponteiro para a estrutura 'jogador' que receberá as cartas.
 * @param cartas_em_jogo Array de estruturas 'carta' que armazena todas as cartas já distribuídas no jogo.
 * @param j Ponteiro para um inteiro que serve como índice para 'cartas_em_jogo', indicando a próxima posição disponível.
 */
void distribuir_cartas_jogador(struct jogador *jogador, struct carta cartas_em_jogo[], int *j);

/**
 * @brief Adiciona nomes para cada jogador em uma equipe.
 * @param time Array de estruturas 'jogador' representando a equipe.
 * @param quantidade_jogadores Número de jogadores na equipe.
 */
void adicionar_equipe(struct jogador time[], int quantidade_jogadores);

/**
 * @brief Exibe uma carta no formato de símbolo e naipe.
 * @param carta A estrutura 'carta' a ser exibida.
 */
void exibir_carta(struct carta carta);

/**
 * @brief Exibe todas as cartas na mão de um jogador.
 * @param mao Array de estruturas 'carta' representando a mão.
 * @param qtd_cartas Número de cartas na mão.
 */
void exibir_mao(struct carta mao[], int qtd_cartas);

/**
 * @brief Exibe o nome de cada jogador em uma equipe.
 * @param time Array de estruturas 'jogador' representando a equipe.
 * @param quantidade_jogadores Número de jogadores na equipe.
 */
void exibir_time(struct jogador time[], int quantidade_jogadores);

/**
 * @brief Exibe a equipe vencedora do jogo.
 * @param time Array de estruturas 'jogador' representando a equipe ganhadora.
 * @param quantidade_jogadores Número de jogadores na equipe vencedora.
 */
void exibir_ganhador(struct jogador time[], int quantidade_jogadores);

/**
 * @brief Exibe a pontuação final de ambos os times.
 * @param pontuacao_time_1 Pontuação total do Time 1.
 * @param pontuacao_time_2 Pontuação total do Time 2.
 */
void exibir_pontuacao_final(int pontuacao_time_1, int pontuacao_time_2);

/**
 * @brief Exibe a pontuação de uma partida específica de truco.
 * @param pontuacao_time_1 Pontuação do Time 1 nesta partida.
 * @param pontuacao_time_2 Pontuação do Time 2 nesta partida.
 */
void exibir_pontuacao_partida(int pontuacao_time_1, int pontuacao_time_2);

/**
 * @brief Finaliza o jogo, exibindo o time ganhador e a pontuação final.
 * @param time_1 Array de estruturas 'jogador' do Time 1.
 * @param time_2 Array de estruturas 'jogador' do Time 2.
 * @param pontuacao_time_1 Pontuação final do Time 1.
 * @param pontuacao_time_2 Pontuação final do Time 2.
 * @param qtd_jogadores_cada_time Número de jogadores em cada time.
 */
void finalizar_jogo(struct jogador time_1[], struct jogador time_2[], int pontuacao_time_1, int pontuacao_time_2, int qtd_jogadores_cada_time);
/**
 * @brief Gerencia uma rodada de truco, incluindo a jogada de cartas e determinação do vencedor da rodada.
 *
 * Esta função orquestra uma única rodada do jogo de truco. Jogadores de ambos os times
 * escolhem suas cartas (ou pedem truco/retruco), as cartas são comparadas, e o time
 * vencedor da rodada é determinado. Lida com a lógica de quem começa a jogar,
 * aceitação ou recusa de truco, e a atualização das pontuações e do time ganhador.
 *
 * @param time_1 Array de estruturas 'jogador' do Time 1.
 * @param time_2 Array de estruturas 'jogador' do Time 2.
 * @param qtd_jogadores_cada_time Número de jogadores em cada time.
 * @param qtd_pontos_time1 Ponteiro para a pontuação atual do Time 1 na partida.
 * @param qtd_pontos_time2 Ponteiro para a pontuação atual do Time 2 na partida.
 * @param valor_partida Ponteiro para o valor atual dos pontos da partida (e.g., 1, 3, 6, 9, 12).
 * @param vira Ponteiro para a carta "vira" da rodada, que define as "manilhas".
 * @param aceitou_truco Ponteiro para um booleano que indica se o truco foi aceito.
 * @param time_que_pediu_truco Ponteiro para um inteiro que indica qual time pediu truco (0 se ninguém pediu, 1 para Time 1, 2 para Time 2).
 * @param time_ganhador Ponteiro para um inteiro que indica qual time ganhou a rodada anterior (influencia quem começa a jogar).
 * @param pontos_valendo Ponteiro para um inteiro que indica quantos pontos a rodada atual está valendo.
 */
void rodada_truco(struct jogador time_1[], struct jogador time_2[], int qtd_jogadores_cada_time, int *qtd_pontos_time1, int *qtd_pontos_time2, int *valor_partida, struct carta *vira, bool *aceitou_truco, int *time_que_pediu_truco, int *time_ganhador, int *pontos_valendo);

/**
 * @brief Solicita ao jogador qual carta ele deseja jogar, exibindo as opções disponíveis.
 * @param jogador A estrutura 'jogador' cujas cartas serão exibidas.
 */
void pedir_carta_jogar(struct jogador jogador);

/**
 * @brief Remove uma carta da mão do jogador após ser jogada.
 *
 * Esta função "remove" uma carta da mão do jogador, substituindo-a pela
 * última carta restante na mão e decrementando a contagem de cartas.
 * Não mantém a ordem das cartas restantes.
 *
 * @param jogador Ponteiro para a estrutura 'jogador' cuja carta será removida.
 * @param posicao_carta_jogada A posição (índice) da carta que foi jogada na mão do jogador.
 */
void retirar_carta_jogada(struct jogador *jogador, int posicao_carta_jogada);

/**
 * @brief Permite que um jogador escolha e jogue uma carta da sua mão.
 *
 * Esta função interage com o jogador, exibindo suas cartas disponíveis
 * e solicitando a escolha de uma. Valida a entrada do usuário e, após
 * uma escolha válida, remove a carta da mão do jogador e a retorna.
 *
 * @param jogador Ponteiro para a estrutura 'jogador' que irá jogar a carta.
 * @return A estrutura 'carta' que foi escolhida e jogada pelo jogador.
 */
struct carta jogar_carta(struct jogador *jogador);

/**
 * @brief Altera o valor dos pontos da rodada para 3 (pede truco).
 * @param qtd_pontos_valendo Ponteiro para a variável que armazena os pontos atuais da rodada.
 */
void pedir_truco(int *qtd_pontos_valendo);

/**
 * @brief Pergunta ao jogador se deseja aceitar um pedido de truco.
 *
 * Esta função interage com o jogador que precisa responder ao truco,
 * solicitando uma confirmação (S/N) e atualizando o status de aceitação do truco.
 *
 * @param aceitou_truco Ponteiro para um booleano que será definido como true se o truco for aceito, false caso contrário.
 * @param jogador_que_responde A estrutura 'jogador' que está respondendo ao truco.
 */
void aceitar_truco(bool *aceitou_truco, struct jogador jogador_que_responde);

/**
 * @brief Permite que um jogador escolha uma ação (exibir cartas, jogar carta, pedir/aumentar truco).
 *
 * Esta função controla o turno de um jogador, oferecendo opções de ação
 * de acordo com o estado atual do jogo (se o truco foi pedido/aceito).
 * Ela lida com a entrada do usuário e invoca outras funções para executar a ação escolhida.
 * Retorna a carta jogada se a opção for jogar uma carta.
 *
 * @param jogador Ponteiro para a estrutura 'jogador' que está realizando a ação.
 * @param valor_partida Ponteiro para o valor atual dos pontos da partida (e.g., 1, 3, 6, 9, 12).
 * @param vira A carta "vira" da rodada, que define as "manilhas".
 * @param aceitou_truco Ponteiro para um booleano que indica se o truco foi aceito.
 * @param time_que_pediu_truco Ponteiro para um inteiro que indica qual time pediu truco (0 se ninguém pediu, 1 para Time 1, 2 para Time 2).
 * @param time_atual O identificador do time do jogador atual (1 ou 2).
 * @param proximo_jogador A estrutura 'jogador' do próximo jogador a agir (geralmente o adversário).
 * @return A estrutura 'carta' que foi jogada pelo jogador.
 */
struct carta escolher_acao(struct jogador *jogador, int *valor_partida, struct carta vira, bool *aceitou_truco, int *time_que_pediu_truco, int time_atual, struct jogador proximo_jogador);

/**
 * @brief Permite aumentar o valor dos pontos da partida (retruco/vale 6, 9, 12).
 *
 * Esta função verifica se é possível aumentar o valor da partida (limite de 12 pontos).
 * Se sim, pergunta ao jogador se ele aceita o aumento. Caso aceite, o valor da partida
 * é incrementado em 3 pontos.
 *
 * @param valor_partida Ponteiro para o valor atual dos pontos da partida.
 * @param jogador_que_responde A estrutura 'jogador' que está respondendo ao pedido de aumento.
 * @param aceitou_truco Ponteiro para um booleano que será definido como true se o aumento for aceito, false caso contrário.
 */
void aumentar_truco(int *valor_partida, struct jogador jogador_que_responde, bool *aceitou_truco);

/**
 * @brief Embaralha e distribui cartas aos jogadores, definindo a vira da rodada.
 *
 * Esta função prepara uma nova rodada do jogo. Ela zera o registro das cartas em jogo,
 * distribui três cartas aleatórias e únicas para cada jogador de ambos os times,
 * e então define a carta "vira", garantindo que ela também seja única.
 *
 * @param time_1 Array de estruturas 'jogador' do Time 1.
 * @param time_2 Array de estruturas 'jogador' do Time 2.
 * @param numero_jogadores_cada_time O número de jogadores em cada time.
 * @param cartas_em_jogo Array de estruturas 'carta' que armazena todas as cartas já distribuídas ou viradas.
 * @param qtd_cartas Quantidade total de cartas que podem estar em jogo (tamanho do array `cartas_em_jogo`).
 * @param vira Ponteiro para a estrutura 'carta' que representará a vira da rodada.
 */
void embaralhar(struct jogador time_1[], struct jogador time_2[], int numero_jogadores_cada_time, struct carta cartas_em_jogo[], int qtd_cartas, struct carta *vira);

/**
 * @brief Compara o valor de duas cartas para determinar qual é a maior, considerando a carta "vira".
 *
 * Esta função avalia a força de duas cartas de truco, levando em conta a carta "vira"
 * que define as manilhas. Retorna um caractere indicando se a 'carta_jogada' é maior,
 * se a 'carta_maior' atual é que prevalece, ou se há um empate.
 *
 * @param carta_maior A carta atualmente considerada a maior.
 * @param carta_jogada A carta que está sendo comparada.
 * @param vira A carta "vira" da rodada.
 * @return Um caractere indicando o resultado da comparação:
 * - '>' se 'carta_jogada' for maior que 'carta_maior'.
 * - '<' se 'carta_maior' for maior que 'carta_jogada'.
 * - '=' se as cartas tiverem o mesmo valor.
 */
char comparar_cartas(struct carta carta_maior, struct carta carta_jogada, struct carta vira);

/**
 * @brief Reordena os jogadores de um time para que o jogador vencedor comece a próxima rodada, e ajusta o outro time.
 *
 * Esta função é usada no truco para alterar a ordem dos jogadores nos times,
 * garantindo que o jogador que ganhou a rodada anterior (ou que será o "mão")
 * do time vencedor comece a próxima. Ela rotaciona os jogadores dentro do array
 * do time vencedor para que o jogador na `posicao_nova` vá para a primeira posição.
 * O outro time também tem sua ordem ajustada de forma correspondente.
 *
 * @param time1 Array de estruturas 'jogador' do Time 1.
 * @param time2 Array de estruturas 'jogador' do Time 2.
 * @param posicao_nova A posição (índice) do jogador vencedor no seu time que deve começar a próxima rodada.
 * @param time_novo O identificador do time que ganhou a rodada e terá seu jogador inicial ajustado (1 ou 2).
 * @param qtd_jogadores O número de jogadores em cada time.
 */
void trocar_comeca(struct jogador time1[], struct jogador time2[], int posicao_nova, int time_novo, int qtd_jogadores);

/**
 * @brief Garante que uma carta gerada não seja repetida no conjunto de cartas em jogo.
 *
 * Esta função verifica se a `nova_carta` já existe no array `cartas_em_jogo`.
 * Se for repetida, uma nova carta aleatória é gerada e o processo se repete
 * até que uma carta única seja encontrada.
 *
 * @param nova_carta A carta gerada inicialmente a ser verificada.
 * @param cartas_em_jogo Um array de estruturas 'carta' contendo todas as cartas já em uso.
 * @return Uma estrutura 'carta' que é garantidamente única em relação às 'cartas_em_jogo'.
 */
struct carta troca_repetida(struct carta nova_carta, struct carta cartas_em_jogo[]);

/**
 * @brief Verifica se uma carta já existe no conjunto de cartas em jogo.
 *
 * Esta função itera sobre um array de `cartas_em_jogo` para determinar
 * se a `nova_carta` fornecida já está presente. A busca termina quando
 * uma carta com `numero == -1` é encontrada, indicando o fim das cartas válidas.
 *
 * @param nova_carta A carta a ser verificada quanto à repetição.
 * @param cartas_em_jogo Um array de estruturas 'carta' que contém as cartas já distribuídas ou viradas.
 * @return `true` se a `nova_carta` for encontrada no array `cartas_em_jogo`, `false` caso contrário.
 */
bool eh_repetida(struct carta nova_carta, struct carta cartas_em_jogo[]);

/**
 * @brief Zera as cartas em jogo, marcando-as como vazias ou não utilizadas.
 *
 * Esta função inicializa (ou "zera") um array de cartas, definindo o campo
 * 'numero' de cada carta como -1. Isso é útil para limpar o estado do baralho
 * ou das cartas já distribuídas no início de uma nova rodada ou jogo,
 * garantindo que não haja resquícios de rodadas anteriores.
 *
 * @param cartas Um ponteiro para o primeiro elemento do array de estruturas 'carta' a ser zerado.
 * @param qtd A quantidade de cartas no array a ser zerada.
 */
void zerar_cartas_em_jogo(struct carta *cartas, int qtd);

/**
 * @brief Compara duas cartas para verificar se são idênticas (mesmo número e naipe).
 * @param primeira_carta A primeira carta a ser comparada.
 * @param segunda_carta A segunda carta a ser comparada.
 * @return `true` se as cartas forem iguais, `false` caso contrário.
 */
bool cartas_iguais(struct carta primeira_carta, struct carta segunda_carta);

/**
 * @brief Copia os dados de um time de jogadores para outro.
 *
 * Esta função realiza uma cópia membro a membro dos jogadores do `timeb` para o `timea`.
 * Isso é útil para, por exemplo, criar um backup do estado de um time ou
 * para manipular uma cópia sem afetar o original.
 *
 * @param timea O array de estruturas 'jogador' de destino (onde os dados serão copiados).
 * @param timeb O array de estruturas 'jogador' de origem (de onde os dados serão lidos).
 * @param qtd_jogadores O número de jogadores a serem copiados.
 */
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

void finalizar_jogo(struct jogador time_1[], struct jogador time_2[], int pontuacao_time_1, int pontuacao_time_2, int qtd_jogadores_cada_time)
{
    if (pontuacao_time_1 > pontuacao_time_2)
        exibir_ganhador(time_1, qtd_jogadores_cada_time);
    else
        exibir_ganhador(time_2, qtd_jogadores_cada_time);

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
                {
                    pedir_truco(valor_partida);
                    aceitar_truco(aceitou_truco, proximo_jogador);
                    *valor_partida = !(*aceitou_truco) ? 1 : (*valor_partida);
                    if (!(*aceitou_truco))
                        opcao = 2;
                }
                else
                {
                    aumentar_truco(valor_partida, proximo_jogador, aceitou_truco);
                    *valor_partida = !(*aceitou_truco) ? 1 : (*valor_partida);
                    if (!(*aceitou_truco))
                        opcao = 2;
                }
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