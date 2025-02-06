#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <bits/stdc++.h>
#include <regex>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    std::vector<std::string> initial_messages = {"The possible commands are: ", 
        "add(number1,number2)", "sub(number1,number2)", "div(number1,number2)", "mul(number1,number2)", "EXIT"};

    std::vector<std::regex> regex_vec = {std::regex(R"(add\(-?\d+(\.\d+)?,-?\d+(\.\d+)?\))"), 
        std::regex(R"(sub\(-?\d+(\.\d+)?,-?\d+(\.\d+)?\))"), std::regex(R"(div\(-?\d+(\.\d+)?,-?\d+(\.\d+)?\))"),
        std::regex(R"(mul\(-?\d+(\.\d+)?,-?\d+(\.\d+)?\))")};

    for (std::string initial_message : initial_messages) {
        std::cout << initial_message << std::endl;
    }

    char buffer[1024] = {0};
    std::string message = "";
    while(true) {
        std::cout << '>'; 
        getline(std::cin,message);
        if (message == "EXIT") {
            send(sock, message.c_str(), message.length(), 0);
            shutdown(sock, SHUT_RDWR);
            close(sock);
            break;
        }
        bool valid_msg = false;
        for (std::regex operation_regex : regex_vec) {
            if (std::regex_match(message,operation_regex)) {
                valid_msg = true;
                break;
            }
        }
        if (valid_msg) {
            send(sock, message.c_str(), message.length(), 0);
            read(sock, buffer, sizeof(buffer));
            std::cout << buffer << std::endl;
            memset(buffer, 0, sizeof(buffer));
        }
        else {
            std::cout << "Invalid message format!" << std::endl;
        }
    }

    return 0;
}
