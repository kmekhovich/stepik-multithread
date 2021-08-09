#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <string.h>

#define IN_PATH "/tmp/in.fifo"
#define OUT_PATH "/tmp/out.fifo"

#define BUF_SZ 1024

int main() {
    mkfifo(IN_PATH, 0666);
    mkfifo(OUT_PATH, 0666);
    char buffer[BUF_SZ];
    size_t sz = 0;
    if (fork() == 0) {
        int in_fd = open(IN_PATH, O_RDONLY | O_NONBLOCK);
        sz = read(in_fd, buffer, BUF_SZ);
        close(in_fd);
        exit(0);
    } else {
        int out_fd = open(OUT_PATH, O_CREAT | O_WRONLY);
        if (sz > 0) {
            write(out_fd, buffer, sz);
        }
        close(out_fd);
        int st;
        wait(&st);
    }
    return 0;
}
