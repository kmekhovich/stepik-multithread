#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    FILE* fp;
    fp = fopen("/tmp/pid", "w");

    struct sigaction new_action;
    new_action.sa_handler = SIG_IGN; // what to do
    sigfillset(&new_action.sa_mask); // blocking all signals during handling
    struct sigaction old_action;
    sigaction(SIGTERM, &new_action, &old_action);
    sigaction(SIGINT, &new_action, &old_action); // added two handlers
    pid_t pid = getpid();
    fprintf(fp, "%d", pid);
    fclose(fp);
//    puts("SIGTERM - ");
    kill(pid, SIGTERM);
//    puts("SIGINT - ");
    kill(pid, SIGINT);
    pause();
//    puts("SIGFPE - ");
//    kill(pid, SIGFPE);
//    puts("FINISH");
    return 0;
}