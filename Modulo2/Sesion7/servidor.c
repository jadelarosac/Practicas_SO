//servidor.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#define FIFOentrada "FIFOe"
#define FIFOsalida "FIFOs"
#define ArchivoBloqueo "bloq"
#define TAM_BUFF 1024


int num_hijos = 0;
int fd_bloq_file, fd_fifoe, fd_fifos;


static void handlerTERM(int sig){
  // Hacemos wait a los hijos
  for(int i = 0; i < num_hijos; ++i){
  	int pid = wait(0);
  	printf("servidor: Terminando el proceso con pid %d\n", pid);
  }
             
             
  // Cerramos los archivos
  close(fd_bloq_file);
  close(fd_fifoe);
  close(fd_fifos);

  unlink(ArchivoBloqueo);
  unlink(FIFOentrada);
  unlink(FIFOsalida);

  printf("Cerrado ordenadamente\n");
  
  // Terminamos el programa
  exit(EXIT_SUCCESS);
}

static void handlerCHLD(int sig){
  int pid = wait(0);
  printf("ha acabado el proxy con pid %d\n", pid);
  num_hijos--;
}


void gestionarPeticion(int fd_fifoe, int fd_fifos){
  int pid_cliente, pid_proxy;
  int fd_fifo_proxy;
  char buff[TAM_BUFF];
 
  // Atiende la petición de un cliente
	if(read(fd_fifoe, &pid_cliente, sizeof(int)) < 0){
  	perror("servidor: error al leer la peticion de un cliente\n");
    exit(EXIT_FAILURE);
  }
  
  // Creamos un proceso hijo
  if ((pid_proxy = fork())< 0 ){
    perror("servidor: error en el fork\n");
    exit(EXIT_FAILURE);
  }
      
  // Si es el hijo ejecutamos el proxy
  if( pid_proxy == 0 ){
    execlp("./proxy.out","proxy.out",NULL);
  }
  
  num_hijos++;

  sprintf(buff, "fifo.%d", pid_proxy);
  
  while(access(buff, F_OK) != 0){}
  
  // Respondemos con el pid del proxy
  if( write(fd_fifos, &pid_proxy, sizeof(int)) <0){
    perror("servidor: error al responder el cliente\n");
    exit(EXIT_FAILURE);
  }
}


int main(int argc, char **argv){
  int numPidCliente;
  struct sigaction sig_TERM_nact;
  struct sigaction sig_CHLD_nact;


  umask(0);
  
  // Creamos los FIFO
  if( mkfifo(FIFOentrada, S_IRWXU) < 0 ){
    perror("servidor: error al crear FIFOentrada\n");
    exit(EXIT_FAILURE);
  }
  if( mkfifo(FIFOsalida, S_IRWXU)< 0 ){
    perror("servidor: error al crear FIFOsalida\n");
    exit(EXIT_FAILURE);
  }
	
  // Abrimos el archivo de bloqueo
  if( (fd_bloq_file = open(ArchivoBloqueo, O_CREAT | O_TRUNC, S_IRWXU  )) < 0){
    perror("servidor: error al crear el archivo de bloqueo\n");
    exit(EXIT_FAILURE);
	}
  
  // Abrimos los fifo
  if( (fd_fifoe = open(FIFOentrada, O_CREAT | O_TRUNC | O_RDWR )) < 0){
    perror("servidor: error al abrir el FIFO de entrada\n");
    exit(EXIT_FAILURE);
  }
  if( (fd_fifos = open(FIFOsalida, O_CREAT | O_TRUNC | O_RDWR )) < 0){
  	perror("servidor: error al abrir el FIFO de salida\n");
    exit(EXIT_FAILURE);
  }
  
  // Cambiamos la acción por defecto de sigterm
  sig_TERM_nact.sa_handler= handlerTERM;
  sigemptyset (&sig_TERM_nact.sa_mask);
  sig_TERM_nact.sa_flags = SA_RESTART;


  if( sigaction(SIGTERM,&sig_TERM_nact,NULL) <0) 
  {
      perror("servidor: Error al intentar establecer el manejador de señal\n");
      exit(EXIT_FAILURE);
  }
  
  sig_CHLD_nact.sa_handler= handlerCHLD;
  sigemptyset (&sig_CHLD_nact.sa_mask);
  sig_CHLD_nact.sa_flags = SA_RESTART;


  if( sigaction(SIGCHLD,&sig_CHLD_nact,NULL) <0) 
  {
      perror("servidor: Error al intentar establecer el manejador de señal\n");
      exit(EXIT_FAILURE);
  }
  
  
  while(1){
    gestionarPeticion(fd_fifoe, fd_fifos);
  }
}
