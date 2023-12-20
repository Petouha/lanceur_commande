//
// Created by petouha on 20/12/23.
//

#include "../include/file.h"



int main(int argc, char **argv){

    if (argc != 2){
        fprintf(stderr,"Veuillez entrer une string avec des \" \" \n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < argc; ++i) {
        printf("%s\n",argv[i]);
    }

    //char **print = get_command_string(argv[1]);

//    int tubes[array_length(print)][2];

    mkfifo("test",0642);
    char *buffer[BUFF_TAILLE];
    switch(fork()){
        case 0:
            printf("\n");
            int fg = open("test",O_RDONLY);
            if(fg == -1)
                 errExit("open");
            ssize_t n=0;
            while( ( n = read(fg,buffer,BUFF_TAILLE)) > 0){
                if(write(STDOUT_FILENO,buffer, (size_t ) n) != n)
                    errExit("write");
            }
            close(fg);
            exit(EXIT_SUCCESS);

        default:
            printf("\n");
            int fd = open("test",O_WRONLY);
            if(fd == -1)
                errExit("open");
            if(write(fd,argv[1],strlen(argv[1])) == -1)
                errExit("write");
            close(fd);
            wait(NULL);
    }


    exit(5);



}

