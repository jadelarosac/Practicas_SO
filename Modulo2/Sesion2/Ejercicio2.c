#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>

int main(int argc, char* argv[])
{
	if ( argc != 3 )
	{
		perror("Error en el número de argumentos. Uso:\nprograma : <path_directorio> <permisos>");
		exit(EXIT_FAILURE);
	}

	DIR *directorio;
	char *endptr;
	int permisos = strtol(argv[2], &endptr, 8);
	struct dirent *linea;
	char *archivo;

	if ( strlen(endptr) != 0 )
	{
		perror("Error al establecer permisos");
		exit(EXIT_FAILURE);
	}

	if ( ( directorio = opendir(argv[1])) == NULL )
	{
		perror("Error al abrir DIR");
		exit(EXIT_FAILURE);
	}

	while ( ( linea = readdir(directorio) ) != 0 )
	{
		if (( strcmp(linea->d_name, ".") ) && ( strcmp(linea->d_name, "..") ))
		{
			archivo = (char*) malloc((strlen(argv[1])+strlen(linea->d_name))*sizeof(char));
			strncpy(archivo, argv[1], strlen(argv[1]) );
			strcat(archivo, linea->d_name); 

	    chmod( archivo, permisos );
		}
	}

	closedir(directorio);

	return EXIT_SUCCESS;
}
