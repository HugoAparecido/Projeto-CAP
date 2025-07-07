#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int gerador_semeado = 0;
const char NAIPES[] = {'O', 'E', 'C', 'P'};

struct carta
{
    int numero;
    int naipe;
};

struct jogador
{
    char nome[50];
    struct carta mao[3];
    int qtd_cartas_restantes;
};

// Protótipos das Funções

/*
 * @brief Gera um número aleatório dentro de um intervalo específico, semeando o gerador uma vez.
 * @param max O valor máximo inclusivo.
 * @return Um número inteiro aleatório entre 0 e 'max'.
 */
int gerar_numero_aleatorio(int max);

/*
 * @brief Atribui um número aleatório à carta.
 * @param carta Um ponteiro para a estrutura 'carta'.
 */
void sorteio_numero_carta(struct carta *carta);

/*
 * @brief Atribui um naipe aleatório à carta.
 * @param carta Um ponteiro para a estrutura 'carta'.
 */
void sorteio_naipe(struct carta *carta);

/*
 * @brief Cria uma nova carta com número e naipe aleatórios.
 * @return A estrutura 'carta' criada.
 */
struct carta criar_carta_aleatoria(void);

/*
 * @brief Solicita ao usuário o nome de um jogador e o armazena.
 * @param jogador Um ponteiro para a estrutura 'jogador'.
 */
void adicionar_nome_jogador(struct jogador *jogador);

/*
 * @brief Distribui 3 cartas aleatórias para a mão de um jogador.
 * @param jogador Um ponteiro para a estrutura 'jogador'.
 */
void distribuir_cartas_jogador(struct jogador *jogador);

/*
 * @brief Adiciona jogadores a uma equipe, obtém seus nomes e distribui as cartas iniciais.
 * @param time Um array de estruturas 'jogador'.
 * @param quantidade_jogadores O número de jogadores na equipe.
 */
void adicionar_equipe(struct jogador time[], int quantidade_jogadores);

/*
 * @brief Exibe uma única carta no formato "númeroNaipe".
 * @param carta A estrutura 'carta' a ser exibida.
 */
void exibir_carta(struct carta carta);

/*
 * @brief Exibe todas as cartas na mão de um jogador.
 * @param mao Um array de estruturas 'carta' representando a mão.
 * @param qtd_cartas O número de cartas na mão.
 */
void exibir_mao(struct carta mao[], int qtd_cartas);

/*
 * @brief Exibe informações detalhadas de todos os jogadores em uma equipe.
 * @param time Um array de estruturas 'jogador'.
 * @param quantidade_jogadores O número de jogadores na equipe.
 */
void exibir_time(struct jogador time[], int quantidade_jogadores);

/*
 * @brief Exibe a equipe vencedora.
 * @param time Um array de estruturas 'jogador' representando a equipe vencedora.
 * @param quantidade_jogadores O número de jogadores na equipe vencedora.
 */
void exibir_ganhador(struct jogador time[], int quantidade_jogadores);

/*
 * @brief Exibe as pontuações finais de ambas as equipes.
 * @param pontuacao_time_1 A pontuação final da Equipe 1.
 * @param pontuacao_time_2 A pontuação final da Equipe 2.
 */
void exibir_pontuacao_final(int pontuacao_time_1, int pontuacao_time_2);

/*
 * @brief Gerencia a conclusão do jogo, anunciando o vencedor e as pontuações finais.
 * @param time_1 Array de 'jogador' para a Equipe 1.
 * @param time_2 Array de 'jogador' para a Equipe 2.
 * @param pontuacao_time_1 Pontuação final da Equipe 1.
 * @param pontuacao_time_2 Pontuação final da Equipe 2.
 * @param qtd_jagadores_cada_time Número de jogadores por equipe.
 */
void finalizar_jogo(struct jogador time_1[], struct jogador time_2[], int pontuacao_time_1, int pontuacao_time_2, int qtd_jagadores_cada_time);

/*
 * @brief Placeholder para a lógica da rodada de Truco.
 * @param time_1 Array de 'jogador' para a Equipe 1.
 * @param time_2 Array de 'jogador' para a Equipe 2.
 * @param qtd_jogadores_cada_time Número de jogadores por equipe.
 * @param qtd_pontos_time1 Ponteiro para a pontuação da Equipe 1.
 * @param qtd_pontos_time2 Ponteiro para a pontuação da Equipe 2.
 */
void rodada_truco(struct jogador time_1[], struct jogador time_2[], int qtd_jogadores_cada_time, int *qtd_pontos_time1, int *qtd_pontos_time2);

/*
 * @brief Solicita a um jogador que escolha uma carta para jogar.
 * @param jogador A estrutura 'jogador' do jogador atual.
 */
void pedir_carta_jogar(struct jogador jogador);

void retirar_carta_jogada(struct jogador *jogador, int posicao_carta_jogada);

/*
 * @brief Lida com a jogada de uma carta por um jogador, incluindo a validação da entrada.
 * @param jogador Um ponteiro para a estrutura 'jogador'.
 * @return A estrutura 'carta' jogada pelo jogador.
 */
struct carta jogar_carta(struct jogador *jogador);

/*
 * @brief Placeholder para a lógica do pedido de 'truco'.
 * @param qtd_pontos_valendo Ponteiro para o valor atual dos pontos da rodada.
 */
void pedir_truco(int *qtd_pontos_valendo);

void distibuir_mao_novamente(struct jogador time_1[], struct jogador time_2[], int numero_jogadores_cada_time);

struct carta comparar_cartas(struct carta carta_maior, struct carta carta_jogada);

void quem_ganhou_rodada(struct jogador time[], int numero_jogadores_cada_time, int posicao_ganhador);

/*
 * @brief Função principal do programa.
 */
int main(void)
{
    int pontuacao_time_1 = 0;
    int pontuacao_time_2 = 0;
    int numero_jogadores;
    int qtd_jogadores_cada_time;

    setlocale(LC_ALL, "Portuguese");

    printf("Bem-vindo ao jogo de Truco!\n");
    printf("Por favor, informe o número de jogadores (2, 4 ou 6): ");
    scanf("%d", &numero_jogadores);

    while (getchar() != '\n')
        ;

    qtd_jogadores_cada_time = numero_jogadores / 2;

    struct jogador time_1_jogadores[qtd_jogadores_cada_time];
    struct jogador time_2_jogadores[qtd_jogadores_cada_time];

    printf("\n--- Configurando o Time 1 ---\n");
    adicionar_equipe(time_1_jogadores, qtd_jogadores_cada_time);

    printf("\n--- Configurando o Time 2 ---\n");
    adicionar_equipe(time_2_jogadores, qtd_jogadores_cada_time);

    printf("\n--- Equipes Configuradas ---\n");
    printf("\nTime 1:\n");
    exibir_time(time_1_jogadores, qtd_jogadores_cada_time);

    printf("\nTime 2:\n");
    exibir_time(time_2_jogadores, qtd_jogadores_cada_time);

    // Loop principal do jogo (atualmente comentado, pois a lógica central do jogo não está implementada)
    while (pontuacao_time_1 < 12 && pontuacao_time_2 < 12) {
        rodada_truco(time_1_jogadores, time_2_jogadores, qtd_jogadores_cada_time, &pontuacao_time_1, &pontuacao_time_2);
        printf("O jogo está %d a %d"); //Arrumar com a função
        
        
        int vitorias_time1 = 0;
        int vitorias_time2 = 0;
        int rodadas_jogadas = 0;
        int pontos_valendo = 1;
        
        while (vitorias_time1 < 2 && vitorias_time2 < 2 &&  rodadas_jogadas < 3) {
            printf("-------------- Inicio da rodada interna %d-------------", rodadas_jogadas+1);
            
            int posicao_do_jogador = 0;
            for(int i = 0; i < quantidade_jogadores; i++) {
                printf("Vez do jogador: \n"); //Arrumar com a função
                printf("O que você deseja fazer:\n 1:Exibir suas cartas\n 2:Jogar alguma carta\n 3:Pedir truco\n")
                int opçao;
                scanf("%d", opçao);
                switch (opçao) {
                    case 1: {
                        exibir_mao(jogador_atual->mao, jogador_atual->qtd_cartas_restantes);
                        break;
                    }    
                    case 2: {
                        struct carta carta_jogada = jogar_carta(jogador_atual);
                        printf("%s jogou: ", jogador_atual->nome);
                        exibir_carta(carta_jogada);
                        printf("\n");
                        break;
                    }
                    case 3:
                        pedir_truco(&pontos_valendo);
                        printf("Agora a rodada vale %d pontos!\n", pontos_valendo);
                        break;
                    
                }
            }
        posicao_do_jogador++;
        //Falta implementar o contador das vitorais do time 1 e 2 e somar no int para acabar o while
        }
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
    carta->naipe = NAIPES[gerar_numero_aleatorio(3)];
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

void distribuir_cartas_jogador(struct jogador *jogador)
{
    for (int i = 0; i < 3; i++)
    {
        jogador->mao[i] = criar_carta_aleatoria();
        jogador->qtd_cartas_restantes = 3;
    }
}

void adicionar_equipe(struct jogador time[], int quantidade_jogadores)
{
    for (int i = 0; i < quantidade_jogadores; i++)
    {
        adicionar_nome_jogador(&time[i]);
        distribuir_cartas_jogador(&time[i]);
    }
}

void exibir_carta(struct carta carta)
{
    printf("%d%c ", carta.numero, NAIPES[carta.naipe]);
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
        exibir_mao(time[i].mao, time[i].qtd_cartas_restantes);
    }
}

void exibir_ganhador(struct jogador time[], int quantidade_jogadores)
{
    printf("\n--- Ganhador do Jogo ---\n");
    exibir_time(time, quantidade_jogadores);
}

void exibir_pontuacao_final(int pontuacao_time_1, int pontuacao_time_2)
{
    printf("\n--- Pontuação Final ---\n");
    printf("Time 1: %d pontos\n", pontuacao_time_1);
    printf("Time 2: %d pontos\n", pontuacao_time_2);
}

void finalizar_jogo(struct jogador time_1[], struct jogador time_2[], int pontuacao_time_1, int pontuacao_time_2, int qtd_jagadores_cada_time)
{
    if (pontuacao_time_1 > pontuacao_time_2)
        exibir_ganhador(time_1, qtd_jagadores_cada_time);
    else
        exibir_ganhador(time_2, qtd_jagadores_cada_time);

    exibir_pontuacao_final(pontuacao_time_1, pontuacao_time_2);
}

void rodada_truco(struct jogador time_1[], struct jogador time_2[], int qtd_jogadores_cada_time, int *qtd_pontos_time1, int *qtd_pontos_time2)
{
        struct carta carta_maior = jogar_carta(time_1[0]);
    for (int i = 0; i < qtd_jagadores_cada_time - 1; i ++){
        struct carta atual = jogar_carta(time_2[i]);
        carta_maior = comparar_cartas(carta_maior, atual);
        atual = jogar_carta(time_1[i + 1]);
        carta_maior = comparar_cartas(carta_maior, atual);
    }
    carta_maior = comparar_cartas(carta_maior, jogar_carta(time_2)[quantidade_jogadores - 1]);
}

void pedir_carta_jogar(struct jogador jogador)
{
    printf("Qual carta deseja jogar?: ");
    for (int i = 0; i < jogador.qtd_cartas_restantes; i++)
        printf("Para carta %d%c digite - %d\n", jogador.mao[i].numero, NAIPES[jogador.mao[i].naipe], i + 1);
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

    retirar_carta_jogada(jogador, posicao_carta);

    return carta_jogada; // Ajusta para índice de array baseado em 0
}

void pedir_truco(int *qtd_pontos_valendo)
{
    printf("Quanto deseja pedir: 3, 6, 9?");
    scanf("%d", qtd_pontos_valendo);
}

void distibuir_mao_novamente(struct jogador time_1[], struct jogador time_2[], int numero_jogadores_cada_time)
{
    for (int i = 0; i < numero_jogadores_cada_time; i++)
    {
        distribuir_cartas_jogador(&time_1[i]);
        distribuir_cartas_jogador(&time_2[i]);
    }
}

struct carta comparar_cartas(struct carta carta_maior, struct carta carta_jogada)
{
    if (carta_jogada.numero == 4 && carta_jogada.naipe == 'P')
    {
        return carta_jogada;
    }
    if (carta_maior.numero == 4 && carta_maior.naipe == 'P')
    {
        return carta_maior;
    }
    if (carta_jogada.numero == 7 && carta_jogada.naipe == 'C')
    {
        return carta_jogada;
    }
    if (carta_maior.numero == 7 && carta_maior.naipe == 'C')
    {
        return carta_maior;
    }
    if (carta_jogada.numero == 1 && carta_jogada.naipe == 'E')
    {
        return carta_jogada;
    }
    if (carta_maior.numero == 1 && carta_maior.naipe == 'E')
    {
        return carta_maior;
    }
    if (carta_jogada.numero == 7 && carta_jogada.naipe == 'O')
    {
        return carta_jogada;
    }
    if (carta_maior.numero == 7 && carta_maior.naipe == 'O')
    {
        return carta_maior;
    }
    if (carta_jogada.numero == carta_maior.numero)
    {
        return carta_jogada.naipe > carta_maior.naipe ? carta_jogada : carta_maior;
    }
    if (carta_maior.numero == 3)
    {
        return carta_maior;
    }
    if (carta_maior.numero >= 1 && carta_maior.numero <= 2 && carta_jogada.numero >= 1 && carta_jogada.numero <= 2)
    {
        return carta_jogada.numero > carta_maior.numero ? carta_jogada : carta_maior;
    }
    if (carta_maior.numero > 3)
    {
        return carta_jogada.numero > carta_maior.numero ? carta_jogada : carta_maior;
    }

    return carta_maior;
}

void quem_ganhou_rodada(struct jogador time[], int numero_jogadores_cada_time, int posicao_ganhador)
{
    struct jogador posicao_temporaria = time[posicao_ganhador];
    time[posicao_ganhador] = time[0];
    time[0] = posicao_temporaria; 
}

