#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char * sym = argv[argc-2];
    char * filename = argv[argc-1];
    int fd, i;

    char * argumentos[argc];

    if (argc < 2 ){
        printf("Error, número de argumentos incorrectos");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < argc - 3; i++) {
            argumentos[i] = (char *) malloc(strlen(argv[i+1])+1);
            strcpy(argumentos[i], argv[i+1]);
    }

    argumentos[i] = NULL;


    if( strcmp(sym, ">") == 0 )
    {
        if( (fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC , S_IRWXU ) ) < 0 )
        {
            perror("Error en el open\n");
            exit(EXIT_FAILURE);
        }

        close(STDOUT_FILENO);

        if ( fcntl(fd, F_DUPFD, STDOUT_FILENO) == -1 )
        {
            perror("Fallo en el fcntl\n");
            exit(EXIT_FAILURE);
        }

        if(execvp(argv[1], argumentos) < 0){
            perror("Error en execvp.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if ( strcmp(sym, "<") == 0 )
    {
        if( (fd = open(filename, O_RDONLY ) ) < 0 )
        {
            perror("Error en el open\n");
            exit(EXIT_FAILURE);
        }

        close(STDIN_FILENO);

        if ( fcntl(fd, F_DUPFD, STDIN_FILENO) == -1 )
        {
            perror("Fallo en el fcntl\n");
            exit(EXIT_FAILURE);
        }
        if(execvp(argv[1], argumentos) < 0){
            perror("Error en execvp.\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        perror("Error. debe usarse: ./programa <orden bash> \">\"|\"<\" <archivo>\n");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
