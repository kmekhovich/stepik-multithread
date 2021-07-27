#include <iostream>
#include <set>
#include <algorithm>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

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
    while (true) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(master_socket, &set);
        for (const auto& it : slave_sockets) {
            FD_SET(it, &set);
        }

        int max_slave = (slave_sockets.empty() ? 0 : *prev(slave_sockets.end()));
        int max = std::max(master_socket, max_slave);

        select(max + 1, &set, NULL, NULL, NULL);

        std::vector<int> erase_sockets;

        for (const auto& it : slave_sockets) {
            if (FD_ISSET(it, &set)) {
                static char buffer[1024];
                int RecvSize = recv(it, buffer, 1024, MSG_NOSIGNAL);
                if ((RecvSize == 0) && (errno != EAGAIN)) {
                    shutdown(it, SHUT_RDWR);
                    close(it);
                    erase_sockets.push_back(it);
                } else if (RecvSize != 0) {
                    send(it, buffer, RecvSize, MSG_NOSIGNAL);
                }
            }
        }

        for (const auto& erase_socket : erase_sockets) {
            slave_sockets.erase(erase_socket);
        }

        if (FD_ISSET(master_socket, &set)) {
            int slave_socket = accept(master_socket, 0, 0);
            set_nonblock(slave_socket);
            slave_sockets.insert(slave_socket);
        }
    }
    return 0;
}