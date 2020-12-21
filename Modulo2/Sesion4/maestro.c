#include<sys/types.h>
#include<sys/wait.h>
#include<sys/signal.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

const unsigned TAM_buf = 80;


void UsoIncorrecto()
{
    perror("La función debe llamarse así: \"./maestro.c <extremo inferior del intervalo> <extremo superior>\"");
    exit(EXIT_FAILURE);

}



int main(int argc, char *argv[])
{
    if(argc != 3)
        UsoIncorrecto();
    
    
    int num_hijos = 2;
    int fd[num_hijos][2];
    int alrdy_rd;
    char inf[strlen(argv[1])+1], sup[ strlen(argv[2]) +1];
    pid_t PID[num_hijos];

    
    const int A = strtol(argv[1],0,10) - 1;
    const int B = strtol(argv[2],0,10);
    double c = (B-A)/num_hijos;
    int a, b;
    
    printf("Los números primos entre %d y %d son:\n", A+1,B);
    


    for(int k = 0; k < num_hijos; ++k)
    {
        pipe(fd[k]); // Llamada al sistema para crear un pipe

        a = A + k*c;
        b = a + c;
        
        if( k == num_hijos -1)
            b = B;


        if ( (PID[k] = fork())<0)
        {
            perror("\Error en fork");
            exit(EXIT_FAILURE);
        }

        if (PID[k] == 0) 
        {
            close(fd[k][0]);

            if(dup2(fd[k][1],STDOUT_FILENO)==-1)
            {
                perror("Error en el dup2");
                exit(EXIT_FAILURE);
            }

            sprintf(inf, "%d", a + 1);
            sprintf(sup, "%d", b);

           
            if(execlp("./estudiante.out", "estudiante.out", inf, sup, NULL)==-1)
            {
                perror("Error en el execlp");
                exit(EXIT_FAILURE);
            }
        }
        
        close(fd[k][1]);
    }
        
    for(int k = 0; k < num_hijos; ++k)
        wait(0);

    for(int k = 0; k < num_hijos; ++k)
    {

        alrdy_rd = TAM_buf - 1;

        while(alrdy_rd == TAM_buf - 1)
        {
            
            char buff[TAM_buf];

            for( int i = 0; i < TAM_buf; ++i)
                sprintf((buff+i), "%c", '\0');

            alrdy_rd = read(fd[k][0], buff, TAM_buf-1);
            
            if(alrdy_rd == -1)
            {
                perror("Error en el read");
                exit(EXIT_FAILURE);
            }

            printf("%s", buff);
        }

    }

    
    return EXIT_SUCCESS;
}
