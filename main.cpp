#include <iostream>
#include "include/spdlog/spdlog.h"
#include "include/spdlog/sinks/stdout_color_sinks.h"

void print_all(std::vector<std::string> tokens) {
    for (const auto& n : tokens) {
        std::cout << n << std::endl;
    }
}
int main() {
    auto console = spdlog::stdout_color_mt("console");
    console -> info("welcome to my key value store!");

    std::string user_input;

    while (true) {
        getline(std::cin, user_input);
        std::vector<std::string> tokens;
        char * input = strdup(user_input.c_str());
        char *token = strtok(input, " ");
        while (token != nullptr) {
            tokens.push_back(token);
            token = strtok(NULL, " ");
        }
        std::string command = tokens[0];
        if (command == "get") {
            print_all(tokens);
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
