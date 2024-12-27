#include "multiplexing.h"

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <sys/socket.h>

int EventLoop::run() const {
    int kq;
    std::vector<struct kevent> change_events;
    struct kevent events[MAX_EVENT_COUNT];
    struct kevent changed_event{};

    if (listen(server_socket, 5) < 0) {
        std::cerr << "listen() failed";

        return -1;
    }

    if ((kq = kqueue()) < 0) {
        std::cerr << "kqueue() failed";

        return -1;
    }

    EV_SET(&changed_event, server_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    change_events.push_back(changed_event);

    std::cout << "Server listening on " << ntohs(server_address.sin_port) << std::endl;

    while (true) {
        if (kevent(kq, change_events.data(), static_cast<int>(change_events.size()), events, MAX_EVENT_COUNT, nullptr) == -1) {
            std::cerr << "kevent() error";
            break;
        }

        change_events.clear();

        for (const auto &event: events) {
            if (event.flags & EV_ERROR) {
                continue;
            }

            if (event.filter == EVFILT_READ) {
                if (event.ident == server_socket) {
                    const int client_socket =
                            accept(
                                server_socket,
                                reinterpret_cast<struct sockaddr *>(const_cast<struct sockaddr_in *>(&server_address)),
                                const_cast<socklen_t *>(&address_len)
                            );

                    if (client_socket < 0) {
                        std::cerr << "accept() failed";
                        continue;
                    }

                    char client_address[address_len];
                    inet_ntop(AF_INET, &server_address.sin_addr, client_address, address_len);
                    std::cout << "New connection from " << client_address << ":" << ntohs(server_address.sin_port) << std::endl;

                    EV_SET(&changed_event, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    change_events.push_back(changed_event);
                } else {
                    const int client_socket = static_cast<int>(event.ident);
                    char buffer[MAX_BUFFER_SIZE];
                    const long n = read(client_socket, buffer, sizeof(buffer));

                    if (n <= 0) {
                        std::cout << "Client disconnected, fd: " << client_socket << std::endl;
                        close(client_socket);
                    } else {
                        std::cout << "Received: " << buffer << std::endl;
                        write(client_socket, buffer, n);
                    }
                }
            }
        }
    }

    return 0;
}
