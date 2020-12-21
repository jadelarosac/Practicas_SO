#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  	/* Primitive system data types for abstraction\
			   of implementation-dependent data types.
			   POSIX Standard: 2.6 Primitive System Data Types
			   <sys/types.h>
			*/
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

#include<string.h>


int main(int argc, char *argv[])
{
    int fdin, fdout, fdtmp, selec, i;
    char bf[80];
    char* aux;
    char* primera_linea;
    
    selec = 80;
    i = 0;
    
    if(argc == 2)
    {
		if( (fdin=open(argv[1],O_RDONLY,S_IRUSR|S_IWUSR))<0) {
			printf("Error %d en open\n",errno);
			perror("Error en el primer open");
			exit(EXIT_FAILURE);
		}
    }
    else if(argc == 1 )
    {
    	fdin = STDIN_FILENO;
    }
    else
    {
        perror("\nError. Introduce solo el nombre del archivo de datos");
        exit(EXIT_FAILURE);
    }


	if( (fdout=open("salida.txt",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))<0) {
		printf("Error %d en open\n",errno);
		perror("Error en el segundo open");
		exit(EXIT_FAILURE);
	}

	if( (fdtmp=open("./",__O_TMPFILE|O_RDWR,S_IRUSR|S_IWUSR))<0) {
		printf("Error %d en open\n",errno);
		perror("Error en el tercer open");
		exit(EXIT_FAILURE);
	}

	while(selec == 80)
	{
		aux = (char*)malloc(strlen("\nBloque 0000\n")*sizeof(char));
		i++;
		sprintf(aux,"\nBloque %d\n", i);
		
		if(write(fdtmp,aux,strlen(aux))!=strlen(aux))
		{
		    perror("\nError en el primer write");
		    exit(EXIT_FAILURE);
		}
		    
		
		selec = read(fdin, bf, 80);

		if(write(fdtmp,bf,selec) != selec) {
		    perror("\nError en el segundo write");
		    exit(EXIT_FAILURE);
		}
		
		free(aux);
	}

	if(lseek(fdtmp, 0, SEEK_SET)!=0)
	{
		perror("\nEror en el lseek");
		exit(EXIT_FAILURE);
	}

	primera_linea = (char*)malloc(strlen("El número de bloques es ****")*sizeof(char));
	sprintf(primera_linea,"El número de bloques es %d", i);


	if(write(fdout,primera_linea,strlen(primera_linea))!=strlen(primera_linea))
	{
		perror("\nError en el tercer write");
		exit(EXIT_FAILURE);
	}

	selec = read(fdtmp, bf, 80);
	while(selec != 0)
	{
		if(write(fdout,bf,selec) != selec) {
		    perror("\nError en el cuarto write");
		    exit(EXIT_FAILURE);
		}
		
		selec = read(fdtmp, bf, 80);
		
	}
	

	free(primera_linea);
	close(fdout);
	close(fdin);
	close(fdtmp);


    
    return EXIT_SUCCESS;
}

