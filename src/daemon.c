
#include "../include/file.h"


void * run(void *arg);

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

    pthread_t th;

    if(pthread_create(&th,NULL,run,&data) != 0)
        errExit("pthread_create");

    pthread_join(th,NULL);

    return 0;
}

void * run(void *arg){

    if (arg == NULL){
        fprintf(stderr, "Unexpected argument value\n");
        exit(EXIT_FAILURE);
    }

    data_t data = *((data_t *) arg);

    int fd = open(create_string(data.pid,STDIN_FILENO),O_RDONLY);
    if (fd == -1)
        errExit("open");

    char buffer[BUFF_TAILLE];
    //lire depuis le tube_in
    while (read(fd,buffer,BUFF_TAILLE) > 0);

    close(fd);

    //ouvrir le tube_out
    fd = open(create_string(data.pid,STDOUT_FILENO),O_WRONLY);
    if (fd == -1)
        errExit("open");

    dup2(fd,STDOUT_FILENO);

    //avoir la liste des commandes
    char **print = get_command_string(buffer);
    //pour avoir chaque commande, on utilise analyse_arg dans la boucle
    char **exec;
    //les tubes
    int tubes[array_length(print)][2];
    int i;
    //fork pour éxécuter toutes les commandes
    for (i =0; i < array_length(print); ++i) {
        if(pipe(tubes[i]) == -1)
            errExit("pipe");
        switch (fork()) {
            case 0:

                //fermer le descripteur ouvert en lecture
                if (close(tubes[i][0]) == -1)
                    errExit("close");
                //si ce n'est pas le dernier processus :sortie devient sur le tube
                //sinon : garder la sortie du père
                if (i != array_length(print)-1)
                    dup2(tubes[i][1],STDOUT_FILENO);


                //sauf pour le 1er proc, tous les tubes :
                if (i > 0){
                    //entrée devient sur le tube précedent
                    dup2(tubes[i-1][0], STDIN_FILENO);
                }

                //récuperer la commande et l'exec
                exec = analyse_arg(print[i]);
                execvp(exec[0],exec);
                errExit("execvp");
            default:
                if (close(tubes[i][1]) == -1)
                    errExit("close");
                printf("fils %d en exec\n",i);
                wait(NULL);
        }
    }
    pthread_exit(NULL);
}