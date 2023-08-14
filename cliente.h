#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdlib.h>
#include <stdio.h>
#include "carro.h"

typedef struct Cliente
{
    char nome[50];
    int cpf;
    TCarro carro;

} TCliente;

void is(FILE *arq);

void imprime_cliente(TCliente *cliente);

void imprime_clientes(FILE *in);

TCliente *le_clientes(FILE *in);

void busca_sequencial_cliente(FILE *in);

TCliente *cliente(char *nome, int cpf, char *placa_do_carro, char *modelo);

int tamanho_registro_cliente();

int tamanho_registro_carro();

int cria_base_desordenada_clientes(FILE *out);

int gera_codigo_cliente();

TCliente* busca_binaria_cliente(FILE *arq, int tam);

void salva_cliente(TCliente *cliente, FILE *out);

void selection_sort_disco_cliente(FILE *arq, int tam);

int obter_tamanho_arquivo_cliente(FILE *arquivo);

int partition_cliente(TCliente arr[], int left, int right);

void quicksort_cliente(TCliente arr[], int left, int right);

int registroMemoria (int vet[6]);

void imprimeParticoesClientes(FILE *file, const char nomeParticao[]);

int selecaoSubstituicaoClientes(FILE *in, char nomeDaParticao[]);

void intercalacaoOtimaUnificadaClientes(char nomeDaParticao[], int qtdParticoes, FILE *out);

#endif // CLIENTE_H
