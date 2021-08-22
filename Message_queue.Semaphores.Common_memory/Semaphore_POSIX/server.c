#include <fcntl.h>
#include <semaphore.h>

int main() {
    sem_t* sem = sem_open("/test.sem", O_CREAT, 0666, 66);
//    sem_close(sem);
    return 0;
}