#include <stdio.h>
#include <stdlib.h>
#include "funcionario.h"
#include "vaga.h"
#include "interface.h"

int main(int argc, char** argv)
{

    FILE *outVagas;
    FILE *outFuncionarios;
    FILE *outClientes;

    if ((outVagas = fopen("vagas.dat", "w+b")) == NULL)
    {
        printf("Erro ao abrir arquivo de vagas\n");
        exit(1);
    }

    if ((outFuncionarios = fopen("funcionarios.dat", "w+b")) == NULL)
    {
        printf("Erro ao abrir arquivo de funcionários\n");
        exit(1);
    }

     if ((outClientes = fopen("clientes.dat", "w+b")) == NULL)
    {
        printf("Erro ao abrir arquivo de vagas\n");
        exit(1);
    }

    MENU(outVagas, outFuncionarios, outClientes);

    fclose(outVagas);
    fclose(outFuncionarios);
    fclose(outClientes);

    return 0;
}
