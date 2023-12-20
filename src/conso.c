#include <file.h>
#include <stdbool.h>

int main(void){
    file_t *file = 0;

    recuperer_file_client(&file);
    data_t data;

    int val = 0;
    for (int i = 0; i < 8; ++i) {
        data.pid=i;
        sem_getvalue(&file->vide,&val);
        printf("%d\n",val);
        enfiler(file,data);
    }


}
