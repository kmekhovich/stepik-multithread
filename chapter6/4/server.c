#include <sys/ipc.h>
#include <sys/sem.h>

int main() {
    key_t key = ftok("/tmp/sem.temp", 1);
    int semid = semget(key, 16, IPC_CREAT | 0666);

    unsigned short array[16];
    for (int i = 0; i < 16; i++) {
        array[i] = i;
    }
    semctl(semid, 0, SETALL, array);
//    semctl(semid, 0, IPC_RMID);
    return 0;
}