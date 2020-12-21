/*
tarea5.c
Trabajo con llamadas al sistema del Subsistema de Procesos conforme a POSIX 2.10
*/
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

void argumentos(char* argfin[], char* argv[], unsigned ini, unsigned fin)
{
    for(int i = ini; i < fin; ++i)
    {
        strcpy(argfin[i-ini], argv[i]);
    }
    argfin[fin-ini] = NULL;
}


int main(int argc, char *argv[]){

    pid_t pid = 0;
    char **new_args;
    int len_max = 0;

    for( int i = 0; i < argc; ++i)
        if ( strlen(argv[i]) > len_max )
                len_max = strlen(argv[i]);

    new_args = (char**) malloc((argc+1)*len_max*sizeof(char)) ;
    for(int i = 0; i < argc; ++i)
        new_args[i] = (char*) malloc(len_max*sizeof(char));

    if( strcmp(argv[argc-1], "bg" ) == 0 )
    {
        argumentos(new_args, argv, 1, argc-1);

        if( (pid=fork())<0) {
            perror("\nError en el fork");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        argumentos(new_args, argv, 1, argc);

    }

    if(pid == 0) // No se ha dividido o es el padre
    {
        if( (execvp( argv[1],(new_args) )<0)) {
                perror("\nError en el execl");
                exit(EXIT_FAILURE);
            }
    }
    else
    {
            exit(EXIT_SUCCESS);
    }


    free(new_args);


}
