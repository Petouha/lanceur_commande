
#include <file.h>

int main(int argc, char **argv){

    if (argc != 2){
        fprintf(stderr,"Veuillez entrer une string avec des \" \" \n");
        exit(EXIT_FAILURE);
    }

    file_t *file = NULL;
    recuperer_file_client(&file);
    char buffer[BUFF_TAILLE];
    //création des tubes

    char *fifo_in = create_string(getpid(),STDIN_FILENO);
    char *fifo_out = create_string(getpid(),STDOUT_FILENO);
    char *fifo_err = create_string(getpid(),STDERR_FILENO);




    mkfifo(fifo_in,0642);
    mkfifo(fifo_out,0642);
    mkfifo(fifo_err,0642);

    data_t data = {getpid()};
    enfiler(file,data);

    //écrire la commande sur le tube_in
    int fd;
    if( (fd = open(fifo_in,O_WRONLY)) == -1){
        errExit("open");
    }

    if(write(fd,argv[1],strlen(argv[1])) == -1)
        errExit("write");
    close(fd);

    if( (fd = open(fifo_out,O_RDONLY)) == -1){
        errExit("open");
    }
    ssize_t n;
    while ((n=read(fd,buffer,BUFF_TAILLE)) > 0){
        if (write(STDOUT_FILENO,buffer, (size_t) n) < n)
            errExit("write");
    }

    close(fd);
    exit(5);

}
