#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void UsoIncorrecto()
{
    perror("La función debe llamarse así: \"./ejercicio3 <tipo de jerarquía (1 o 2)>\"");
    exit(EXIT_FAILURE);

}

int main(int argc, char** argv)
{
    
    int i, tipo, nprocs = 20, childpid;
    
    
    if(argc != 2 || (strcmp(argv[1],"1") != 0 && strcmp(argv[1],"2") != 0))
        UsoIncorrecto();

    

    /*Jerarquía de procesos tipo 1*/
    for (i=0; i < nprocs && (strcmp(argv[1],"1")==0); i++)
    {             
        if ((childpid = fork()) == -1)
        {
            fprintf(stderr, "Could not create child %d: %s\n",i,strerror(errno));
            exit(EXIT_FAILURE);
        }
    
        if (childpid!=0)
        {
            printf("Yo, %d, soy hijo de %d\n", getpid(), getppid());
            break;
        }
    }

    /*Jerarquía de procesos tipo 2*/
    for (i=0; i < nprocs && (strcmp(argv[1],"2")==0); i++)
    {
        if ((childpid = fork()) == -1)
        {
            fprintf(stderr, "Could not create child %d: %s\n",i,strerror(errno));
            exit(EXIT_FAILURE);
        }         
    
        if (childpid==0)
        {
            printf("Yo, %d, soy hijo de %d\n", getpid(), getppid());
            break;
        }
    }
    
    
    
    if(strcmp(argv[1],"1")==0)
    {
    	wait(0);
    }
    
    

    exit(EXIT_SUCCESS);

}
