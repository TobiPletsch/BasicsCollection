#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation error");
        return 1;
    }

    // Server address configuration
    struct hostent *server = gethostbyname("server"); // Use the service name
    if (server == nullptr) {
        std::cerr << "Error: Unable to resolve server hostname." << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Use the port your server listens on

    // Copy the resolved IP address
    memcpy(&serverAddr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Send a message to the server
    const char *message = "Hello, Server!";
    sendto(clientSocket, message, strlen(message), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // Close the socket
    close(clientSocket);

    return 0;
}
