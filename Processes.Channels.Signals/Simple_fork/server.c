#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    FILE* fp_parent;
    fp_parent = fopen("/tmp/pid_parent", "w");
    pid_t pid_parent = getpid();
    fprintf(fp_parent, "%d", pid_parent);
    fclose(fp_parent);
//    printf("%d\n", pid_parent);
    pid_t fork_result = fork();
    if (fork_result == 0) {
        pid_t pid_child = getpid();
//        printf("%d\n", pid_child);
        FILE* fp_child;
        fp_child = fopen("/tmp/pid_child", "w");
        fprintf(fp_child, "%d", pid_child);
        fclose(fp_child);

        struct sigaction new_action;
        new_action.sa_handler = SIG_DFL; // what to do
        sigfillset(&new_action.sa_mask); // blocking all signals during handling
        struct sigaction old_action;
        sigaction(SIGTERM, &new_action, &old_action);
//        raise(SIGTERM);
    } else {
        int status;
        waitpid(-1, &status, 0);
//        printf("%d\n", status);
    }
    pause();
    return 0;
}