#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <IP> <PORT>\n", argv[0]);
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
    {
        fprintf(stderr, "Invalid IP address: %s\n", ip);
        close(sock_fd);
        return 1;
    }

    if (connect(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connect failed");
        close(sock_fd);
        return 1;
    }

    printf("Connected to %s:%d — type messages to send\n", ip, port);

    char input[1024];
    char buffer[1024];

    while (fgets(input, sizeof(input), stdin))
    {
        size_t len = strlen(input);
        if (send(sock_fd, input, len, 0) < 0)
        {
            perror("Send failed");
            break;
        }

        ssize_t received = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
        if (received < 0)
        {
            perror("Receive failed");
            break;
        }
        else if (received == 0)
        {
            printf("Server closed connection.\n");
            break;
        }

        buffer[received] = '\0';
        printf("Received: %s", buffer);
    }

    printf("Exiting client.\n");
    close(sock_fd);
    return 0;
}

