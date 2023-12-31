
#include <file.h>


void * run(void *arg);
int main(){



    file_t *file = NULL;
    initialiser_file(&file);

    //désactiver les signaux

    sigset_t sigset;

    if(sigfillset(&sigset) == -1){
        errExit("sigfillset");
    }
    sigprocmask(SIG_BLOCK,&sigset,NULL);


    while (1) {
        data_t *data = malloc(sizeof(data_t));  // Allocate a new data instance
        if (data == NULL)
            errExit("malloc");
        printf("avant défiler\n");
        *data = defiler(file);
        pthread_t th;

        printf("thread\n");

        dup2(STDOUT_FILENO,STDOUT_FILENO);

        pthread_attr_t attr;
        if ((pthread_attr_init(&attr)) != 0)
            errExit("pthread_attr_init");

        if ((pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) != 0)
            errExit("pthread_attr_setdetachstate");
        if (pthread_create(&th, &attr, run, data) != 0)
            errExit("pthread_create");

        pthread_attr_destroy(&attr);


    }
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
        errExit("open stdin");

    char *buffer = malloc(sizeof(char)*BUFF_TAILLE);
    //lire depuis le tube_in
    while (read(fd,buffer,BUFF_TAILLE) > 0);

    close(fd);

    //ouvrir le tube_out
    fd = open(create_string(data.pid,STDOUT_FILENO),O_WRONLY);
    if (fd == -1)
        errExit("open stdout");

    dup2(fd,STDOUT_FILENO);
    close(fd);



    //avoir la liste des commandes
    char **print = get_command_string(buffer);
    //pour avoir chaque commande, on utilise analyse_arg dans la boucle
    char **exec;
    //les tubes
    int tubes[array_length(print)][2];
    int i;
    int pid;
    //fork pour éxécuter toutes les commandes
    for (i =0; i < array_length(print); ++i) {
        if(pipe(tubes[i]) == -1)
            errExit("pipe");
        switch (pid = fork()) {
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
                waitpid(pid,NULL,0);

        }
    }
    //close(fd);
//    free(arg);
//    free_char_array(print);
//    for (i = 0; i < array_length(print); ++i) {
//        close(tubes[i][0]);
//        close(tubes[i][1]);
//    }

    close(STDOUT_FILENO);
    free(buffer);
    pthread_exit(NULL);
}

void free_char_array(char **arr) {
    if (arr == NULL)
        return;

    for (int i = 0; arr[i] != NULL; ++i) {
        free(arr[i]);
    }

    free(arr);
}