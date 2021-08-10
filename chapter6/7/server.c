#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

#define NAME "/test.shm"

int main() {
    int fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    size_t sz = 1024 * 1024;
    ftruncate(fd, sz);
    char* ptr = mmap((void *) 0, sz, PROT_WRITE, MAP_SHARED, fd, 0);
    char* end = ptr + sz;
    for (char* cur_ptr = ptr; cur_ptr != end; cur_ptr++) {
        *cur_ptr = 13;
    }
    munmap(ptr, sz);
//    shm_unlink(NAME);
    return 0;
}