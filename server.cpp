#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[1024] = {0};
    const char* acknowledgment = "Acknowledged";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding socket to port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return -1;
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return -1;
    }
    std::cout << "Waiting for connections..." << std::endl;

    // Accepting incoming connection
    if ((new_socket = accept(server_fd, NULL, NULL)) < 0) {
        perror("Accept failed");
        return -1;
    }

    std::cout << "Connection established with client" << std::endl;

    read(new_socket, buffer, sizeof(buffer));
    while((std::string) buffer != "EXIT") {
        std::string message = (std::string) buffer;
        double result;
        std::string operation = message.substr(0,3);
        double operand1 = std::stod(message.substr(message.find("(")+1,message.find(",")-message.find("(")-1));
        double operand2 = std::stod(message.substr(message.find(",")+1,message.find(")")-message.find(",")-1));
        if (operation == "add") {
            result = operand1 + operand2;
        }
        else if (operation == "sub") {
            result = operand1 - operand2;
        }
        else if (operation == "div") {
            result = operand1 / operand2;
        }
        else {
            result = operand1 * operand2;
        }
        std::string answer = "The result of " + message + " is: " + std::to_string(result);
        send(new_socket, answer.c_str(), answer.length(),0);
        memset(buffer, 0, sizeof(buffer));
        read(new_socket, buffer, sizeof(buffer));
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
