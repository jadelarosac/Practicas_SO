#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    
    
    int *pid, i, aux, hijos_vivos=0;
    int numeroHijos =5;
    int  pid_org = getpid();
    char buff[256];
    
    
    pid = malloc((1+numeroHijos)*sizeof(int));
    
    
    //Convirtamos la salida en unbuffered
    setvbuf(stdout, buff, _IONBF, strlen(buff)); 
    //Ahora printf(buff) equivale a write(STDOUT_FILENO, buff, strlen(buff));
    
    for(i = 1; i <= numeroHijos && pid_org == getpid(); ++i)
    {
        pid[i] = 0;
        
        if((pid[i] = fork())<0)
        {
            perror("Error en el fork");
            exit(EXIT_FAILURE);
        }
        else if (pid[i] == 0) //Es el hijo
        {
            printf("Soy el hijo %d\n", getpid());
        }
        else
        {
            hijos_vivos++;   
        }
        
    }
    
    
    for(i = 1; i <= numeroHijos && pid_org == getpid(); i+=2)
    {
        aux = waitpid(pid[i], 0, 0);
        
        if(aux ==-1)
        {
            perror("Error en el waitpid");
            exit(EXIT_FAILURE);
        }
        else
        {
            if ((pid[i] != 0))//Es el padre
            {
                --hijos_vivos;
                printf("Acaba de finalizar mi hijo %d\nMe quedan %d hijos vivos\n", aux, hijos_vivos);
                
            }
        }
    }
    
    for(i = 2; i <= numeroHijos && pid_org == getpid(); i+=2)
    {
        aux = waitpid(pid[i], 0, 0);
        
        if(aux ==-1)
        {
            perror("Error en el waitpid");
            exit(EXIT_FAILURE);
        }
        else
        {
            if ((pid[i] != 0))//Es el padre
            {
                --hijos_vivos;
                printf("Acaba de finalizar mi hijo %d\nMe quedan %d hijos vivos\n", aux, hijos_vivos);
                
            }
        }
    }
    
    
    free(pid);
    exit(EXIT_SUCCESS);
    
}
