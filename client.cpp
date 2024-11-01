#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#include "multiplexing.h"

constexpr char SERVER_IP[] = "127.0.0.1";
constexpr unsigned short SERVER_PORT = 8080;
constexpr int CLIENT_COUNT = 5;

static std::mutex console_mutex;

void sync_println(const std::string &message) {
    std::lock_guard lock(console_mutex);
    std::cout << message << std::endl;
}

[[noreturn]] void client_task(const int client_id) {
    const int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address{};

    if (client_socket < 0) {
        std::cerr << "Socket creation error for client " << client_id << std::endl;

        return;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    if (connect(client_socket, reinterpret_cast<sockaddr *>(&server_address), sizeof(server_address)) < 0) {
        std::cerr << "Connection failed for client " << client_id << std::endl;
        close(client_socket);

        return;
    }

    while (true) {
        char buffer[MAX_BUFFER_SIZE];
        const std::string message = "Hello from client " + std::to_string(client_id);

        send(client_socket, message.c_str(), message.length(), 0);

        sync_println("Client " + std::to_string(client_id) + " sent message to server: " + message);

        if (const int n = static_cast<int>(read(client_socket, buffer, 1024)); n > 0) {
            sync_println("Client " + std::to_string(client_id) + " received from server: " + buffer);
        }
    }
}

int main() {
    std::vector<std::thread> clients;
    clients.reserve(CLIENT_COUNT);

    for (int i = 1; i <= CLIENT_COUNT; i++) {
        clients.emplace_back(client_task, i);
    }

    for (auto &client: clients) {
        client.join();
    }

    return 0;
}
