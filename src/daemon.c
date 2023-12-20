
#include "../include/file.h"

int main(){

    file_t *file;
    initialiser_file(&file);

    sigset_t sigset;

    if(sigfillset(&sigset) == -1){
        errExit("sigfillset");
    }

    sigdelset(&sigset,SIGINT);
    data_t data;

    sigprocmask(SIG_BLOCK,&sigset,NULL);
    while(1){
        data = defiler(file);
        printf("data : %d",data.pid);
    }

    return 0;
}

