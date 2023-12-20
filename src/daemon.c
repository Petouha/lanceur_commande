
#include "../include/file.h"

int main(){

    file_t *file = NULL;
    initialiser_file(&file);

    data_t data;
    //désactiver les signaux

//    sigset_t sigset;
//
//    if(sigfillset(&sigset) == -1){
//        errExit("sigfillset");
//    }
//
//    sigdelset(&sigset,SIGINT);
//
//
//    sigprocmask(SIG_BLOCK,&sigset,NULL);

    //récupérer le pid du proc appelant
    data = defiler(file);

    //lire depuis le tube_in

    int fd = open(create_string(data.pid,STDIN_FILENO),O_RDONLY);
    if (fd == -1)
        errExit("open");

    char buffer[BUFF_TAILLE];
    while (read(fd,buffer,BUFF_TAILLE) > 0);

    close(fd);
    //avoir la liste des commandes
    char **print = get_command_string(buffer);
    //pour avoir chaque commande, on utilise analyse_arg
    char **exec;

    //fork pour éxécuter toutes les commandes sans pipe pour l'instant
    for (int i = 0; i < array_length(print); ++i) {
        switch (fork()) {
            case 0:
                exec = analyse_arg(print[i]);
                execvp(exec[0],exec);
                errExit("execvp");
            default:
                wait(NULL);
        }
    }

    return 0;
}
