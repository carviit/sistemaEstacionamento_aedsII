#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include <dirent.h>

void MSG_MENU( )
{
    printf("\n\n\t>>>>>>>>>>>>>>>>>>>>>>> OPCOES DE MENU <<<<<<<<<<<<<<<<<<<<<<<<");
    printf("\n\n\t1.  CRIAR BASE DE DADOS (4000 FUNCIONÁRIOS), (3500 VAGAS) e (3300 CLIENTES)");
    printf("  \n\t2.  ADICIONAR 1 FUNCIONÁRIO");
    printf("  \n\t3.  ADICIONAR 1 CLIENTE");
    printf("  \n\t4.  IMPRIMIR BASE DE DADOS");
    printf("  \n\t5.  ORDENAR BASE DE DADOS");
    printf("  \n\t6.  BUSCA SEQUENCIAL");
    printf("  \n\t7.  BUSCA BINÁRIA");
    printf("  \n\t8.  ESTACIONAR UM CARRO");
    printf("  \n\t9.  SAIDA DE UM CARRO");
    printf("  \n\t10. LIMPAR TELA");
    printf("  \n\t11. SAIR");
}

void MENU(FILE *outVagas, FILE *outFuncionarios, FILE *outClientes)
{
    int controle;
    int aux, aux2, base_total_vagas = 0, base_vagas, base_total_funcionarios = 0,op, base_total_clientes = 0;
    int imp, ord, seq, bina;
    TFunc *aux3;
    do
    {
        MSG_MENU();
        printf("\n\nDIGITE UMA OPCAO: ");
        fflush(stdin);
        scanf("%d", &controle);
        switch(controle)
        {
        case 1:

            base_vagas = cria_base_desordenada_vaga(outVagas);
            base_total_vagas += base_vagas;

            base_total_funcionarios += cria_base_desordenada_funcionarios(outFuncionarios);

            base_total_clientes+= cria_base_desordenada_clientes(outClientes);

            break;

        case 2:
            adiciona_funcionario(outFuncionarios, obter_tamanho_arquivo(outFuncionarios));
            base_total_funcionarios+=1;
            break;

        case 3:

            adiciona_cliente(outClientes, obter_tamanho_arquivo_cliente(outClientes));
            base_total_clientes+=1;

            break;

        case 4:

            printf("\nQual BASE de DADOS deseja IMPRIMIR\n\n");
            printf("1 ---- VAGAS\n");
            printf("2 ---- FUNCIONÁRIOS\n");
            printf("3 ---- CLIENTES");

            printf("\n\nDIGITE UMA OPCAO: ");
            fflush(stdin);
            scanf("%d", &imp);

            if(imp == 1)
            {
                imprime_vagas(outVagas);
            }
            else if(imp == 2)
            {

                imprime_funcionarios(outFuncionarios);
            }
            else
            {
                imprime_clientes(outClientes);
            }

            break;

        case 5:

            printf("\nQual BASE de DADOS deseja ORDENAR\n\n");
            printf("1 ---- VAGAS (SELEÇÃO POR SUBSTITUIÇÃO e INTERCALAÇÃO ÓTIMA) \n");
            printf("2 ---- FUNCIONÁRIOS (INSERTION SORT)\n");
            printf("3 ---- CLIENTES (INSERTION SORT)");

            printf("\n\nDIGITE UMA OPCAO: ");
            fflush(stdin);
            scanf("%d", &ord);

            if(ord == 1)
            {
                int parts = selecaoSubstituicaoVagas(outVagas,"P");
                intercalacaoOtimaUnificadaVagas("P",parts, outVagas);

            }
            else if(ord == 2)
            {
                insertion_sort_disco_funcionario(outFuncionarios,base_total_funcionarios);

            }
            else
            {
                insertion_sort_disco_cliente(outClientes,base_total_clientes);
            }

            break;

        case 6:

            printf("\nQual BASE de DADOS deseja realizar a BUSCA SEQUENCIAL\n\n");
            printf("1 ---- VAGAS\n");
            printf("2 ---- FUNCIONÁRIOS\n");
            printf("3 ---- CLIENTES");

            printf("\n\nDIGITE UMA OPCAO: ");
            fflush(stdin);
            scanf("%d", &seq);

            if(seq == 1)
            {
                busca_sequencial_vagas(outVagas);

            }
            else if(seq == 2)
            {
                busca_sequencial(outFuncionarios);
            }
            else
            {
                busca_sequencial_cliente(outClientes);
            }

            break;

        case 7:

            printf("\nQual BASE de DADOS deseja realizar a BUSCA BINÁRIA\n\n");
            printf("1 ---- VAGAS\n");
            printf("2 ---- FUNCIONÁRIOS\n");
            printf("3 ---- CLIENTES");

            printf("\n\nDIGITE UMA OPCAO: ");
            fflush(stdin);
            scanf("%d", &bina);

            if(bina == 1)
            {
                ordenar_por_placa(outVagas, base_total_vagas);
                busca_binaria_vaga(outVagas, base_total_vagas);

            }
            else if(bina == 2)
            {
                busca_binaria(outFuncionarios, base_total_funcionarios);
            }
            else
            {
                busca_binaria_cliente(outClientes, base_total_clientes);
            }

            break;

        case 8:

            estacionar_carro(outVagas);

            break;
        case 9:

            saida_carro(outVagas);

            break;
        case 10:

            system("clear");
            break;

        case 11:

            //SAIR
            system("clear");
            printf("\n\n\n\t >>>>>> MSG: Saindo do MODULO...!!! <<<<<<");
            break;

        default:
            system("clear");
            printf("\n\n\n\t >>>>>> MSG: Digite uma opcao valida!!! <<<<<<");
        } // fim do bloco switch
    }
    while(controle != 11);
}
