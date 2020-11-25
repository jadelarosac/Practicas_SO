#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



void UsoIncorrecto()
{
    perror("La función debe llamarse así: \"./esclavo.c <extremo inferior del intervalo> <extremo superior>\"");
    exit(EXIT_FAILURE);

}



int main(int argc, char *argv[])
{
    int fd;
    
    if(argc < 4 || strcmp(argv[2], "<" ) &&  strcmp(argv[2], ">" ))
        UsoIncorrecto();
    
    if((fd = open(argv[3], O_CREAT|O_RDWR, S_IRUSR|S_IWUSR)) < 0)
    {
        perror("Error en el open");
        exit(EXIT_FAILURE);
    }
    
    if(strcmp(argv[2], ">" ) == 0)
    {
        close(STDOUT_FILENO);
        if( (fcntl(fd, F_DUPFD, STDOUT_FILENO)) < 0)
        {
            perror("Fallo en el fcntl");
            exit(EXIT_FAILURE);
        }
    }
    else if(strcmp(argv[2], "<" ) == 0)
    {
        close(STDIN_FILENO);
        if( (fcntl(fd, F_DUPFD, STDIN_FILENO)) < 0)
        {
            perror("Fallo en el fcntl");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        perror("No es \"<\" o \">\"");
        exit(EXIT_FAILURE);
    }
        
    execlp(argv[1], argv[1], NULL);
    
    exit(EXIT_SUCCESS);
}
