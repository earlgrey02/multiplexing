#include <iostream>
#include <unistd.h>

#include "multiplexing.h"

constexpr int PORT = 8080;

int main() {
    sockaddr_in address{};
    constexpr socklen_t address_len = sizeof(address);
    const int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == 0) {
        std::cerr << "socket() failed";

        return EXIT_FAILURE;
    }

    constexpr int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt() failed";

        return EXIT_FAILURE;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_socket, reinterpret_cast<sockaddr *>(&address), sizeof(address)) < 0) {
        std::cerr << "bind() failed";

        return EXIT_FAILURE;
    }

    const auto *event_loop = new EventLoop(server_socket, address, address_len);

    if (event_loop->run() < 0) {
        std::cerr << "run() failed";

        return EXIT_FAILURE;
    }

    close(server_socket);

    return 0;
}
