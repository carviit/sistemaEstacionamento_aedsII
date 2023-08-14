#include "vaga.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <dirent.h>
#include <limits.h>

void imprime_vaga(TVaga *vaga)
{
    printf("**********************************************");
    printf("\nNúmero da VAGA: ");
    printf("%d", vaga->numero);
    printf("\nPlaca do Carro: ");
    printf("%s", vaga->placa_carro_ocupado);
    printf("\n**********************************************");
}

TVaga *vaga(int numero, char *placa_carro_ocupado)
{
    TVaga *vaga = (TVaga *) malloc(sizeof(TVaga));
    if (vaga) memset(vaga, 0, sizeof(TVaga));
    vaga->numero = numero;
    strcpy(vaga->placa_carro_ocupado, placa_carro_ocupado);
    return vaga;
}

void imprime_vagas(FILE *in)
{
    printf("\n\nLendo Vagas do arquivo...\n\n");
    rewind(in);
    TVaga *vaga;
    while ((vaga = le_vaga(in)) != NULL)
    {
        imprime_vaga(vaga);
        free(vaga);
    }
}

void salva_vaga(TVaga *vaga, FILE *out)
{
    fwrite(&vaga->numero, sizeof(int), 1, out);
    fwrite(vaga->placa_carro_ocupado, sizeof(char), sizeof(vaga->placa_carro_ocupado), out);
}

TVaga *le_vaga(FILE *in)
{
    TVaga *vaga = (TVaga *) malloc(sizeof(TVaga));
    if (0 >= fread(&vaga->numero, sizeof(int), 1, in))
    {
        free(vaga);
        return NULL;
    }
    fread(vaga->placa_carro_ocupado, sizeof(char), sizeof(vaga->placa_carro_ocupado), in);
    return vaga;
}

int cria_base_desordenada_vaga(FILE *out)
{
    int numeroDeVagas = 3500;

    int vagas[numeroDeVagas];

    srand(time(NULL));

    for (int i = 0; i < numeroDeVagas; i++)
    {
        vagas[i] = i + 1;
    }

    for (int i = numeroDeVagas - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = vagas[i];
        vagas[i] = vagas[j];
        vagas[j] = temp;
    }

    for (int x = 0; x < numeroDeVagas; x++)
    {
        int codigoAleatorio = vagas[x];
        TVaga *v1 = vaga(codigoAleatorio, "N/A");
        salva_vaga(v1, out);
        free(v1);
    }

    return numeroDeVagas;
}

void busca_sequencial_vagas(FILE *in)
{
    char placa[8];
    rewind(in);
    TVaga *vaga;
    int found = 0;
    int cont = 0;

    clock_t inicio = clock();

    printf("\nInforme a placa do carro: ");
    scanf("%s", placa);

    while ((vaga = le_vaga(in)) != NULL)
    {
        cont++;
        if (strcmp(vaga->placa_carro_ocupado, placa) == 0)
        {
            printf("\n======== Vaga Encontrada ========\n\n");
            imprime_vaga(vaga);
            found = 1;
            break;
        }
        free(vaga);
    }
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    if (found)
    {

        FILE *arquivo = fopen("log.txt", "a");  // Abre o arquivo em modo append (se existir)

        if (arquivo == NULL)
        {
            // Se não for possível abrir o arquivo, imprime uma mensagem de erro e sai
            printf("Erro ao abrir o arquivo de log.\n");
            return 1;  // Retorno não zero para indicar erro
        }

        // Obter a hora e data atual
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        // Escreve as informações no arquivo
        fprintf(arquivo, "\n---- Log Vaga ----\n");
        fprintf(arquivo, "Data e hora: %s", asctime(timeinfo));  // Imprime a data e hora
        fprintf(arquivo, "Tempo de execucao do metodo de busca sequencial: %.2f segundos\n", tempo);
        fprintf(arquivo, "Numero de comparacoes: %d\n", cont);

        // Fecha o arquivo
        fclose(arquivo);

        printf("\n\nTempo de execucao da busca: %.2f segundos", tempo);
        printf("\nNumero de comparacoes: %d\n", cont);
    }
    else
    {
        printf("\n>>> Nenhuma vaga ocupada pelo carro com a placa informada!...");
        printf("\nNumero de comparacoes: %d\n", cont);
    }
}

void selection_sort_disco_vaga(FILE *arq, int tam)
{
    rewind(arq); // posiciona o cursor no início do arquivo

    clock_t start_time = clock(); // marca o tempo de início
    int comparacoes = 0; // contador de comparações

    // faz o selection sort
    for (int i = 1; i < tam; i++)
    {
        // posiciona o cursor no registro i
        fseek(arq, (i - 1) * sizeof(TVaga), SEEK_SET);
        TVaga *fi = le_vaga(arq);
        printf("\n******** Registro atual: %d, pos = %d\n", fi->numero, i);

        // procura o menor elemento do restante do arquivo (registros i+1 até tam)
        // Assume que o menor é o próximo (i + 1)
        // i + 1 sempre existe, pois o loop for vai até tam - 1
        fseek(arq, i * sizeof(TVaga), SEEK_SET);
        TVaga *fmenor = le_vaga(arq);
        int posmenor = i + 1;

        for (int j = i + 2; j <= tam; j++)
        {
            TVaga *fj = le_vaga(arq);
            comparacoes++;
            if ((fj->numero) < (fmenor->numero))
            {
                fmenor = fj;
                posmenor = j;
            }
        }

        // Troca fmenor de posição com fi, se realmente for menor
        if (fmenor->numero < fi->numero)
        {
            printf("Troca %d na posição %d por %d na posição %d\n", fi->numero, i, fmenor->numero, posmenor);
            fseek(arq, (posmenor - 1) * sizeof(TVaga), SEEK_SET);
            salva_vaga(fi, arq);
            fseek(arq, (i - 1) * sizeof(TVaga), SEEK_SET);
            salva_vaga(fmenor, arq);
        }
        else
        {
            printf("Não troca...");
        }
    }

    // descarrega o buffer para ter certeza de que os dados foram gravados
    fflush(arq);

    clock_t end_time = clock(); // marca o tempo de fim
    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC; // calcula o tempo total em segundos

    FILE *arquivo = fopen("log.txt", "a");  // Abre o arquivo em modo append (se existir)

    if (arquivo == NULL)
    {
        // Se não for possível abrir o arquivo, imprime uma mensagem de erro e sai
        printf("Erro ao abrir o arquivo de log.\n");
        return 1;  // Retorno não zero para indicar erro
    }

    // Obter a hora e data atual
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Escreve as informações no arquivo
    fprintf(arquivo, "\n---- Log Vaga ----\n");
    fprintf(arquivo, "Data e hora: %s", asctime(timeinfo));  // Imprime a data e hora
    fprintf(arquivo, "Tempo de execucao do metodo de selection sort disco: %.2f segundos\n", total_time);
    fprintf(arquivo, "Numero de comparacoes: %d\n", comparacoes);

    // Fecha o arquivo
    fclose(arquivo);

    printf("\nTempo total: %.2f segundos\n", total_time);
    printf("Número de comparações: %d\n", comparacoes);
}

// Função para trocar dois registros
void trocar_registros(TVaga *a, TVaga *b)
{
    TVaga temp = *a;
    *a = *b;
    *b = temp;
}

// Função de partição do Quick Sort
int particionar(TVaga *vagas, int esquerda, int direita)
{
    TVaga pivo = vagas[direita]; // Seleciona o último elemento como pivo
    int i = (esquerda - 1); // Índice do menor elemento

    for (int j = esquerda; j <= direita - 1; j++)
    {
        // Se o elemento atual for menor ou igual ao pivo
        if (strcmp(vagas[j].placa_carro_ocupado, pivo.placa_carro_ocupado) <= 0)
        {
            i++; // Incrementa o índice do menor elemento
            trocar_registros(&vagas[i], &vagas[j]);
        }
    }
    trocar_registros(&vagas[i + 1], &vagas[direita]);
    return (i + 1);
}

// Função principal do Quick Sort
void quick_sort(TVaga *vagas, int esquerda, int direita)
{
    if (esquerda < direita)
    {
        int indice_pivo = particionar(vagas, esquerda, direita);

        // Classifica os elementos separadamente antes e depois do índice do pivô
        quick_sort(vagas, esquerda, indice_pivo - 1);
        quick_sort(vagas, indice_pivo + 1, direita);
    }
}

// Função de ordenação que chama o Quick Sort para ordenar pelo campo placa_carro_ocupado
void ordenar_por_placa(FILE *arq, int tam)
{
    TVaga *vagas = (TVaga *)malloc(tam * sizeof(TVaga));

    // Lê os registros do arquivo
    fseek(arq, 0, SEEK_SET);
    for (int i = 0; i < tam; i++)
    {
        vagas[i] = *le_vaga(arq);
    }

    // Aplica o Quick Sort para ordenar pelo campo placa_carro_ocupado
    quick_sort(vagas, 0, tam - 1);

    // Escreve os registros ordenados de volta no arquivo
    fseek(arq, 0, SEEK_SET);
    for (int i = 0; i < tam; i++)
    {
        salva_vaga(&vagas[i], arq);
    }

    free(vagas);
}

void estacionar_carro(FILE *arq)
{
    char placa[8];
    printf("\nInforme a placa do carro: ");
    scanf("%s", placa);

    rewind(arq); // Posiciona o cursor no início do arquivo

    TVaga *vaga;
    while ((vaga = le_vaga(arq)) != NULL)
    {
        if (strcmp(vaga->placa_carro_ocupado, "N/A") == 0)
        {
            // Vaga disponível encontrada, estaciona o carro nessa vaga
            strcpy(vaga->placa_carro_ocupado, placa);
            fseek(arq, -sizeof(TVaga), SEEK_CUR);
            salva_vaga(vaga, arq);
            printf("\nCarro estacionado na vaga %d com placa %s\n", vaga->numero, placa);
            free(vaga);
            return;
        }
        free(vaga);
    }

    // Se o loop terminar sem encontrar uma vaga disponível
    printf("\n>>> Não há vagas disponíveis!...\n");
}

void saida_carro(FILE *arq)
{
    char placa[8];
    printf("\nInforme a placa do carro a ser retirado: ");
    scanf("%s", placa);

    rewind(arq); // Posiciona o cursor no início do arquivo

    TVaga *vaga;
    while ((vaga = le_vaga(arq)) != NULL)
    {
        if (strcmp(vaga->placa_carro_ocupado, placa) == 0)
        {
            // Carro encontrado na vaga, retira o carro da vaga
            strcpy(vaga->placa_carro_ocupado, "N/A");
            fseek(arq, -sizeof(TVaga), SEEK_CUR);
            salva_vaga(vaga, arq);
            printf("\nCarro com placa %s retirado da vaga %d\n", placa, vaga->numero);
            free(vaga);
            return;
        }
        free(vaga);
    }

    // Se o loop terminar sem encontrar o carro
    printf("\n>>> Carro com placa %s não encontrado em nenhuma vaga!...\n", placa);
}

TVaga *busca_binaria_vaga(FILE *arq, int tam)
{
    char placa[8];
    printf("\nInforme a placa do carro: ");
    scanf("%s", placa);

    clock_t inicio = clock();
    int left = 0, right = tam - 1;
    int cont = 0;
    TVaga *vaga;
    while (left <= right)
    {
        cont++;
        int middle = (left + right) / 2;
        fseek(arq, middle * sizeof(TVaga), SEEK_SET);
        vaga = le_vaga(arq);
        int comparacao = strcmp(vaga->placa_carro_ocupado, placa);
        if (comparacao == 0)
        {
            cont++;
            printf("\n======= V A G A   E N C O N T R A D A =======\n\n");
            imprime_vaga(vaga);
            clock_t fim = clock();
            double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
            printf("\nTempo de execução: %.2f segundos\n", tempo);
            printf("\nNúmero de comparações: %d", cont);

            FILE *arquivo = fopen("log.txt", "a");  // Abre o arquivo em modo append (se existir)

            if (arquivo == NULL)
            {
                // Se não for possível abrir o arquivo, imprime uma mensagem de erro e sai
                printf("Erro ao abrir o arquivo de log.\n");
                return 1;  // Retorno não zero para indicar erro
            }

            // Obter a hora e data atual
            time_t rawtime;
            struct tm *timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);

            // Escreve as informações no arquivo
            fprintf(arquivo, "\n---- Log Vagas ----\n");
            fprintf(arquivo, "Data e hora: %s", asctime(timeinfo));  // Imprime a data e hora
            fprintf(arquivo, "Tempo de execucao do metodo de busca binária: %.2f segundos\n", tempo);
            fprintf(arquivo, "Numero de comparacoes: %d\n", cont);

            // Fecha o arquivo
            fclose(arquivo);
            return vaga;
        }
        else if (comparacao < 0)
        {
            cont++;
            left = middle + 1;
        }
        else
        {
            right = middle - 1;
        }
    }
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\n>>> Nenhuma vaga ocupada pelo carro com a placa informada!...");
    printf("\nNúmero de comparações: %d", cont);
    return NULL;
}

int tamanho_registro_vagas()
{
    return sizeof(int) + sizeof(char) * 8 ;
}

int obter_tamanho_arquivo_vagas(FILE *arquivo)
{
    fseek(arquivo, 0, SEEK_END); // posiciona o cursor no final do arquivo
    int tamanho = ftell(arquivo); // obtém a posição atual do cursor (tamanho do arquivo)
    rewind(arquivo); // reposiciona o cursor no início do arquivo
    return tamanho;
}

// Função de seleção por substituição para ordenar as vagas no vetor por número da vaga
void selection_sort_vagas(TVaga *vagas[], int tam)
{
    for (int i = 0; i < tam - 1; i++)
    {
        int posicao_menor = i;

        // Encontra a posição do menor elemento restante
        for (int j = i + 1; j < tam; j++)
        {
            if (vagas[j]->numero < vagas[posicao_menor]->numero)
            {
                posicao_menor = j;
            }
        }

        // Troca o elemento atual com o menor elemento encontrado
        if (posicao_menor != i)
        {
            TVaga *temp = vagas[i];
            vagas[i] = vagas[posicao_menor];
            vagas[posicao_menor] = temp;
        }
    }
}

int partition_vaga(TVaga arr[], int left, int right)
{
    TVaga pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; j++)
    {
        if (arr[j].numero <= pivot.numero)
        {
            i++;
            TVaga temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    TVaga temp = arr[i + 1];
    arr[i + 1] = arr[right];
    arr[right] = temp;

    return i + 1;
}

void quicksort_vaga(TVaga arr[], int left, int right)
{
    if (left < right)
    {
        int pivot = partition_vaga(arr, left, right);
        quicksort_vaga(arr, left, pivot - 1);
        quicksort_vaga(arr, pivot + 1, right);
    }
}

int registroMemoriaVagas(int vet[6])
{
    int cont = 0;

    for (int i = 0; i < 6; ++i)
    {
        if (vet[i] == 1)
        {
            cont++;
        }
    }

    if (cont == 6)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void imprimeParticoesVagas(FILE *file, const char nomeParticao[])
{
    printf("%s \n\n", nomeParticao);

    int tamanhoRegistro = tamanho_registro_vagas();
    int totalVagas = obter_tamanho_arquivo_vagas(file) / tamanhoRegistro;

    for (int i = 0; i < totalVagas; ++i)
    {
        fseek(file, i * tamanhoRegistro, SEEK_SET);
        TVaga *aux = le_vaga(file);

        if (aux != NULL)
        {
            printf("%d ", aux->numero);
            // Você pode imprimir outros campos relevantes da vaga aqui
            free(aux); // Liberar a memória alocada por le_vaga()
        }
    }

    printf("\n================================================================\n");
}

int selecaoSubstituicaoVagas(FILE *in, char nomeDaParticao[])
{
    // Registrar o tempo de início da execução
    clock_t inicio = clock();

    // Variáveis para controle do processo de particionamento
    int numeroDeParticoes = 0, posicao = 0, posicaoMenorNumero = 0, menorNumero = INT_MAX;
    int comparacao = 0;
    int tamRegistro = tamanho_registro_vagas();
    TVaga vagas[6];
    int auxVetNumero[6] = {0, 0, 0, 0, 0, 0};

    // Colocar o ponteiro do arquivo na posição inicial
    rewind(in);

    // Obter o tamanho do arquivo de entrada
    int AuxArquivo = obter_tamanho_arquivo_vagas(in);
    int totalVagas = AuxArquivo / tamRegistro;

    // Verificar se o arquivo de entrada está vazio
    if (totalVagas == 0)
    {
        printf("O arquivo de entrada não possui registros.\n");
        return 0;
    }

    printf("Gerando partições por Seleção com Substituição...\n\n");

    // Looping da geração de partições
    while (posicao < totalVagas)
    {
        // Construir o nome da partição usando o nome base e um número
        char nomeParticao[100];
        char str1[100];
        char str2[100] = ".dat";

        snprintf(str1, sizeof(str1), "%d", numeroDeParticoes);
        strcpy(nomeParticao, nomeDaParticao);
        strcat(nomeParticao, str1);
        strcat(nomeParticao, str2);

        // Abrir a partição para escrita (modo "wb+")
        FILE *filePartition = fopen(nomeParticao, "wb+");
        if (filePartition == NULL)
        {
            printf("Erro ao criar a partição %s.\n", nomeParticao);
            return -1;
        }

        // Vetor que irá fazer o controle das posições que serão congeladas
        int vetCongelado[6] = {0, 0, 0, 0, 0, 0};

        // Loop interno para selecionar elementos para a partição
        while (posicao < totalVagas)
        {
            menorNumero = INT_MAX;

            // Encontrar o menor número dentre os números presentes no vetor auxiliar 'auxVetNumero'
            for (int i = 0; i < 6; ++i)
            {
                if (menorNumero > auxVetNumero[i] && vetCongelado[i] != 1)
                {
                    comparacao = comparacao + 1;
                    menorNumero = auxVetNumero[i];
                    posicaoMenorNumero = i;
                }
            }

            // Salvar o registro com menor número na partição
            salva_vaga(&vagas[posicaoMenorNumero], filePartition);

            // Ler próximo registro do arquivo de entrada
            if (posicao < totalVagas)
            {
                fseek(in, posicao * tamRegistro, SEEK_SET);
                TVaga *aux = le_vaga(in);
                if (aux == NULL)
                {
                    // Tratar caso ocorra erro na leitura
                    printf("Erro na leitura do registro %d.\n", posicao);
                    fclose(filePartition);
                    return -1;
                }
                posicao++;

                // Atualizar vetor auxiliar 'auxVetNumero' e vetor 'vagas' com o novo registro lido
                auxVetNumero[posicaoMenorNumero] = aux->numero;
                vagas[posicaoMenorNumero] = *aux;

                // Congelar o registro se o número for menor que o recém gravado na partição de saída
                if (aux->numero < menorNumero)
                {
                    comparacao = comparacao + 1;
                    vetCongelado[posicaoMenorNumero] = 1;
                }

                // Verificar se a memória da partição está cheia
                if (registroMemoriaVagas(vetCongelado) == 1)
                {
                    comparacao = comparacao + 1;
                    numeroDeParticoes++;
                    break;
                }
            }
        }

        // Fechar a partição criada
        fclose(filePartition);

        // Verificar se a última posição foi atingida
        if (posicao >= totalVagas)
        {
            break;
        }
    }

    // Nome da partição final a ser criada
    char nomeParticao[100];
    char str1[100];
    char str2[100] = ".dat";

    snprintf(str1, sizeof(str1), "%d", numeroDeParticoes);
    strcpy(nomeParticao, nomeDaParticao);
    strcat(nomeParticao, str1);
    strcat(nomeParticao, str2);

    // Criar a partição final
    FILE *filePartitionFinal = fopen(nomeParticao, "ab+");
    if (filePartitionFinal == NULL)
    {
        printf("Erro ao criar a partição final %s.\n", nomeParticao);
        return -1;
    }

    fclose(filePartitionFinal);

    // Looping para imprimir o conteúdo de cada partição gerada
    for (int i = 0; i <= numeroDeParticoes; ++i)
    {
        snprintf(str1, sizeof(str1), "%d", i);
        strcpy(nomeParticao, nomeDaParticao);
        strcat(nomeParticao, str1);
        strcat(nomeParticao, str2);

        FILE *filePartition = fopen(nomeParticao, "rb+");
        if (filePartition == NULL)
        {
            printf("Erro ao abrir a partição %s para impressão.\n", nomeParticao);
            return -1;
        }

        imprimeParticoesVagas(filePartition, nomeParticao);

        fclose(filePartition);
    }

    //Escreve no arquivo de LOG

    FILE *arquivo = fopen("log.txt", "a");  // Abre o arquivo em modo append (se existir)

    if (arquivo == NULL)
    {
        // Se não for possível abrir o arquivo, imprime uma mensagem de erro e sai
        printf("Erro ao abrir o arquivo de log.\n");
        return 1;  // Retorno não zero para indicar erro
    }

    // Obter a hora e data atual
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Escreve as informações no arquivo
    fprintf(arquivo, "\n---- Log Vaga ----\n");
    fprintf(arquivo, "Data e hora: %s", asctime(timeinfo));  // Imprime a data e hora
    fprintf(arquivo, "Tempo de execucao do metodo de particao: %.2f segundos\n", ((double)(clock() - inicio) / CLOCKS_PER_SEC));
    fprintf(arquivo, "Numero de comparacoes: %d\n", comparacao);

    // Fecha o arquivo
    fclose(arquivo);
    printf("\nTempo de execução: %.6f segundos\n", (double)(clock() - inicio) / CLOCKS_PER_SEC);
    printf("Número de comparações: %d\n", comparacao);

    return numeroDeParticoes;
}

// Função para realizar a intercalação otima de partições de vagas
void intercalacaoOtimaUnificadaVagas(char nomeDaParticao[], int qtdParticoes, FILE *out) {
    FILE *particoes[qtdParticoes + 1];
    TVaga *registros[qtdParticoes + 1];

    clock_t inicio = clock();
    int comparacao = 0;

    // Fase de Interlecalação Otimizada
    // Nesta fase, as partições são agrupadas em grupos de 4, e cada grupo é intercalado
    // otimizadamente para ordenar os registros de todas as partições do grupo.

    // Abre as partições de entrada e carrega os primeiros registros
    for (int i = 0; i < qtdParticoes; i++) {
        char nomeArqParticao[20];
        snprintf(nomeArqParticao, sizeof(nomeArqParticao), "%s%d.dat", nomeDaParticao, i);
        particoes[i] = fopen(nomeArqParticao, "rb");
        if (particoes[i] == NULL) {
            printf("Erro ao abrir o arquivo da partição %s\n", nomeArqParticao);
            exit(1);
        }

        registros[i] = le_vaga(particoes[i]);
    }

    // Define o tamanho do grupo para intercalação (neste caso, 4)
    int grupoSize = 4;
    int numGrupos = qtdParticoes / grupoSize;

    // Itera sobre os grupos de partições
    for (int grupo = 0; grupo < qtdParticoes; grupo += 4) {
        // Prepara um array para os registros de cada grupo
        TVaga *grupoRegistros[4 * qtdParticoes];
        int idx = 0;

        // Lê e mescla os registros de cada partição no grupo
        for (int i = grupo; i < grupo + 4 && i < qtdParticoes; i++) {
            while (registros[i] != NULL) {
                comparacao++;
                grupoRegistros[idx++] = registros[i];
                registros[i] = le_vaga(particoes[i]);

                if (idx >= 4 * qtdParticoes) {
                    break;
                }
            }
        }

        // Ordena os registros do grupo
        for (int i = 0; i < idx - 1; i++) {
            for (int j = i + 1; j < idx; j++) {
                if (grupoRegistros[i]->numero > grupoRegistros[j]->numero) {
                    comparacao++;
                    TVaga *temp = grupoRegistros[i];
                    grupoRegistros[i] = grupoRegistros[j];
                    grupoRegistros[j] = temp;
                }
            }
        }

        // Gera o nome da nova partição
        char nomeParticao[100];
        char str1[100];
        char str2[100] = ".dat";
        int numeroParticao = qtdParticoes + 1 + grupo / 4;
        sprintf(str1, "%d", numeroParticao);
        strcat(strcpy(nomeParticao, nomeDaParticao), str1);
        strcat(strcpy(nomeParticao, nomeParticao), str2);

        // Cria a nova partição e salva os registros do grupo
        FILE *filePartition = fopen(nomeParticao, "wb+");
        if (filePartition == NULL) {
            printf("Erro ao criar a partição %s\n", nomeParticao);
            exit(1);
        }

        for (int i = 0; i < idx; i++) {
            salva_vaga(grupoRegistros[i], filePartition);
        }

        fclose(filePartition);
    }
    // Fim da Fase de Interlecalação Otimizada

    // Fase de União das Partições Ordenadas
    // Nesta fase, as partições ordenadas são unificadas para formar um único arquivo de saída.

    // Abre novamente as partições ordenadas
    for (int i = 0; i < qtdParticoes; i++) {
        char nomeArqParticao[20];
        snprintf(nomeArqParticao, sizeof(nomeArqParticao), "%s%d.dat", nomeDaParticao, i);
        particoes[i] = fopen(nomeArqParticao, "rb");
        if (particoes[i] == NULL) {
            printf("Erro ao abrir o arquivo da partição %s\n", nomeArqParticao);
            exit(1);
        }

        registros[i] = le_vaga(particoes[i]);
    }

    // Realiza a união das partições ordenadas
    while (1) {
        int menorNumero = INT_MAX;
        int idxMenor = -1;

        // Encontra o menor registro entre todas as partições
        for (int i = 0; i < qtdParticoes; i++) {
            if (registros[i] != NULL && registros[i]->numero < menorNumero) {
                menorNumero = registros[i]->numero;
                idxMenor = i;
            }
        }

        if (idxMenor == -1) {
            break;
        }

        // Salva o menor registro na saída

        salva_vaga(registros[idxMenor], out);
        registros[idxMenor] = le_vaga(particoes[idxMenor]);
    }

    // Fecha as partições e remove os arquivos temporários
    for (int i = 0; i < qtdParticoes; i++) {
        fclose(particoes[i]);
        char nomeArqParticao[20];
        snprintf(nomeArqParticao, sizeof(nomeArqParticao), "%s%d.dat", nomeDaParticao, i);
        remove(nomeArqParticao);
    }

    // Fim da Fase de União das Partições Ordenadas

    // Liberação de memória, finalização do processo e salvando no log
    for (int i = 0; i < qtdParticoes; i++) {
        free(registros[i]);
    }

    // Calcula o tempo de execução
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // Escreve no arquivo de LOG
    FILE *arquivo = fopen("log.txt", "a"); // Abre o arquivo em modo append (se existir)

    if (arquivo == NULL) {
        // Se não for possível abrir o arquivo, imprime uma mensagem de erro e sai
        printf("Erro ao abrir o arquivo de log.\n");
        return; // Não há retorno de erro, apenas uma mensagem é exibida
    }

    // Obter a hora e data atual
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Escreve as informações no arquivo de LOG
    fprintf(arquivo, "\n---- Log Vaga ----\n");
    fprintf(arquivo, "Data e hora: %s", asctime(timeinfo)); // Imprime a data e hora
    fprintf(arquivo, "Tempo de execucao do metodo de intercalacao otima: %.2f segundos\n", tempo);
    fprintf(arquivo, "Numero de comparacoes: %d\n", comparacao);

    // Fecha o arquivo de LOG
    fclose(arquivo);

    // Imprime informações na saída padrão
    printf("\nTempo de execucao do metodo de intercalacao otima: %.2f segundos\n", tempo);
    printf("Numero de comparacoes: %d\n\n\n", comparacao);
}
