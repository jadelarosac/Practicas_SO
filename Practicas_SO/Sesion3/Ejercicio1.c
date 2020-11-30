#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



void Es_par(int input)
{
    if(input%2)
        printf("El número %d es impar.\n", input);
    else
        printf("El número %d es par.\n", input);
    
    return;
}
void Es_mul_4(int input)
{
    if(input%4)
        printf("El número %d no es divisible por 4.\n", input);
    else
        printf("El número %d es divisible por 4.\n", input);
    
    return;
}


void UsoIncorrecto()
{
    perror("La función debe llamarse así: \"./ejercicio4 <número>\"");
    exit(EXIT_FAILURE);

}

int main(int argc, char** argv)
{
    if(argc != 2)
        UsoIncorrecto();
    
    int input, pid;
    
    
    input = strtoul(argv[1], 0, 10);
    
    
    
    if((pid=fork())==-1)
    {
        perror("Error al crear hijo");
        printf("No se pudo crear un hijo");
        exit(EXIT_FAILURE);
    }
    else if(pid)
        Es_par(input);
    else
        Es_mul_4(input);
            
    return 0;
}
