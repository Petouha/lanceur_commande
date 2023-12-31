
#include <file.h>

void P(sem_t *sem){
    if(sem_wait(sem) == -1){
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
}


void V(sem_t *sem){
    if(sem_post(sem) == -1){
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
}

//initialise la file pour le démon
void initialiser_file(file_t **file) {
    //créer ou réduire la taille du segment de mémoire partagée
    int fd = shm_open(NOM_SHM, O_RDWR | O_CREAT | O_TRUNC, 0642);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    //défini la taille du SMP
    if (ftruncate(fd, sizeof(file_t)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    //projetté le SHM dans la mémoire du processus
    *file = (file_t *)mmap(NULL, sizeof(file_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (*file == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    //intiliase les sémaphores
    if (sem_init(&((*file)->plein), 1, 0) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    if (sem_init(&((*file)->vide), 1, TAILLE_FILE) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    if (sem_init(&((*file)->mutex), 1, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
}


void recuperer_file_client(file_t **file){
    int fd;
    //tente d'ouvrir le SHM
    if ( (fd = shm_open(NOM_SHM,O_RDWR,0642)) == -1){
        errExit("shm_open");
    }

    //projettes le SHM dans la variable passée en adresse
    *file = (file_t *) mmap(NULL,sizeof (file_t),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);

}

void enfiler(file_t *file, data_t data){
    P(&file->vide);
    P(&file->mutex);

    file->data[file->tete] = data;
    file->tete = (file->tete + 1) % TAILLE_FILE;

    V(&file->mutex);
    V(&file->plein);
}

data_t defiler(file_t *file){

    P(&file->plein);
    P(&file->mutex);

    data_t return_value = file->data[file->queue];
    file->queue = (file->queue + 1) %  TAILLE_FILE;

    V(&file->mutex);
    V(&file->vide);

    return return_value;
}


//fonction qui crée le tube nommé
char *create_string(int pid_proc, int tube){
    char *return_value = malloc(sizeof(char)*256);
    switch (tube) {
        case STDIN_FILENO:
            snprintf(return_value,256,"tube_in_%d",pid_proc);
            break;
        case STDOUT_FILENO:
            snprintf(return_value,256,"tube_out_%d",pid_proc);
            break;
        case STDERR_FILENO:
            snprintf(return_value,256,"tube_err_%d",pid_proc);
            break;
        default:
            exit(EXIT_FAILURE);
    }
    return return_value;
}

//fonction qui renvoie un tableau de string contenant les commandes à éxecuter
char **get_command_string(char *argv){
    char **anal;
    char result[MAX_STRING_LENGTH] = "";
    // pour séparer les commandes
    char **split_components = malloc(MAX_COMPONENTS * sizeof(char *));  // pour retourner le tableau

    if (split_components == NULL)
        errExit("malloc");
    anal = analyse_arg(argv);

    // concaténer le resultat de l'analyse dans une variable
    for (int i = 0; anal[i] != NULL; ++i) {
        strcat(result, anal[i]);
        strcat(result, " ");
    }
    // séparer les commandes par |
    char *parcours = strtok(result, "|");
    int i = 0;

    while (parcours != NULL && i < MAX_COMPONENTS) {
        split_components[i] = strdup(parcours);  // Store a copy of the component
        parcours = strtok(NULL, "|");
        i++;
    }

    return  split_components;
}

int array_length(char **arr) {
    int length = 0;

    while (arr[length] != NULL) {
        length++;
    }

    return length;
}
