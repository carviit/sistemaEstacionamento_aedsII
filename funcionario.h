#ifndef FUNCIONARIOS_FUNCIONARIO_H
#define FUNCIONARIOS_FUNCIONARIO_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dirent.h>

typedef struct Funcionario
{
    int cod;
    char nome[50];
    char cargo[11];
    double salario;
} TFunc;

// Imprime funcionario
void imprime(TFunc *func);

// Cria funcionario. Lembrar de usar free(funcionario)
TFunc *funcionario(int cod, char *nome, char *cargo, double salario);

// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out);

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in);

// Retorna tamanho do funcionario em bytes
int tamanho();

int gera_codigo();

void le_funcionarios(FILE *out);

int cria_base_desordenada_funcionarios(FILE *out);

void imprime_funcionarios(FILE *in);

int obter_tamanho_arquivo(FILE *arquivo);

void ordenar_por_placa(FILE *arq, int tam);

void adiciona_funcionario(FILE *in, int tamanho_atual);

void busca_sequencial(FILE *in);

TFunc* busca_binaria(FILE *arq, int tam);

void insertion_sort_disco_funcionario(FILE *arq, int tam);

void insertion_sort_funcionario(TFunc *arr[], int n);

void printParticaoCodFuncionario(FILE *file, char nomeParticao[]);

int sizeFile(FILE *file, int contSizeFile);

int partition_funcionario(TFunc arr[], int left, int right);

void quicksort_funcionario(TFunc arr[], int left, int right);

int registroMemoriaFuncionarios(int vet[6]);

void imprimeParticoesFuncionarios(FILE *file, const char nomeParticao[]);

int selecaoSubstituicaoFuncionarios(FILE *in, char nomeDaParticao[]);

void intercalacaoOtimaUnificadaFuncionarios(char nomeDaParticao[], int qtdParticoes, FILE *out);

#endif //FUNCIONARIOS_FUNCIONARIO_H
