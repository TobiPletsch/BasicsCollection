#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {

    bool messageRead = false;
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address configuration
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Use the same port as in the client
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    std::cout << "Server listening on port 8080..." << std::endl;

    // Receive and print messages from clients
    while (!messageRead) {
        char buffer[1024] = {0};
        socklen_t clientAddrLen = sizeof(struct sockaddr_in);
        ssize_t bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddr, &clientAddrLen);

        if (bytesRead > 0) {
            std::cout << "Message from client: " << buffer << std::endl;
            messageRead = true;
        }
    }

    // Close the socket (this won't be reached in this example)
    close(serverSocket);

    return 0;
}
