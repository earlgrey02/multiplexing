#ifndef MULTIPLEXING_H
#define MULTIPLEXING_H
#include <netinet/in.h>

constexpr int MAX_BUFFER_SIZE = 1024;
constexpr int MAX_EVENT_COUNT = 1024;

class EventLoop {
    const int &server_socket;
    const sockaddr_in &server_address;
    const socklen_t &address_len;

public:
    EventLoop(const int &server_socket, const sockaddr_in &server_address, const socklen_t &address_len): server_socket(server_socket), server_address(server_address), address_len(address_len) {}

    [[nodiscard]] int run() const;
};

#endif
