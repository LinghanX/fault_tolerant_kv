#include <iostream>
#include "include/spdlog/spdlog.h"
#include "include/spdlog/sinks/stdout_color_sinks.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXDATASIZE 1024

void print_all(std::vector<std::string> tokens) {
    for (const auto& n : tokens) {
        std::cout << n << std::endl;
    }
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char **argv) {
    auto console = spdlog::stdout_color_mt("console");
    console -> info("welcome to my key value store!");

    std::string address, port_num, user_input;
    std::tie(address, port_num) = handle_input(argc, argv);

    // --------- start connecting ------

    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(address.c_str(), port_num.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }
    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo);

    while (true) {
        getline(std::cin, user_input);
        std::vector<std::string> tokens;

        char *input = strdup(user_input.c_str());
        char *token = strtok(input, " ");
        while (token != nullptr) {
            tokens.push_back(token);
            token = strtok(NULL, " ");
        }

        std::string command = tokens[0];

        if (command == "get") {
            send(sockfd, &command, command.size(), 0);
        } else if (command == "put") {
            print_all(tokens);
        } else if (command == "join") {
            print_all(tokens);
        } else if (command == "kill") {
            print_all(tokens);
        } else {
            console -> error("unable to recognize command");
        }
    }
    return 0;
}
