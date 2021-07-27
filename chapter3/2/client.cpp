#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(12345);
    sock_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    connect(client_socket, (struct sockaddr*) &sock_addr, sizeof(sock_addr));

    char Buffer [] = "ping";
    send(client_socket, Buffer, 4, MSG_NOSIGNAL);
    recv(client_socket, Buffer, 4, MSG_NOSIGNAL);
    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);

    printf("%s\n", Buffer);

    return 0;
}