#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>


int contador = 0;
int tamanno = 0;

int visitar(const char* path, const struct stat* atr, int flags, struct FTW* ftw)
{
  char * filename;
  filename = (char*) malloc(strlen(path)*sizeof(char));
  strcpy(filename, path);
  filename = basename(filename);
  for( int i = 0; i < ftw->level; ++i)
    printf("\t"); 
  if ( flags == FTW_D )
  {
    printf("El directorio %s\t%ld", filename, atr->st_ino);
    printf(" que contiene:\n");
  }else if ( flags == FTW_F )
  {
    printf("%s\t%ld\n", filename, atr->st_ino);
  }
  
  if((atr->st_mode&S_IXOTH)==S_IXOTH && (atr->st_mode&S_IXGRP)==S_IXGRP)
  {
    contador++;
    tamanno += atr->st_size;
  }
  
  free(filename);
  
  return 0;
}


int main(int argc, char** argv)
{
  printf("Los inodos son:\n");
  if (nftw(argc >= 2 ? argv[1] : ".", visitar, 10, 0) != 0)
  {
    perror("nftw");
    exit(EXIT_FAILURE);
  }
  printf("\n\n");
  printf("Existen %d archivos regulares con permiso x para grupo y otros.\n\n", contador);
  printf("El tamaño total ocupado por dichos archivos es %d bytes.\n\n", tamanno);
  
}
