#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(12345);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(master_socket, (struct sockaddr *)(&sock_addr), sizeof(sock_addr));
    listen(master_socket, SOMAXCONN);
    while (1) {
        int slave_socket = accept(master_socket, 0, 0);
        char buffer[5] = {0, 0, 0, 0, 0};
        recv(slave_socket, buffer, 4, MSG_NOSIGNAL);
        send(slave_socket, buffer, 4, MSG_NOSIGNAL);
        shutdown(slave_socket, SHUT_RDWR);
        close(master_socket);
        printf("%s\n", buffer);
    }
    return 0;
}