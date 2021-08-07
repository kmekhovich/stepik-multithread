#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <string.h>

struct Data {
    char* command;
    int arg_ind;
    char* argv[1024];
};

int com_sz = 0;
struct Data commands[1024];

void AddArg(char* arg) {
    int* arg_ind = &commands[com_sz].arg_ind;
    commands[com_sz].argv[*arg_ind] = malloc(sizeof(char) * 1024);
    strcpy(commands[com_sz].argv[*arg_ind], arg);
    (*arg_ind)++;
}

void AddCommand(char* command) {
    commands[com_sz].command = malloc(sizeof(char) * 1024);
    strcpy(commands[com_sz].command, command);
    commands[com_sz].arg_ind = 0;
    AddArg(command);
}

void NextCommand() {
    int* arg_ind = &commands[com_sz].arg_ind;
    commands[com_sz].argv[*arg_ind] = NULL;
    com_sz++;
}

void FreeCommands() {
    for (size_t i = 0; i < com_sz; i++) {
        free(commands[i].command);
        for (size_t j = 0; j < commands[com_sz].arg_ind; j++) {
            free(commands[i].argv[j]);
        }
    }
}

void Rec(int exec_com, int read_from_fd) {
    int fd[2];
    pipe(fd);
    int cpid = fork();
    if (cpid == 0) {
        if (exec_com != com_sz - 1) {
            dup2(fd[1], STDOUT_FILENO);
        } else {
            FILE* fd_end = fopen("/tmp/index",  "w");
            dup2(fileno(fd_end), STDOUT_FILENO);
            close(fileno(fd_end));
        }
        dup2(read_from_fd, STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        close(read_from_fd);
        execvp(commands[exec_com].command, commands[exec_com].argv);
    } else {
        int st;
        wait(&st);
        close(fd[1]);
        if (exec_com != com_sz - 1) {
            Rec(exec_com + 1, fd[0]);
        }
    }
}

int IsMatches(char x) {
    if (x == ' ' || x == '\t' || x == '\n' || x == '\r') {
        return 1;
    }
    return 0;
}

int main() {
    size_t buf_size = 1024;
    char* buf = malloc(buf_size * sizeof(char));
    read(STDIN_FILENO, buf, buf_size);
    char* buf_last = malloc(buf_size * sizeof(char));
    int spaces_ind[1024];
    int sp_ind = 0;
    for (int i = 0; buf[i] != 0; i++) {
        if (IsMatches(buf[i])) {
            spaces_ind[sp_ind++] = i;
        }
    }
    int now_args = 0;
    for (int i = 0; i < sp_ind; i++) {
        int last = (i ? spaces_ind[i - 1] : -1);
        int new_sz = spaces_ind[i] - last - 1;
        strncpy(buf_last,
                buf + last + 1,
                new_sz);
        buf_last[new_sz] = 0;
        if (strcmp(buf_last, "|") == 0) {
            NextCommand();
            now_args = 0;
            continue;
        }
        if (now_args) {
            AddArg(buf_last);
            continue;
        }
        AddCommand(buf_last);
        now_args = 1;
    }
    NextCommand();
    free(buf);
    free(buf_last);

    pid_t cpid = fork();
    if (cpid == 0) {
        Rec(0, STDIN_FILENO);
    } else {
        int st;
        wait(&st);
    }

    FreeCommands();
    return 0;
}
