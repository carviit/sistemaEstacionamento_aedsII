#ifndef VAGA_H
#define VAGA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Vaga
{
    int numero;
    char placa_carro_ocupado[8];

} TVaga;

void imprime_vaga(TVaga *vaga);

TVaga *vaga(int numero, char *placa_carro_ocupado);

int tamanho_registro();

void busca_sequencial_vagas(FILE *in);

void salva_vaga(TVaga *vaga, FILE *out);

TVaga *le_vaga(FILE *in);

void saida_carro(FILE *arq);

void estacionar_carro(FILE *arq);

void selection_sort_disco_vaga(FILE *arq, int tam);

void insertion_sort_disco_vaga_lexicografico(FILE *arq, int tam);

int cria_base_desordenada_vaga(FILE *out);

void imprime_vagas(FILE *in);

TVaga *busca_binaria_vaga(FILE *arq, int tam);

int partition_vaga(TVaga arr[], int left, int right);

void quicksort_vaga(TVaga arr[], int left, int right);

int registroMemoriaVagas(int vet[6]);

void imprimeParticoesVagas(FILE *file, const char nomeParticao[]);

int selecaoSubstituicaoVagas(FILE *in, char nomeDaParticao[]);

void intercalacaoOtimaUnificadaVagas(char nomeDaParticao[], int qtdParticoes, FILE *out);

#endif // VAGA_H
