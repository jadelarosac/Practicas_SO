#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>


void recorrer(char *arg, int tab, int *contador, int *tamanno);
    
int main(int argc, char *argv[])
{
   
    int contador = 0;
    int tamanno = 0;
    char *arg;
    if ( argc == 2 )
    {
        arg = (char*) malloc(strlen(argv[1])*sizeof(char)); 
        strncpy(arg, argv[1], strlen(argv[1]));
    }
    else if ( argc == 1 )
    {
        arg = (char*) malloc(3*sizeof(char));
        strncpy(arg, "./", 3*sizeof(char));
    }
    else
    {
        perror("Error en el número de argumentos");
        exit(EXIT_FAILURE);
    }
    
    printf("Los inodos son:\n");
    recorrer(arg, 0, &contador, &tamanno);
    printf("\n\n");
    printf("Existen %d archivos regulares con permiso x para grupo y otros.\n\n", contador);
    printf("El tamaño total ocupado por dichos archivos es %d bytes.\n\n", tamanno);
    free(arg);
    exit(EXIT_SUCCESS);
}


void recorrer(char *arg, int tab, int *contador_ref, int *tamanno_ref)
{   
    struct dirent *linea;
    DIR *dir;
    char* buff;
    
    if( (dir = opendir(arg))==0)
    {
        perror("\nError al abrir DIR");
        exit(EXIT_FAILURE);
    }
    
    while( (linea = readdir(dir))!=0)
    {
        struct stat atributos;
        
        buff = (char*) malloc( (strlen(arg)+strlen(linea->d_name))*sizeof(char) );
        strncpy(buff, arg, strlen(arg));
        strcat(buff,linea->d_name);
        
        if( (strcmp(linea->d_name, ".") != 0) && (strcmp(linea->d_name, "..") != 0)  )
        {
            if( stat(buff, &atributos)<0)
            {
                continue;
            }
            
            for(int i = 0; i < tab; ++i)
                printf("\t");
            
            
            if(S_ISDIR(atributos.st_mode))
            {
                printf("El directorio %s\t%ld ",linea->d_name, linea->d_ino);
                strcat(buff, "/");
                (*contador_ref)++;
                *tamanno_ref += atributos.st_size;

                printf("que contiene:\n");
                recorrer(buff, tab+1, contador_ref, tamanno_ref);
            } 
            else if(S_ISREG(atributos.st_mode)&& (atributos.st_mode&S_IXOTH)==S_IXOTH && (atributos.st_mode&S_IXGRP)==  S_IXGRP  )
            {
                printf("%s\t%ld\n",linea->d_name, linea->d_ino);
                (*contador_ref)++;
                *tamanno_ref += atributos.st_size;
            }
                
            
        }
       free(buff); 
    }
    
    closedir(dir);
}
