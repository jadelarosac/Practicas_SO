//#define _DEFAULT_SOURCE
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TAM_buf 80

void UsoIncorrecto()
{
    perror("La función debe llamarse así: \"./esclavo.c <extremo inferior del intervalo> <extremo superior>\"");
    exit(EXIT_FAILURE);

}

int EsPrimo(int n)
{
    int resultado = 1, i = 2;
    
    
    while(resultado != 0 && i <= sqrt(n))
    {
        resultado = n % i;
        ++i;
    }
    
    return resultado;
}


int main(int argc, char *argv[])
{
    if(argc != 3)
        UsoIncorrecto();
    
    
    int a = strtol(argv[1],0,10);
    int b = strtol(argv[2],0,10);
    
    for(int n = a; n <= b; ++n)
    {        
        if(EsPrimo(n))
        {
            printf("%d\n",n);
        }
    }
    
    
    exit(EXIT_SUCCESS);
}
