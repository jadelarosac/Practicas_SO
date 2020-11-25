#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



void UsoIncorrecto()
{
    perror("La función debe llamarse así: \"./ejercicio2 <programa 1> \"|\" <programa 2>\"");
    exit(EXIT_FAILURE);

}



int main(int argc, char *argv[])
{
    int fd[2], pid;
    
    if(argc < 4 || strcmp(argv[2], "|" ))
        UsoIncorrecto();
    
    
    if( pipe(fd) < 0 )
    {
        perror("Error en pipe");
        exit(EXIT_FAILURE);
    }
    
    if((pid=fork())==-1)
    {
        perror("Error al crear hijo");
        printf("No se pudo crear un hijo");
        exit(EXIT_FAILURE);
    }
    
    
    
    if(pid == 0) //Es el hijo
    {
        close(fd[0]); //No queremos leer, solo escribir
        
        close(STDOUT_FILENO);
        if( (fcntl(fd[1], F_DUPFD, STDOUT_FILENO)) < 0)
        {
            perror("Fallo en el fcntl (output)");
            exit(EXIT_FAILURE);
        }
                
        execlp(argv[1], argv[1], NULL);
    }
    else //Es el padre
    {
        if(wait(0) < 0)
        {
            perror("Error en el write");
            exit(EXIT_FAILURE);
        }
        
        close(fd[1]); //No queremos escribir, solo leer
                
        close(STDIN_FILENO);
        if( (fcntl(fd[0], F_DUPFD, STDIN_FILENO)) < 0)
        {
            perror("Fallo en el fcntl (input)");
            exit(EXIT_FAILURE);
        }
        
        execlp(argv[3], argv[3], NULL);
    }
    
    exit(EXIT_SUCCESS);
}
