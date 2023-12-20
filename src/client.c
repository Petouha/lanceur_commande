
#include <file.h>

int main(int argc, char **argv){

    if (argc < 2){
        fprintf(stderr,"Pas assez d'arguments\n");
        exit(EXIT_FAILURE);
    }

    file_t *file = NULL;
    recuperer_file_client(&file);

    char *fifo_in = create_string(getpid(),STDIN_FILENO);
    char *fifo_out = create_string(getpid(),STDOUT_FILENO);
    char *fifo_err = create_string(getpid(),STDERR_FILENO);

    data_t data = {getpid()};
    enfiler(file,data);


    mkfifo(fifo_in,0642);
    mkfifo(fifo_out,0642);
    mkfifo(fifo_err,0642);


    int fd;
    if( (fd = open(fifo_in,O_WRONLY)) == -1){
        errExit("open");
    }

    ssize_t written;
    int i=1;
    char buffer[BUFF_TAILLE];
    while(argv[i]!=NULL){
        if ( (written = write(fd,argv[i++],BUFF_TAILLE) ) != sizeof(argv[i]) ){

        }
    }


    exit(5);



//    int val;
//    for (int i = 0; i <3; ++i) {
//        sleep(2);
//        sem_getvalue(&file->vide,&val);
//        printf("%d\n",val);
//        defiler(file);
//    }



}
