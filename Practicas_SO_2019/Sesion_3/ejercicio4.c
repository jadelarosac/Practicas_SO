#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    
    
    int *pid, i, aux;
    int numeroHijos =5;
    int  pid_org = getpid();
    char buff[256];
    
    
    pid = malloc(numeroHijos*sizeof(int));
    
    
    //Convirtamos la salida en unbuffered
    setvbuf(stdout, buff, _IONBF, strlen(buff)); 
    //Ahora printf(buff) equivale a write(STDOUT_FILENO, buff, strlen(buff));
    
    for(i = 1; i <= 5 && pid_org == getpid(); ++i)
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
            
            //sprintf(buff, "Soy el hijo %d\n", getpid());
            
            //write(STDOUT_FILENO, buff, strlen(buff))
        }
        
    }
    
    
    for(i = 1; i <= numeroHijos && pid_org == getpid(); ++i)
    {
        
        if (pid[i] != 0)//Es el padre
        {
            aux = wait(0);
            
            if(aux ==-1)
            {
                perror("Error en el waitpid");
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("Acaba de finalizar mi hijo %d\nMe quedan %d hijos vivos\n", aux, numeroHijos-i);
                
                //sprintf(buff, "Acaba de finalizar mi hijo %d\nMe quedan %d hijos vivos\n", aux, numeroHijos-i);
                
                
                //write(STDOUT_FILENO, buff, strlen(buff));
            }
        }
    }
    
    
    free(pid);
    exit(EXIT_SUCCESS);
    
}
