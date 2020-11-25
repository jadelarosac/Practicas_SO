//tarea5.c
//Trabajo con llamadas al sistema del Subsistema de Procesos conforme a POSIX 2.10

#include<sys/types.h>	
#include<sys/wait.h>	
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>


int main(int argc, char *argv[])
{

    pid_t pid;
    int estado, buff_size;
    char **buff;
    char *path;
    
    buff_size = argc - 1;
    path = argv[0];
    
    buff = malloc((buff_size+1)*256);
    
    if(strcmp(argv[argc-1],"bg")==0)
        --buff_size;
    
    for(int i = 0; i < buff_size; i++)
        buff[i] = argv[i+1];
    
    buff[buff_size] = argv[argc];
    
    
    
    if( (pid=fork())<0)
    {
        perror("\nError en el fork");
        exit(EXIT_FAILURE);
    }
    else if(pid==0)
    {  
        //proceso hijo ejecutando el programa
        if( (execv("/usr/bin/ldd", argv) < 0))
        {
            perror("\nError en el execl");
            exit(EXIT_FAILURE);
        }
    }
    
    if(buff_size == argc-1)
        wait(&estado);
    else
        signal(SIGCHLD, SIG_IGN);
    
    
    printf("\nHago cosas\n");
    /*
    <estado> mantiene información codificada a nivel de bit sobre el motivo de finalización del proceso hijo 
    que puede ser el número de señal o 0 si alcanzó su finalización normalmente.
    Mediante la variable estado de wait(), el proceso padre recupera el valor especificado por el proceso hijo como argumento de la llamada exit(), pero desplazado 1 byte porque el sistema incluye en el byte menos significativo 
    el código de la señal que puede estar asociada a la terminación del hijo. Por eso se utiliza estado>>8 
    de forma que obtenemos el valor del argumento del exit() del hijo.
    */

    if(buff_size == argc-1)
        printf("\nMi hijo %d ha finalizado con el estado %d\n", pid, estado >> 8);

    free(buff);
    
    if(buff_size != argc-1)
        wait(&estado);
    exit(EXIT_SUCCESS);

}
