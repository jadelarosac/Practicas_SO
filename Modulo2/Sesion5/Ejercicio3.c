#include <stdio.h>
#include <stdio.h>
#include <signal.h>

int main()

{
    sigset_t new_mask;

    /* inicializar la nueva mascara de señales */
    sigfillset(&new_mask);

    sigdelset(&new_mask, SIGUSR1);

    /*esperar a SIGUSR1 */
    sigsuspend(&new_mask);
    
    printf("HE ACABADO\n");

}
