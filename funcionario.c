#include "funcionario.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <dirent.h>
#include <limits.h>

char nome[50];
char cargo[11];

void imprime(TFunc *func)
{
    printf("**********************************************");
    printf("\nFuncionario de codigo ");
    printf("%d", func->cod);
    printf("\nNome: ");
    printf("%s", func->nome);
    printf("\nCargo: ");
    printf("%s", func->cargo);
    printf("\nSalario: ");
    printf("%4.2f", func->salario);
    printf("\n**********************************************");
}

TFunc *funcionario(int cod, char *nome,char *cargo, double salario)
{
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    //inicializa espaço de memória com ZEROS
    if (func) memset(func, 0, sizeof(TFunc));
    //copia valores para os campos de func
    func->cod = cod;
    strcpy(func->nome, nome);
    strcpy(func->cargo, cargo);
    func->salario = salario;
    return func;
}

void salva(TFunc *func, FILE *out)
{
    fwrite(&func->cod, sizeof(int), 1, out);
    fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
    fwrite(func->cargo, sizeof(char), sizeof(func->cargo), out);
    fwrite(&func->salario, sizeof(double), 1, out);
}

TFunc *le(FILE *in)
{
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    if (0 >= fread(&func->cod, sizeof(int), 1, in))
    {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(func->cargo, sizeof(char), sizeof(func->cargo), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func;
}

// Retorna tamanho do funcionario em bytes
int tamanho()
{
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(char) * 15 //cpf
           + sizeof(char) * 11 //data_nascimento
           + sizeof(double); //salario
}

void imprime_funcionarios(FILE *in)
{
    printf("\n\nLendo funcionários do arquivo...\n\n");
    rewind(in);
    TFunc *f;
    while ((f = le(in)) != NULL)
    {
        imprime(f);
        free(f);

    }
}

int gera_codigo()
{
    static int codigos_retornados[1001] = {0};

    // Semente para geração de números aleatórios
    srand(time(NULL));

    // Loop para gerar e verificar os códigos
    while (1)
    {
        int codigo = rand() % 1001;

        // Verifica se o código já foi retornado antes
        if (codigos_retornados[codigo] == 0)
        {
            codigos_retornados[codigo] = 1;
            return codigo;
        }
    }
}

int obter_tamanho_arquivo(FILE *arquivo)
{
    fseek(arquivo, 0, SEEK_END); // posiciona o cursor no final do arquivo
    int tamanho = ftell(arquivo); // obtém a posição atual do cursor (tamanho do arquivo)
    rewind(arquivo); // reposiciona o cursor no início do arquivo
    return tamanho;
}

void adiciona_funcionario(FILE *in, int tamanho_atual)
{
    printf("\n\nAdicionando funcionário no final do arquivo...\n\n");
    fseek(in, 0, SEEK_END); // posiciona o cursor no final do arquivo

    int novo_codigo = gera_codigo();

    char nome[50];
    char cargo[11];
    double salario;

    getchar();

    printf("\nDigite o nome do funcionário: ");
    fgets(nome, sizeof(nome), stdin);

    printf("\nDigite o cargo do funcionário: ");
    fgets(cargo, sizeof(cargo), stdin);

    printf("\nDigite o salrio do funcionário: R$");
    scanf("%lf", &salario);

    TFunc *f = funcionario(novo_codigo, nome, cargo, salario);
    salva(f, in);
    free(f);

    //lê funcionário que acabou de ser gravado
    fseek(in, tamanho_registro() * tamanho_atual, SEEK_SET);
    TFunc *novo_funcionario = le(in);
    if (novo_funcionario != NULL)
    {
        printf("\n>>>> FUNCIONÁRIO ADICIONADO <<<<\n");
        imprime(novo_funcionario);
        free(novo_funcionario);
    }
}

void busca_sequencial(FILE *in)
{
    int c;
    rewind(in);
    TFunc *f;
    int found = 0;
    int cont = 0;

    clock_t inicio = clock();

    printf("\nInforme o codigo do funcionario: ");
    scanf("%d", &c);

    while((f = le(in)) != NULL)
    {
        cont++;
        if (f->cod == c)
        {
            printf("\n======== Funcionario Encontrado ========\n\n");
            imprime(f);
            found = 1;
            break;
        }
        free(f);
    }
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    if(found)
    {
        printf("\n\nTempo de execucao da busca: %.2f segundos", tempo);
        printf("\nNumero de comparacoes: %d\n", cont);


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
        fprintf(arquivo, "\n---- Log Funcionario ----\n");
        fprintf(arquivo, "Data e hora: %s", asctime(timeinfo));  // Imprime a data e hora
        fprintf(arquivo, "Tempo de execucao do metodo de busca sequencial: %.2f segundos\n", tempo);
        fprintf(arquivo, "Numero de comparacoes: %d\n", cont);

        // Fecha o arquivo
        fclose(arquivo);
    }
    else
    {
        printf("\n>>> Funcionario nao pertence a base de dados!...");
        printf("\nNumero de comparacoes: %d\n", cont);
    }
}

int tamanho_registro()
{
    return sizeof(int) + sizeof(char) * 50 + sizeof(char) * 11 + sizeof(double);
}

int cria_base_desordenada_funcionarios(FILE *out)
{
    int numeroDeVagas = 4000;

    int codigos[numeroDeVagas];

    srand(time(NULL));

    for (int i = 0; i < numeroDeVagas; i++)
    {
        codigos[i] = i + 1;
    }

    for (int i = numeroDeVagas - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = codigos[i];
        codigos[i] = codigos[j];
        codigos[j] = temp;
    }

    for (int x = 0; x < numeroDeVagas; x++)
    {
        int codigoAleatorio = codigos[x];
        char nome[50];
        char cargo[11];
        snprintf(nome, sizeof(nome), "Funcionario %d", x);
        snprintf(cargo, sizeof(cargo), "Funcionario");
        TFunc *f1 = funcionario(codigoAleatorio, nome, cargo, 1300.00);
        salva(f1, out);
        free(f1);
    }

    return numeroDeVagas;
}

TFunc* busca_binaria(FILE *arq, int tam)
{
    int cod;
    printf("\nInforme o codigo do funcionario: ");
    scanf("%d", &cod);

    clock_t inicio = clock();
    int left = 0, right = tam - 1;
    int cont = 0;
    TFunc* func;
    while(left <= right)
    {
        cont++;
        int middle = (left + right) / 2;
        fseek(arq, middle * tamanho_registro(), SEEK_SET);
        func = le(arq);
        if(cod == func->cod)
        {
            cont++;
            printf("\n====== F U N C I O N Á R I O   E N C O N T R A D O =======\n\n");
            imprime(func);
            clock_t fim = clock();
            double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

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
            fprintf(arquivo, "\n---- Log Funcionario ----\n");
            fprintf(arquivo, "Data e hora: %s", asctime(timeinfo));  // Imprime a data e hora
            fprintf(arquivo, "Tempo de execucao do metodo de busca binaria: %.2f segundos\n", tempo);
            fprintf(arquivo, "Numero de comparacoes: %d\n", cont);

            // Fecha o arquivo
            fclose(arquivo);
            printf("\nTempo de execução: %.2f segundos\n", tempo);
            printf("\nNumero de comparacoes: %d", cont);
            return func;
        }
        else if(func->cod < cod)
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

    printf("\n>>> Funcionario nao pertence a base de dados!...");
    printf("\nNumero de comparacoes: %d", cont);
    return NULL;
}

void insertion_sort_disco_funcionario(FILE *arq, int tam)
{
    int i;
    int num_comparacoes = 0; // Variável para armazenar o número de comparações

    clock_t inicio = clock(); // Marca o tempo de início do algoritmo

    // Faz o insertion sort
    for (int j = 2; j <= tam; j++)
    {
        fseek(arq, (j - 1) * tamanho_registro(), SEEK_SET);
        TFunc *fj = le(arq);
        i = j - 1;
        fseek(arq, (i - 1) * tamanho_registro(), SEEK_SET);
        TFunc *fi = le(arq);

        while ((i > 0) && (fi->cod > fj->cod))
        {
            fseek(arq, i * tamanho_registro(), SEEK_SET);
            salva(fi, arq);
            i = i - 1;
            fseek(arq, (i - 1) * tamanho_registro(), SEEK_SET);
            fi = le(arq);
            num_comparacoes++; // Incrementa o número de comparações
        }

        fseek(arq, i * tamanho_registro(), SEEK_SET);
        salva(fj, arq);
    }

    fflush(arq); // Descarrega o buffer para garantir que os dados foram gravados

    clock_t fim = clock(); // Marca o tempo de fim do algoritmo
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC; // Calcula o tempo de execução em segundos

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
    fprintf(arquivo, "\n---- Log Funcionario ----\n");
    fprintf(arquivo, "Data e hora: %s", asctime(timeinfo));  // Imprime a data e hora
    fprintf(arquivo, "Tempo de execucao do metodo de insertion sort disco: %.2f segundos\n", tempo);
    fprintf(arquivo, "Numero de comparacoes: %d\n", num_comparacoes);

    // Fecha o arquivo
    fclose(arquivo);
}

void insertion_sort_funcionario(TFunc *arr[], int n)
{
    int i, j;
    TFunc *key;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j]->cod > key->cod)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int partition_funcionario(TFunc arr[], int left, int right)
{
    TFunc pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; j++)
    {
        if (arr[j].cod <= pivot.cod)
        {
            i++;
            TFunc temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    TFunc temp = arr[i + 1];
    arr[i + 1] = arr[right];
    arr[right] = temp;

    return i + 1;
}

void quicksort_funcionario(TFunc arr[], int left, int right)
{
    if (left < right)
    {
        int pivot = partition_funcionario(arr, left, right);
        quicksort_funcionario(arr, left, pivot - 1);
        quicksort_funcionario(arr, pivot + 1, right);
    }
}

int registroMemoriaFuncionario(int vet[6])
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

void imprimeParticoesFuncionarios(FILE *file, const char nomeParticao[])
{
    printf("%s \n\n", nomeParticao);

    int totalFuncionarios = obter_tamanho_arquivo(file) / tamanho_registro();

    for (int i = 0; i < totalFuncionarios; ++i)
    {
        fseek(file, i * tamanho_registro(), SEEK_SET);
        TFunc *aux = le(file);

        if (aux != NULL)
        {
            printf("%d ", aux->cod);
            // Você pode imprimir outros campos relevantes do funcionário aqui
            free(aux); // Liberar a memória alocada por le()
        }
    }
    printf("\n================================================================\n");
}

int selecaoSubstituicaoFuncionarios(FILE *in, char nomeDaParticao[])
{
    clock_t inicio = clock();
    int numeroDeParticoes = 0, posicao = 0, posicaoMenorCod = 0, menorCod = INT_MAX;
    int comparacao = 0;
    int tamRegistro = tamanho_registro();
    TFunc funcionarios[6];
    int auxVetCod[6] = {0, 0, 0, 0, 0, 0};

    rewind(in);

    // Obter o tamanho do arquivo de entrada
    int AuxArquivo = obter_tamanho_arquivo(in);
    int totalFuncionarios = AuxArquivo / tamRegistro;

    if (totalFuncionarios == 0)
    {
        printf("O arquivo de entrada não possui registros.\n");
        return 0;
    }

    printf("Gerando partições por Seleção com Substituição...\n\n");

    // Looping da geração de partições
    while (posicao < totalFuncionarios)
    {
        char nomeParticao[100];
        char str1[100];
        char str2[100] = ".dat";

        snprintf(str1, sizeof(str1), "%d", numeroDeParticoes);
        strcpy(nomeParticao, nomeDaParticao);
        strcat(nomeParticao, str1);
        strcat(nomeParticao, str2);

        FILE *filePartition = fopen(nomeParticao, "wb+");
        if (filePartition == NULL)
        {
            printf("Erro ao criar a partição %s.\n", nomeParticao);
            return -1;
        }

        // Vetor que irá fazer o controle das posições que serão congeladas
        int vetCongelado[6] = {0, 0, 0, 0, 0, 0};

        // Loop interno para selecionar elementos para a partição
        while (posicao < totalFuncionarios)
        {
            menorCod = INT_MAX;

            // Encontrar o menor código (cod) dentre os códigos presentes no vetor auxiliar 'auxVetCod'
            for (int i = 0; i < 6; ++i)
            {
                if (menorCod > auxVetCod[i] && vetCongelado[i] != 1)
                {
                    comparacao = comparacao + 1;
                    menorCod = auxVetCod[i];
                    posicaoMenorCod = i;
                }
            }

            // Salvar o registro com menor código (cod) na partição
            salva(&funcionarios[posicaoMenorCod], filePartition);

            // Ler próximo registro do arquivo de entrada
            if (posicao < totalFuncionarios)
            {
                fseek(in, posicao * tamRegistro, SEEK_SET);
                TFunc *aux = le(in);
                if (aux == NULL)
                {
                    // Tratar caso ocorra erro na leitura
                    printf("Erro na leitura do registro %d.\n", posicao);
                    fclose(filePartition);
                    return -1;
                }
                posicao++;

                // Atualizar vetor auxiliar 'auxVetCod' e vetor 'funcionarios' com o novo registro lido
                auxVetCod[posicaoMenorCod] = aux->cod;
                funcionarios[posicaoMenorCod] = *aux;

                // Congelar o registro se o código (cod) for menor que o recém gravado na partição de saída
                if (aux->cod < menorCod)
                {
                    comparacao = comparacao + 1;
                    vetCongelado[posicaoMenorCod] = 1;
                }

                // Verificar se a memória da partição está cheia
                if (registroMemoriaFuncionario(vetCongelado) == 1)
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
        if (posicao >= totalFuncionarios)
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

        imprimeParticoesFuncionarios(filePartition, nomeParticao);

        fclose(filePartition);
    }

    // Imprimir o tempo e o número de comparações
    printf("\nTempo de execução: %.6f segundos\n", (double)(clock() - inicio) / CLOCKS_PER_SEC);
    printf("Número de comparações: %d\n", comparacao);

    return numeroDeParticoes;
}

void intercalacaoOtimaUnificadaFuncionarios(char nomeDaParticao[], int qtdParticoes, FILE *out)
{
    FILE *particoes[qtdParticoes + 1];
    TFunc *registros[qtdParticoes + 1];

    clock_t inicio = clock();
    int comparacao = 0;

    // Início da intercalação otimizada
    for (int i = 0; i < qtdParticoes; i++)
    {
        char nomeArqParticao[20];
        snprintf(nomeArqParticao, sizeof(nomeArqParticao), "%s%d.dat", nomeDaParticao, i);
        particoes[i] = fopen(nomeArqParticao, "rb");
        if (particoes[i] == NULL)
        {
            printf("Erro ao abrir o arquivo da partição %s\n", nomeArqParticao);
            exit(1);
        }

        registros[i] = le(particoes[i]);
    }

    int grupoSize = 4;
    int numGrupos = qtdParticoes / grupoSize;

    for (int grupo = 0; grupo < qtdParticoes; grupo += 4)
    {
        TFunc *grupoRegistros[4 * qtdParticoes];
        int idx = 0;

        for (int i = grupo; i < grupo + 4 && i < qtdParticoes; i++)
        {
            while (registros[i] != NULL)
            {
                comparacao++;
                grupoRegistros[idx++] = registros[i];
                registros[i] = le(particoes[i]);

                if (idx >= 4 * qtdParticoes)
                {
                    break;
                }
            }
        }

        for (int i = 0; i < idx - 1; i++)
        {
            for (int j = i + 1; j < idx; j++)
            {
                if (grupoRegistros[i]->cod > grupoRegistros[j]->cod)
                {
                    comparacao++;
                    TFunc *temp = grupoRegistros[i];
                    grupoRegistros[i] = grupoRegistros[j];
                    grupoRegistros[j] = temp;
                }
            }
        }

        char nomeParticao[100];
        char str1[100];
        char str2[100] = ".dat";

        int numeroParticao = qtdParticoes + 1 + grupo / 4;
        sprintf(str1, "%d", numeroParticao);
        strcat(strcpy(nomeParticao, nomeDaParticao), str1);
        strcat(strcpy(nomeParticao, nomeParticao), str2);

        FILE *filePartition = fopen(nomeParticao, "wb+");
        if (filePartition == NULL)
        {
            printf("Erro ao criar a partição %s\n", nomeParticao);
            exit(1);
        }

        for (int i = 0; i < idx; i++)
        {
            salva(grupoRegistros[i], filePartition);
        }

        fclose(filePartition);

    }
    // Fim da intercalação otimizada

    // Início da união das partições ordenadas

    for (int i = 0; i < qtdParticoes; i++)
    {
        char nomeArqParticao[20];
        snprintf(nomeArqParticao, sizeof(nomeArqParticao), "%s%d.dat", nomeDaParticao, i);
        particoes[i] = fopen(nomeArqParticao, "rb");
        if (particoes[i] == NULL)
        {
            printf("Erro ao abrir o arquivo da partição %s\n", nomeArqParticao);
            exit(1);
        }

        registros[i] = le(particoes[i]);
    }

    while (1)
    {
        int menorCod = INT_MAX;
        int idxMenor = -1;

        for (int i = 0; i < qtdParticoes; i++)
        {
            if (registros[i] != NULL && registros[i]->cod < menorCod)
            {
                menorCod = registros[i]->cod;
                idxMenor = i;
            }
        }

        if (idxMenor == -1)
        {
            break;
        }

        salva(registros[idxMenor], out);
        registros[idxMenor] = le(particoes[idxMenor]);
    }

    for (int i = 0; i < qtdParticoes; i++)
    {
        fclose(particoes[i]);
        char nomeArqParticao[20];
        snprintf(nomeArqParticao, sizeof(nomeArqParticao), "%s%d.dat", nomeDaParticao, i);
        remove(nomeArqParticao);
    }

    // Fim da união das partições ordenadas

    // Liberação de memória, finalização do processo e salvando no log
    for (int i = 0; i < qtdParticoes; i++)
    {
        free(registros[i]);
    }

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nTempo de execucao do metodo de intercalacao otima: %.2f segundos\n", tempo);
    printf("Numero de comparacoes: %d\n\n\n", comparacao);

}
