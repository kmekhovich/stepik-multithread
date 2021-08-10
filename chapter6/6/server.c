#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main() {
    key_t key = ftok("/tmp/mem.temp", 1);
    size_t sz = 1024 * 1024;
    int shmid = shmget(key, sz, IPC_CREAT | 0666);
    void* ptr = shmat(shmid, (void *)0, 0);
    void* end = ptr + sz;
    int cnt = 0;
    for (char* cur_ptr = ptr; cur_ptr != end; cur_ptr++) {
        *cur_ptr = 42;
        cnt++;
    }
//    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}