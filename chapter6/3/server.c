#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>

int main() {
    struct mq_attr attr;
    int mqd = mq_open("/test.mq", O_RDONLY | O_CREAT, 0666, &attr);
    char buf[1024];
    memset(buf, 0, 1024);
    mq_receive(mqd, buf, attr.mq_msgsize, NULL);
    int fd = open("/home/box/message.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    write(fd, buf, strlen(buf));
    close(fd);
    mq_close(mqd);
    return 0;
}