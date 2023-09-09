#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_DCCP, IPPROTO_DCCP);
    if(sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        close(sockfd);
        return 1;
    }

    if(listen(sockfd, 5) < 0) {
        std::cerr << "Error listening on socket" << std::endl;
        close(sockfd);
        return 1;
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);

    if(client_sock < 0) {
        std::cerr << "Error accepting connection" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Connection established" << std::endl;
    close(client_sock);
    close(sockfd);

    return 0;
}
