#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

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
    
    exit(recorrer(argv[1], 0));
}


int recorrer(char *arg, int tab)
{   
    int returnval = 0;
    struct dirent *linea;
    DIR *dir;
    char *buff;
    char *perm_ant;
    
    
    if( (dir = opendir(arg))==0)
    {
        perror("\nError al abrir DIR");
        returnval = EXIT_FAILURE;
    }
    
    while( (linea = readdir(dir))!=0)
    {
        struct stat atributos;
        buff = (char *) malloc(256*sizeof(char));
            
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
            printf("%s\n",linea->d_name);
            if((atributos.st_mode)&__S_IFDIR)
            {
                strcat(buff, "/");
                
                for(int i = 0; i < tab; ++i)
                    printf("\t");
                printf("Que contiene:\n");
                recorrer(buff, tab+1);
            }
                
            
        }
        
        free(buff);
    }
    
    closedir(dir);
    
    return EXIT_SUCCESS|returnval;
}
