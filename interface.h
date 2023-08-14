# ifndef INTERFACE_H_INCLUDED
# define INTERFACE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "funcionario.h"
#include "cliente.h"
#include "carro.h"
#include "vaga.h"

void MSG_MENU(); // MENSAGEM PARA EXIBIR O MENU PRINCIPAL

void MENU(FILE *outVagas, FILE *outFuncionarios, FILE *outClientes); // MÓDULO DE MENU

# endif // INTERFACE_H_INCLUDED

//FIM
