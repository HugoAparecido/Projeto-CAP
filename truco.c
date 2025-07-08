#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

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
void distribuir_cartas_jogador(struct jogador *jogador, struct carta cartas_em_jogo[], int *j);

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
void rodada_truco(struct jogador time_1[], struct jogador time_2[], int qtd_jogadores_cada_time, int *qtd_pontos_time1, int *qtd_pontos_time2, int *valor_partida, struct carta *vira);

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

struct carta escolher_acao(struct jogador *jogador, int *valor_partida, struct carta vira);

void embaralhar(struct jogador time_1[], struct jogador time_2[], int numero_jogadores_cada_time, struct carta cartas_em_jogo[], int qtd_cartas, struct carta *vira);

char comparar_cartas(struct carta carta_maior, struct carta carta_jogada, struct carta vira);

void quem_ganhou_rodada(struct jogador time[], int posicao_ganhador);

struct carta troca_repetida(struct carta nova_carta, struct carta cartas_em_jogo[]);

bool eh_repetida(struct carta nova_carta, struct carta cartas_em_jogo[]);

void zerar_cartas_em_jogo(struct carta *cartas, int qtd);

bool cartas_iguais(struct carta a, struct carta b);

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
    int qtd_carta = numero_jogadores * 3 + 1;
    struct carta cartas_em_jogo[qtd_carta];
    struct carta vira;

    struct jogador time_1_jogadores[qtd_jogadores_cada_time];
    struct jogador time_2_jogadores[qtd_jogadores_cada_time];

    printf("\n--- Configurando o Time 1 ---\n");
    adicionar_equipe(time_1_jogadores, qtd_jogadores_cada_time);

    printf("\n--- Configurando o Time 2 ---\n");
    adicionar_equipe(time_2_jogadores, qtd_jogadores_cada_time);

    // Loop principal do jogo (atualmente comentado, pois a lógica central do jogo não está implementada)
    while (pontuacao_time_1 < 12 && pontuacao_time_2 < 12)
    {
        exibir_pontuacao_final(pontuacao_time_1, pontuacao_time_2);

        int pontos_valendo = 1;

        embaralhar(time_1_jogadores, time_2_jogadores, qtd_jogadores_cada_time, cartas_em_jogo, qtd_carta, &vira);

        int vitorias_time1 = 0;
        int vitorias_time2 = 0;
        int rodadas_jogadas = 1;
        while (vitorias_time1 < 2 && vitorias_time2 < 2 && rodadas_jogadas < 4)
        {
            printf("-------------- Inicio da rodada interna %d-------------\n", rodadas_jogadas++);
            rodada_truco(time_1_jogadores, time_2_jogadores, qtd_jogadores_cada_time, &vitorias_time1, &vitorias_time2, &pontos_valendo, &vira);

            exibir_pontuacao_final(vitorias_time1, vitorias_time2);

            if (vitorias_time1 == 2)
                pontuacao_time_1 += pontos_valendo;
            else if (vitorias_time2 == 2)
                pontuacao_time_2 += pontos_valendo;

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
    }
}

void exibir_ganhador(struct jogador time[], int quantidade_jogadores)
{
    printf("\n--- Ganhador do Jogo ---\n");
    exibir_time(time, quantidade_jogadores);
}

void exibir_pontuacao_final(int pontuacao_time_1, int pontuacao_time_2)
{
    printf("\n------ Pontuação ------\n");
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

void rodada_truco(struct jogador time_1[], struct jogador time_2[], int qtd_jogadores_cada_time, int *qtd_pontos_time1, int *qtd_pontos_time2, int *valor_partida, struct carta *vira)
{
    struct carta atual_1 = escolher_acao(&time_1[0], valor_partida, *vira);
    struct carta atual_2 = escolher_acao(&time_2[0], valor_partida, *vira);
    struct carta carta_maior_1 = atual_1;
    struct carta carta_maior_2 = atual_2;
    int maior_posicao_1 = 0;
    int maior_posicao_2 = 0;
    for (int i = 0; i < qtd_jogadores_cada_time; i++)
    {
        if (comparar_cartas(atual_1,carta_maior_1, *vira) == '>')
        {
            carta_maior_1 = atual_1;
            maior_posicao_1 = i;
        }

        if (comparar_cartas(atual_2,carta_maior_2, *vira) == '>')
        {
            carta_maior_2 = atual_2;
            maior_posicao_2 = i;
        }
        if (i+1 < qtd_jogadores_cada_time)
        {
            atual_1 = escolher_acao(&time_1[i+1], valor_partida, *vira);
            atual_2 = escolher_acao(&time_2[i+1], valor_partida, *vira);
        }
    }
    if(comparar_cartas(carta_maior_1,carta_maior_2, *vira) == '='){
        (*qtd_pontos_time1)++;
        (*qtd_pontos_time2)++;
        printf("\n\n--------------------------------------------\n");
        printf("   As cartas de %s: %d%c e de %s: %d%c empacharam\n", time_1[maior_posicao_1].nome, carta_maior_1.numero, NAIPES[carta_maior_1.naipe], time_2[maior_posicao_2].nome, carta_maior_2.numero, NAIPES[carta_maior_2.naipe]);
        printf("--------------------------------------------\n");
    }
    else if (comparar_cartas(carta_maior_1, carta_maior_2, *vira) == '>')
    {
        (*qtd_pontos_time1)++;
        printf("\n\n----------------------------------\n");
        printf("   Maior Carta foi de %s: %d%c\n", time_1[maior_posicao_1].nome, carta_maior_1.numero, NAIPES[carta_maior_1.naipe]);
        printf("----------------------------------\n");
        quem_ganhou_rodada(time_1, maior_posicao_1);
    }
    else
    {
        (*qtd_pontos_time2)++;
        printf("\n\n----------------------------------\n");
        printf("   Maior Carta foi de %s: %d%c\n", time_2[maior_posicao_2].nome, carta_maior_2.numero, NAIPES[carta_maior_2.naipe]);
        printf("----------------------------------\n");
        quem_ganhou_rodada(time_2, maior_posicao_2);
    }
}

void pedir_carta_jogar(struct jogador jogador)
{
    printf("Qual carta deseja jogar?: \n");
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

    retirar_carta_jogada(jogador, posicao_carta - 1);

    return carta_jogada; // Ajusta para índice de array baseado em 0
}

void pedir_truco(int *qtd_pontos_valendo)
{
    printf("Quanto deseja pedir: 3, 6, 9?");
    scanf("%d", qtd_pontos_valendo);
}

struct carta escolher_acao(struct jogador *jogador, int *valor_partida, struct carta vira)
{
    
    int opcao = 0;
    struct carta carta_jogada;
    while (opcao != 2)
    {
        printf("\n\n--------------------------------------\n");
        printf("      Vira da rodada: %d%c\n", vira.numero, NAIPES[vira.naipe]);
        printf("--------------------------------------\n");
        printf("----------Vez de %s----------\n", jogador->nome);
        printf("O que você deseja fazer:\n 1:Exibir suas cartas\n 2:Jogar alguma carta\n 3:Pedir truco\n");
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
            pedir_truco(valor_partida);
            printf("Agora a rodada vale %d pontos!\n", *valor_partida);
            break;
        default:
            printf("Opção inválida!!!");
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
    if(vira.numero == 10)
    {
        manilha.numero = 1;
    }
    else
    {
        manilha.numero = vira.numero + 1;
    }

    char resultado;
    if(a.numero == manilha.numero && b.numero == manilha.numero)
    {
        if(a.naipe > b.naipe)
        {
            resultado = '>';
        }
        else
        {
            resultado = '<';
        }
    }
    else if(a.numero == manilha.numero)
    {
        resultado = '>';       
    }
    else if(b.numero == manilha.numero)
    {
        resultado = '<';
    }
    else if(a.numero > b.numero)
    {
        resultado = '>';
    }
    else if(b.numero > a.numero)
    {
        resultado = '<';
    }
    else
    {
        resultado = '=';
    }
    return resultado;
}

void quem_ganhou_rodada(struct jogador time[], int posicao_ganhador)
{
    struct jogador posicao_temporaria = time[posicao_ganhador];
    time[posicao_ganhador] = time[0];
    time[0] = posicao_temporaria;
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

bool cartas_iguais(struct carta a, struct carta b)
{
    return a.numero == b.numero && a.naipe == b.naipe;
}
