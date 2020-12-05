#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

#define NUM_SIG 31

int contador[NUM_SIG];

static void sig_USR_hdlr(int sigNum)
{
    ++contador[sigNum];
    printf("\nRecibida %d veces la señal %d\n\n", contador[sigNum],sigNum);

}

int main(int argc, char *argv[])
{
    
    struct sigaction sig_USR_nact;
    
    for(int i = 0; i < NUM_SIG; ++i)
        contador[i] = 0;

    if(setvbuf(stdout,NULL,_IONBF,0)) 
    {
        perror("\nError en setvbuf");
    }

    sig_USR_nact.sa_handler= sig_USR_hdlr;


    sigemptyset (&sig_USR_nact.sa_mask);
    sig_USR_nact.sa_flags = SA_RESTART;

    for(int i = 1; i < NUM_SIG; ++i)
    {
        if(i != SIGKILL && i != SIGSTOP)
        {
            if( sigaction(i,&sig_USR_nact,NULL) <0) 
            {
                perror("\nError al intentar establecer el manejador de señal");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    for(;;)
    	sleep(60);
}
