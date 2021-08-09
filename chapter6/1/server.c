#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <stdio.h>

struct msg {
    long mtype;
    char msg[80];
};

int main() {
    key_t key = ftok("/tmp/msg.temp", 1);
    int msgid = msgget(key, IPC_CREAT | 0666);
    struct msg message;
    message.mtype = 1;
    strcpy(message.msg, "a");
    if (fork() == 0) {
        puts(message.msg);
        msgsnd(msgid, &message, sizeof(message.msg), 0);
        raise(SIGTERM);
    } else {
        struct msg getmsg;
        msgrcv(msgid, &getmsg, sizeof(message.msg), 0, 0);
        puts(getmsg.msg);
        wait(NULL);
    }
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}