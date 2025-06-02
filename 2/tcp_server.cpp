#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>

#define BACKLOG 10

static void handle_client(int client_fd, int client_index)
{
    char buffer[1024];

    printf("Client %d connected.\n", client_index);

    while (true)
    {
        ssize_t n = recv(client_fd, buffer, sizeof(buffer), 0);
        if (n == 0)
        {
            printf("Client %d disconnected.\n", client_index);
            break;
        }
        else if (n < 0)
        {
            perror("recv");
            break;
        }

        printf("Client %d sent: %.*s\n", client_index, (int)n, buffer);

        ssize_t total_sent = 0;
        while (total_sent < n)
        {
            ssize_t sent = send(client_fd, buffer + total_sent, n - total_sent, 0);
            if (sent < 0)
            {
                perror("send");
                break;
            }
            total_sent += sent;
        }
    }

    close(client_fd);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <PORT>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, BACKLOG) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

    printf("Server listening on port %d\n", port);

    int client_counter = 0;

    while (true)
    {
        sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &addr_len);

        if (client_fd < 0)
        {
            perror("Accept failed");
            continue;
        }

        std::thread(handle_client, client_fd, client_counter).detach();

        client_counter++;
    }

    close(server_fd);
    return 0;
}

