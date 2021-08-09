#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <stdio.h>
#include <fcntl.h>

struct msg {
    long mtype;
    char mtext[80];
};

int main() {
    key_t key = ftok("/tmp/msg.temp", 1);
    int msgid = msgget(key, IPC_CREAT | 0666);
    struct msg getmsg;
    memset(getmsg.mtext, 0, sizeof(getmsg.mtext));
    msgrcv(msgid, &getmsg, sizeof(getmsg.mtext), 0, 0);
    int fd = open("message.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    write(fd, getmsg.mtext, strlen(getmsg.mtext));
    close(fd);
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}