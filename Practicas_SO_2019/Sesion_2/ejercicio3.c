#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int contador;
long int tamanno;

void UsoIncorrecto()
{
    perror("La función debe llamarse así: \"./ejercicio3 <pathname del directorio>\"");
    exit(EXIT_FAILURE);

}

int recorrer(char *arg, int tab);
    
int main(int argc, char *argv[])
{
    if(argc != 2)
        UsoIncorrecto();
    
    int returnval;
    contador = 0;
    tamanno = 0;
    
    printf("Los inodos son:\n");
    returnval = recorrer(argv[1], 0);
    printf("\n\n");
    printf("Existen %d archivos regulares con permiso x para grupo y otros.\n\n", contador);
    printf("El tamaño total ocupado por dichos archivos es %ld bytes.\n\n", tamanno);
    exit(returnval);
}


int recorrer(char *arg, int tab)
{   
    int returnval = 0;
    struct dirent *linea;
    DIR *dir;
    char buff[256];
    
    
    if( (dir = opendir(arg))==0)
    {
        perror("\nError al abrir DIR");
        returnval = EXIT_FAILURE;
    }
    
    while( (linea = readdir(dir))!=0)
    {
        struct stat atributos;
        //buff = (char *) malloc(256*sizeof(char));
            
        sprintf(buff, "./");
        strcat(buff, arg);
        strcat(buff,linea->d_name);
        
        if(strcmp(linea->d_name,".") && strcmp(linea->d_name,".."))
        {
            if( stat(buff, &atributos)<0)
            {
                printf("\nError al intentar acceder a los atributos");
                perror("\nError en el lstat");
                returnval = EXIT_FAILURE;
            }
            
            for(int i = 0; i < tab; ++i)
                printf("\t");
            
            if(atributos.st_mode&__S_IFREG && atributos.st_mode&S_IXOTH)
            {
                if(!(atributos.st_mode&__S_IFDIR))
                    printf("%s\t%ld\n",linea->d_name, linea->d_ino);
                contador++;
                tamanno += atributos.st_size;
            }
            
            if(atributos.st_mode&__S_IFDIR)
            {
                printf("El directorio %s\t%ld\n",linea->d_name, linea->d_ino);
                strcat(buff, "/");
                
                for(int i = 0; i < tab; ++i)
                    printf("\t");
                printf("Que contiene:\n");
                recorrer(buff, tab+1);
            }
                
            
        }
        
        //free(buff);
    }
    
    closedir(dir);
    
    return EXIT_SUCCESS|returnval;
}
