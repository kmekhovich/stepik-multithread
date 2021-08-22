#include <iostream>
#include <set>
#include <algorithm>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <poll.h>

#define POLL_SIZE 2048

int set_nonblock(int fd) {
    int flags;
#if defined(O_NONBLOCK)
    if (-1 == (flags = fcntl(fd, F_GETFL, 0))) {
        flags = 0;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}


int main() {
    int master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    std::set<int> slave_sockets;
    struct sockaddr_in sock_addr;
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(12345);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(master_socket, (struct sockaddr *)(&sock_addr), sizeof(sock_addr));
    set_nonblock(master_socket);
    listen(master_socket, SOMAXCONN);

    struct pollfd set[POLL_SIZE];
    set[0].fd = master_socket;
    set[0].events = POLLIN;

    while (true) {
        unsigned int index = 1;

        for (const auto& it : slave_sockets) {
            set[index].fd = it;
            set[index].events = POLLIN;
            index++;
        }

        unsigned int set_size = 1 + slave_sockets.size();

        poll(set, set_size, -1);

        std::vector<int> erase_sockets;
        for (unsigned int i = 0; i < set_size; i++) {
           if (set[i].revents & POLLIN) {
               if (i) {
                    static char buffer[1024];
                    int recv_size = recv(set[i].fd, buffer, 1024, MSG_NOSIGNAL);
                    if ((recv_size == 0) && (errno != EAGAIN)) {
                        shutdown(set[i].fd, SHUT_RDWR);
                        close(set[i].fd);
                        erase_sockets.push_back(set[i].fd);
                    } else if (recv_size > 0) {
                        send(set[i].fd, buffer, recv_size, MSG_NOSIGNAL);
                    }
               } else {
                   int slave_socket = accept(master_socket, 0, 0);
                   set_nonblock(slave_socket);
                   slave_sockets.insert(slave_socket);
               }
           }
        }
        for (const auto& it : erase_sockets) {
            slave_sockets.erase(it);
        }
    }
    return 0;
}