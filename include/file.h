#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <signal.h>
#include <sys/types.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)


#define TAILLE_FILE 5
#define NOM_SHM "/shm_file"
#define BUFF_TAILLE 1024
typedef struct
{
    pid_t pid;
} data_t;

typedef struct
{
    data_t data[TAILLE_FILE];
    sem_t plein,vide;
    sem_t mutex;
    size_t queue, tete;
}file_t;

void initialiser_file(file_t **file);

void recuperer_file_client(file_t **file);

void enfiler(file_t *file, data_t val);

data_t defiler(file_t *file);

char *create_string(int pid_proc, int tube);
