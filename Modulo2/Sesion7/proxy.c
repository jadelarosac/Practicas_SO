//proxy.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#define TAM_BUFF 1024
#define ArchivoBloqueo "bloq"

int main(int argc, char **argv){
  char buff[TAM_BUFF];
  char mensaje[TAM_BUFF + 1];
  int fd_fifo;
  int fd_bloq;
  int num_leidos;
  FILE *tmp;
  char *c;
  struct flock cerrojo1;
  struct flock cerrojo2;
  
  umask(0);
  
  sprintf(buff, "fifo.%d", getpid());
  
  // Creamos el FIFO
  if( mkfifo(buff, S_IRWXU) < 0 ){
    perror("proxy: error al crear FIFOproxy\n");
    exit(EXIT_FAILURE);
  }
  
  // Abrimos el FIFO
  if( (fd_fifo = open(buff, O_RDONLY ))<0){
    perror("proxy: error al abrir FIFOproxy\n");
    exit(EXIT_FAILURE);
  }
  
  if( (fd_bloq = open(ArchivoBloqueo, O_CREAT | O_WRONLY)) < 0){
    perror("proxy: error al abrir bloq\n");
  	exit(EXIT_FAILURE);
  }
  
  // Redireccionamos la entrada estándar
  dup2(fd_fifo, STDIN_FILENO);
  close(fd_fifo);
  
  // Abrimos el archivo temporal
  if( (tmp = tmpfile()) == NULL){
    perror("proxy: error al abrir el archivo temporal\n");
    exit(EXIT_FAILURE);
  }
 
   while((num_leidos = read(STDIN_FILENO, &mensaje, TAM_BUFF)) != 0){
    mensaje[num_leidos + 1] = '\0';
    fputs(mensaje, tmp);
  }


  // Configuramos cerrojos
  cerrojo1.l_type = F_WRLCK;
  cerrojo1.l_whence = SEEK_SET;
  cerrojo1.l_start = 0;
  cerrojo1.l_len = 0;
  
  cerrojo2.l_type = F_UNLCK;
  cerrojo2.l_whence = SEEK_SET;
  cerrojo2.l_start = 0;
  cerrojo2.l_len = 0;
  
  // Esperamos a que no haya cerrojos para escribir y ponemos un cerrojo
  while(fcntl(fd_bloq, F_SETLKW, &cerrojo1) == -1){
    perror("proxy: Error en el bloqueo\n");
  }
  
  // Leemos y escribimos por pantalla
  do {
    c = fgets(mensaje, TAM_BUFF, tmp);
    printf("%s", mensaje);
  }  
  while(c != NULL);
  printf("\n");
  
  // Desbloqueamos el archivo de bloqueo
  if(fcntl(fd_bloq, F_SETLKW, &cerrojo2) == -1){
    perror("proxy: Error en el desbloqueo\n");
  }
  
  fclose(tmp);

  unlink(buff);
}
