//
// Created by petouha on 20/12/23.
//

#include "../include/file.h"



int main(int argc, char **argv){

    if (argc != 2){
        fprintf(stderr,"Veuillez entrer une string avec des \" \" \n");
        exit(EXIT_FAILURE);
    }

    //execlp("ps","ps aux","|","wc -l",NULL);

    char **print = get_command_string(argv[1]);
    char **exec;

    for (int i = 0; i < array_length(print); ++i) {
        printf("%s\n",print[i]);
    }

    int tubes[array_length(print)][2];
    int i;
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

                exec = analyse_arg(print[i]);
                execvp(exec[0],exec);
                errExit("execvp");
            default:
                if (close(tubes[i][1]) == -1)
                    errExit("close");

                wait(NULL);
        }
    }

    exit(5);



}

