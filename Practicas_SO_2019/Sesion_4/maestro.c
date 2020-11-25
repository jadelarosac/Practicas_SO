/*
tarea8.c
Programa ilustrativo del uso de pipes y la redirección de entrada y
salida estándar: "ls | sort", utilizando la llamada dup2.
*/

#include<sys/types.h>
#include<sys/wait.h>
#include<sys/signal.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#define TAM_buf 80


void UsoIncorrecto()
{
    perror("La función debe llamarse así: \"./esclavo.c <extremo inferior del intervalo> <extremo superior>\"");
    exit(EXIT_FAILURE);

}



int main(int argc, char *argv[])
{
    if(argc != 3)
        UsoIncorrecto();
    
    
    int fd[2];
    int arg_wait, salir=0, alrdy_rd;
    char buff[TAM_buf], inf[sizeof(int)+1], sup[sizeof(int)+1];
    pid_t PID = getpid();

    
    int a = strtol(argv[1],0,10);
    int b = strtol(argv[2],0,10);
    int c = (a+b)/2;
    
    printf("Los números primos entre %d y %d son:\n", a,b);
    
    pipe(fd); // Llamada al sistema para crear un pipe

    for(int k = 0; k < 2 && PID != 0; ++k)
    {
        if ( (PID = fork())<0)
        {
            perror("\Error en fork");
            exit(EXIT_FAILURE);
        }

        if (PID == 0) 
        {
            close(fd[0]);

            if(dup2(fd[1],STDOUT_FILENO)==-1)
            {
                perror("Error en el dup2");
                exit(EXIT_FAILURE);
            }
            
            if(k==0)
            {
                sprintf(inf, "%d", a);
                sprintf(sup, "%d", c);
            }
            if(k==1)
            {
                sprintf(inf, "%d", c+1);
                sprintf(sup, "%d", b);
            }
            
            if(execlp("./esclavo", "esclavo", inf, sup, NULL)==-1)
            {
                perror("Error en el execlp");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            if(wait(0)<0)
            {
                perror("Error en el wait");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    if(PID != 0) 
    {
        {
            close(fd[1]);

            alrdy_rd = read(fd[0], buff, TAM_buf-1);
            
            while(alrdy_rd == TAM_buf-1)
            {
                
                if(alrdy_rd == -1)
                {
                    perror("Error en el read");
                    exit(EXIT_FAILURE);
                }
                
                printf("%s", buff);
                
                alrdy_rd = read(fd[0], buff, TAM_buf-1);
                   
            }
            
            for(int k = 0; k < alrdy_rd; ++k)
                printf("%c", buff[k]);
            
        }
    }    
    
    
    return EXIT_SUCCESS;
}



