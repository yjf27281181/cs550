
// Created by Yuying on 4/8/20.

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <thread>

void task1(int client_socket, char* buffer, char* from_redis, int redis_sock) {
    while (1) {
        int from_app_len = read(client_socket, buffer, 10240);
        send(redis_sock, buffer, from_app_len, 0);
        int from_redis_len = read(redis_sock, from_redis, 10240);
        int to_app_len = send(client_socket, from_redis, from_redis_len, 0);
    }
}

int main(int argc, char const *argv[]) {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[10240] = {0}, from_redis[10240] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[1]));

    if (bind(server_fd, (struct sockaddr *) &address,
             sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 1000) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (true) {
        if ((client_socket = accept(server_fd, (struct sockaddr *) &address,
                                    (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        int redis_sock = 0;
        struct sockaddr_in serv_addr;
        if ((redis_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            exit(EXIT_FAILURE);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(atoi(argv[2]));

        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported \n");
            exit(EXIT_FAILURE);
        }

        if (connect(redis_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed \n");
            exit(EXIT_FAILURE);
        }

        std::thread t (task1, client_socket, buffer, from_redis, redis_sock);
        t.detach();

    }
}
