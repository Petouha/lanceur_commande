//
// Created by petouha on 20/12/23.
//

#include "../include/file.h"

int array_length(char **arr) {
    int length = 0;

    while (arr[length] != NULL) {
        length++;
    }

    return length;
}


int main(int argc, char **argv){

    if (argc < 2){
        fprintf(stderr,"Pas assez d'arguments\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < argc; ++i) {
        printf("%s\n",argv[i]);
    }

    char **print = get_command_string(argv);

//    int tubes[array_length(print)][2];

    for (int j = 0; array_length(print); ++j) {

    }

//    char *fifo_in = create_string(getpid(),STDIN_FILENO);
//    char *fifo_out = create_string(getpid(),STDOUT_FILENO);
//    char *fifo_err = create_string(getpid(),STDERR_FILENO);
//
//
//    mkfifo(fifo_in,0642);
//    mkfifo(fifo_out,0642);
//    mkfifo(fifo_err,0642);
//
//    int fd;
//
//    char buffer[BUFF_TAILLE];
//
//    switch (fork()) {
//        case -1:
//            errExit("fork");
//        case 0:
//            if( (fd = open(fifo_in,O_WRONLY)) == -1){
//                errExit("open");
//            }
//            //ssize_t written;
//            for (int i = 1; i < argc; ++i){
//                snprintf(buffer,BUFF_TAILLE,"%s ",argv[i]);
//                if (write(fd,buffer,strlen(buffer)) == -1){
//                    errExit("write");
//                }
//            }
//            close(fd);
//            exit(EXIT_FAILURE);
//        default:
//
//            if( (fd = open(fifo_in,O_RDONLY)) == -1){
//                errExit("open");
//            }
//            wait(NULL);
//            ssize_t n;
//            while ((n = read(fd,buffer,BUFF_TAILLE)) > 0){
//                if ( (write(STDOUT_FILENO, buffer,(size_t ) n)) !=  n){
//                    errExit("write");
//                }
//            }
//            close(fd);
//    }
//
//

    exit(5);



//    int val;
//    for (int i = 0; i <3; ++i) {
//        sleep(2);
//        sem_getvalue(&file->vide,&val);
//        printf("%d\n",val);
//        defiler(file);
//    }



}

