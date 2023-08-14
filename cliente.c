#include "cliente.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stdarg.h>
#include <dirent.h>

void imprime_cliente(TCliente *cliente)
{
    printf("**********************************************");
    printf("\nNome: ");
    printf("%s", cliente->nome);
    printf("\nCPF: ");
    printf("%d", cliente->cpf);
    printf("\nPlaca CARRO: ");
    printf("%s", cliente->carro.placa);
    printf("\nModelo do CARRO: ");
    printf("%s", cliente->carro.modelo);
    printf("\n**********************************************");
}

void imprime_clientes(FILE *in)
{
    printf("\n\nLendo Clientes do arquivo...\n\n");
    rewind(in);
    TCliente *cliente;
    while ((cliente = le_clientes(in)) != NULL)
    {
        imprime_cliente(cliente);
        free(cliente);
    }
}

int gera_codigo_cliente()
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

TCliente *le_clientes(FILE *in)
{
    TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
    if (0 >= fread(&cliente->cpf, sizeof(int), 1, in))
    {
        free(cliente);
        return NULL;
    }
    fread(cliente->nome, sizeof(char), sizeof(cliente->nome), in);
    fread(cliente->carro.placa, sizeof(char), sizeof(cliente->carro.placa), in);
    fread(cliente->carro.modelo, sizeof(char), sizeof(cliente->carro.modelo), in);
    return cliente;
}

TCliente *cliente(char *nome, int cpf, char *placa_do_carro, char *modelo)
{
    TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
    //inicializa espaço de memória com ZEROS
    if (cliente) memset(cliente, 0, sizeof(TCliente));
    //copia valores para os campos de func
    strcpy(cliente->nome, nome);
    cliente->cpf = cpf;
    strcpy(cliente->carro.placa, placa_do_carro);
    strcpy(cliente->carro.modelo, modelo);

    return cliente;
}

int tamanho_registro_cliente()
{
    return sizeof(char) * 50 + sizeof(int) + tamanho_registro_carro();
}

int tamanho_registro_carro()
{
    return sizeof(char) * 8 + sizeof(char) * 50;
}

void salva_cliente(TCliente *cliente, FILE *out)
{

    fwrite(&cliente->cpf, sizeof(int), 1, out);
    fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), out);
    fwrite(cliente->carro.placa, sizeof(char), sizeof(cliente->carro.placa), out);
    fwrite(cliente->carro.modelo, sizeof(char), sizeof(cliente->carro.modelo), out);

}

int obter_tamanho_arquivo_cliente(FILE *arquivo)
{
    fseek(arquivo, 0, SEEK_END); // posiciona o cursor no final do arquivo
    int tamanho = ftell(arquivo); // obtém a posição atual do cursor (tamanho do arquivo)
    rewind(arquivo); // reposiciona o cursor no início do arquivo
    return tamanho;
}

int cria_base_desordenada_clientes(FILE *out)
{
    int codigos[3300];

    srand(time(NULL));

    for (int i = 0; i < 3300; i++)
    {
        codigos[i] = i + 1;
    }

    for (int i = 3300 - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = codigos[i];
        codigos[i] = codigos[j];
        codigos[j] = temp;
    }

    for (int x = 0; x < 3300; x++)
    {
        int codigoAleatorio = codigos[x];
        TCliente *c = cliente("CLIENTE", codigoAleatorio, "XXX000", "SIENA");
        salva_cliente(c, out);
        free(c);
    }

    return 3300;
}

void adiciona_cliente(FILE *in, int tamanho_atual)
{
    printf("\n\nAdicionando funcionário no final do arquivo...\n\n");
    fseek(in, 0, SEEK_END); // posiciona o cursor no final do arquivo

    char nome[50];
    char modelo[50];
    char placa[8];
    int cpf;

    getchar();

    printf("\nDigite o nome do cliente: ");
    fgets(nome, sizeof(nome), stdin);

    printf("\nDigite o modelo do carro: ");
    fgets(modelo, sizeof(modelo), stdin);

    printf("\nDigite a placa do carro: ");
    fgets(placa, sizeof(placa), stdin);

    printf("\nDigite o CPF do cliente: ");
    scanf("%d", &cpf);

    TCliente *c = cliente(nome, cpf, placa, modelo);
    salva_cliente(c, in);
    free(c);

    //lê funcionário que acabou de ser gravado
    fseek(in, tamanho_registro() * tamanho_atual, SEEK_SET);
    TCliente *novo_cliente = le_clientes(in);
    if (novo_cliente != NULL)
    {
        printf("\n>>>> CLIENTE ADICIONADO <<<<\n");
        free(novo_cliente);
    }
}

TCliente* busca_binaria_cliente(FILE *arq, int tam)
{
    int cod;
    printf("\nInforme o CPF do CLIENTE: ");
    scanf("%d", &cod);

    clock_t inicio = clock();
    int left = 0, right = tam - 1;
    int cont = 0;
    TCliente* cliente;
    while(left <= right)
    {
        cont++;
        int middle = (left + right) / 2;
        fseek(arq, middle * tamanho_registro_cliente(), SEEK_SET);
        cliente = le_clientes(arq);
        if(cod == cliente->cpf)
        {
            cont++;
            printf("\n====== F U N C I O N Á R I O   E N C O N T R A D O =======\n\n");
            imprime_cliente(cliente);
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
            fprintf(arquivo, "\n---- Log Vaga ----\n");
            fprintf(arquivo, "Data e hora: %s", asctime(timeinfo));  // Imprime a data e hora
            fprintf(arquivo, "Tempo de execucao do metodo de busca sequencial: %.2f segundos\n", tempo);
            fprintf(arquivo, "Numero de comparacoes: %d\n", cont);

            // Fecha o arquivo
            fclose(arquivo);
            printf("\nTempo de execução: %.2f segundos\n", tempo);
            printf("\nNumero de comparacoes: %d", cont);
            return cliente;
        }
        else if(cliente->cpf < cod)
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

void busca_sequencial_cliente(FILE *in)
{
    int c;
    rewind(in);
    TCliente *f;
    int found = 0;
    int cont = 0;

    clock_t inicio = clock();

    printf("\nInforme o CPF do CLIENTE: ");
    scanf("%d", &c);

    while((f = le_clientes(in)) != NULL)
    {
        cont++;
        if (f->cpf == c)
        {
            printf("\n======== Cliente Encontrado ========\n\n");
            imprime_cliente(f);
            found = 1;
            break;
        }
        free(f);
    }
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    if(found)
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
        fprintf(arquivo, "\n---- Log Cliente ----\n");
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
        printf("\n>>> Cliente nao pertence a base de dados!...");
        printf("\nNumero de comparacoes: %d\n", cont);
    }
}

void insertion_sort_disco_cliente(FILE *arq, int tam)
{
    int i;
    int num_comparacoes = 0; // Variável para armazenar o número de comparações

    clock_t inicio = clock(); // Marca o tempo de início do algoritmo

    // Faz o insertion sort
    for (int j = 2; j <= tam; j++)
    {
        fseek(arq, (j - 1) * tamanho_registro_cliente(), SEEK_SET);
        TCliente *fj = le_clientes(arq);
        i = j - 1;
        fseek(arq, (i - 1) * tamanho_registro_cliente(), SEEK_SET);
        TCliente *fi = le_clientes(arq);

        while ((i > 0) && (fi->cpf > fj->cpf))
        {
            fseek(arq, i * tamanho_registro_cliente(), SEEK_SET);
            salva_cliente(fi, arq);
            i = i - 1;
            fseek(arq, (i - 1) * tamanho_registro_cliente(), SEEK_SET);
            fi = le_clientes(arq);
            num_comparacoes++; // Incrementa o número de comparações
        }

        fseek(arq, i * tamanho_registro_cliente(), SEEK_SET);
        salva_cliente(fj, arq);
    }

    fflush(arq); // Descarrega o buffer para garantir que os dados foram gravados

    clock_t fim = clock(); // Marca o tempo de fim do algoritmo
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC; // Calcula o tempo de execução em segundos

    printf("\nNúmero de comparações: %d\n", num_comparacoes);
    printf("Tempo de execução: %.2f segundos\n", tempo);
}

int partition_cliente(TCliente arr[], int left, int right)
{
    TCliente pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; j++)
    {
        if (arr[j].cpf <= pivot.cpf)
        {
            i++;
            TCliente temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    TCliente temp = arr[i + 1];
    arr[i + 1] = arr[right];
    arr[right] = temp;

    return i + 1;
}

void quicksort_cliente(TCliente arr[], int left, int right)
{
    if (left < right)
    {
        int pivot = partition_cliente(arr, left, right);
        quicksort_cliente(arr, left, pivot - 1);
        quicksort_cliente(arr, pivot + 1, right);
    }
}

//MEMÓRIA DE REGISTRO PARA FUNÇÃO SELEÇÃO COM SUBSTITUIÇÃO
int registroMemoria (int vet[6])
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

void imprimeParticoesClientes(FILE *file, const char nomeParticao[])
{
    printf("%s \n\n", nomeParticao);

    int totalClientes = obter_tamanho_arquivo_cliente(file) / tamanho_registro_cliente();

    for (int i = 0; i < totalClientes; ++i)
    {
        fseek(file, i * tamanho_registro_cliente(), SEEK_SET);
        TCliente *aux = le_clientes(file);

        if (aux != NULL)
        {
            printf("%d ", aux->cpf);
            // Você pode imprimir outros campos relevantes do cliente aqui
            free(aux); // Liberar a memória alocada por le_clientes()
        }
    }
    printf("\n================================================================\n");
}

int selecaoSubstituicaoClientes(FILE *in, char nomeDaParticao[])
{
    // Registrar o tempo de início da execução
    clock_t inicio = clock();

    // Variáveis para controle do processo de particionamento
    int numeroDeParticoes = 0, posicao = 0, posicaoMenorCpf = 0, menorCpf = 999999999;
    int comparacao = 0;  // Contagem de comparações de CPFs
    int tamRegistro = tamanho_registro_cliente();
    TCliente clientes[6];  // Array para armazenar os registros de clientes
    int auxVetCpf[6] = {0, 0, 0, 0, 0, 0};  // Vetor auxiliar para armazenar os CPFs em memória

    // Colocar o ponteiro do arquivo na posição inicial
    rewind(in);

    // Obter o tamanho do arquivo de entrada para determinar o número total de clientes
    int AuxArquivo = obter_tamanho_arquivo_cliente(in);
    int totalClientes = AuxArquivo / tamRegistro;

    // Verificar se o arquivo de entrada está vazio
    if (totalClientes == 0)
    {
        printf("O arquivo de entrada não possui registros.\n");
        return 0;
    }

    printf("Gerando partições por Seleção com Substituição...\n\n");

    // Loop para a geração de partições
    while (posicao < totalClientes)
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

        // Vetor para controle de registros congelados na memória
        int vetCongelado[6] = {0, 0, 0, 0, 0, 0};

        // Loop interno para selecionar elementos para a partição
        while (posicao < totalClientes)
        {
            // Encontrar o menor CPF dentre os CPFs presentes no vetor auxiliar 'auxVetCpf'
            menorCpf = 999999999;
            for (int i = 0; i < 6; ++i)
            {
                if (menorCpf > auxVetCpf[i] && vetCongelado[i] != 1)
                {
                    comparacao = comparacao + 1;
                    menorCpf = auxVetCpf[i];
                    posicaoMenorCpf = i;
                }
            }

            // Salvar o registro com o menor CPF na partição
            salva_cliente(&clientes[posicaoMenorCpf], filePartition);

            // Ler o próximo registro do arquivo de entrada
            if (posicao < totalClientes)
            {
                fseek(in, posicao * tamRegistro, SEEK_SET);
                TCliente *aux = le_clientes(in);
                if (aux == NULL)
                {
                    // Tratar erro na leitura
                    printf("Erro na leitura do registro %d.\n", posicao);
                    fclose(filePartition);
                    return -1;
                }
                posicao++;

                // Atualizar vetor auxiliar 'auxVetCpf' e vetor 'clientes' com o novo registro lido
                auxVetCpf[posicaoMenorCpf] = aux->cpf;
                clientes[posicaoMenorCpf] = *aux;

                // Congelar o registro se o CPF for menor que o recém gravado na partição de saída
                if (aux->cpf < menorCpf)
                {
                    comparacao = comparacao + 1;
                    vetCongelado[posicaoMenorCpf] = 1;
                }

                // Verificar se a memória da partição está cheia
                if (registroMemoria(vetCongelado) == 1)
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
        if (posicao >= totalClientes)
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

    // Criar a partição final (modo "ab+")
    FILE *filePartitionFinal = fopen(nomeParticao, "ab+");
    if (filePartitionFinal == NULL)
    {
        printf("Erro ao criar a partição final %s.\n", nomeParticao);
        return -1;
    }

    // Fechar a partição final
    fclose(filePartitionFinal);

    // Loop para imprimir o conteúdo de cada partição gerada
    for (int i = 0; i <= numeroDeParticoes; ++i)
    {
        // Construir o nome da partição
        snprintf(str1, sizeof(str1), "%d", i);
        strcpy(nomeParticao, nomeDaParticao);
        strcat(nomeParticao, str1);
        strcat(nomeParticao, str2);

        // Abrir a partição para leitura e impressão
        FILE *filePartition = fopen(nomeParticao, "rb+");
        if (filePartition == NULL)
        {
            printf("Erro ao abrir a partição %s para impressão.\n", nomeParticao);
            return -1;
        }

        // Imprimir o conteúdo da partição
        imprimeParticoesClientes(filePartition, nomeParticao);

        // Fechar a partição após a impressão
        fclose(filePartition);
    }

    // Imprimir o tempo de execução e o número de comparações
    printf("\nTempo de execução: %.6f segundos\n", (double)(clock() - inicio) / CLOCKS_PER_SEC);
    printf("Número de comparações: %d\n", comparacao);
}
void intercalacaoOtimaUnificadaClientes(char nomeDaParticao[], int qtdParticoes, FILE *out)
{
    FILE *particoes[qtdParticoes + 1];
    TCliente *registros[qtdParticoes + 1];


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

        registros[i] = le_clientes(particoes[i]);
    }

    int grupoSize = 4;
    int numGrupos = qtdParticoes / grupoSize;

    for (int grupo = 0; grupo < qtdParticoes; grupo += 4)
    {
        TCliente *grupoRegistros[4 * qtdParticoes];
        int idx = 0;

        for (int i = grupo; i < grupo + 4 && i < qtdParticoes; i++)
        {
            while (registros[i] != NULL)
            {
                comparacao++;
                grupoRegistros[idx++] = registros[i];
                registros[i] = le_clientes(particoes[i]);

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
                if (grupoRegistros[i]->cpf > grupoRegistros[j]->cpf)
                {
                    comparacao++;
                    TCliente *temp = grupoRegistros[i];
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
            salva_cliente(grupoRegistros[i], filePartition);
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

        registros[i] = le_clientes(particoes[i]);
    }

    while (1)
    {
        int menorCpf = INT_MAX;
        int idxMenor = -1;

        for (int i = 0; i < qtdParticoes; i++)
        {
            if (registros[i] != NULL && registros[i]->cpf < menorCpf)
            {
                menorCpf = registros[i]->cpf;
                idxMenor = i;
            }
        }

        if (idxMenor == -1)
        {
            break;
        }

        salva_cliente(registros[idxMenor], out);
        registros[idxMenor] = le_clientes(particoes[idxMenor]);
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
    fprintf(arquivo, "\n---- Log Cliente ----\n");
    fprintf(arquivo, "Data e hora: %s", asctime(timeinfo));  // Imprime a data e hora
    fprintf(arquivo, "Tempo de execucao do metodo de intercalacao otima: %.2f segundos\n", tempo);
    fprintf(arquivo, "Numero de comparacoes: %d\n", comparacao);

    // Fecha o arquivo
    fclose(arquivo);
}
