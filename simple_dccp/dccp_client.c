#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 4444
#define BUFFER_SIZE 1024

// pipe into wireshark:
// 


int main() {
    int server_sock;
    struct sockaddr_in server_addr;
    
    char buffer[BUFFER_SIZE];
    
    if ((server_sock = socket(AF_INET, SOCK_DCCP, IPPROTO_DCCP)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("172.16.93.3");
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }
    
   if (listen(server_sock, 3) == -1) {
    perror("Listen failed");
    close(server_sock);
    exit(EXIT_FAILURE);
}

printf("Server is listening on port %d...\n", PORT);

struct sockaddr_in client_addr;
socklen_t client_addr_len = sizeof(client_addr);
int new_sock;

if ((new_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_len)) == -1) {
    perror("Accept failed");
    close(server_sock);
    exit(EXIT_FAILURE);
}

printf("Connection established with client.\n");

while(1) {
    int len = recv(new_sock, buffer, BUFFER_SIZE, 0);
    if (len == -1) {
        perror("Receive failed");
        close(new_sock);
        close(server_sock);
        exit(EXIT_FAILURE);
    } else if (len == 0) {
        printf("Client disconnected.\n");
        close(new_sock);
        break;
    } else {
        buffer[len] = '\0';
        printf("Received message: %s\n", buffer);
    }
}
    
    close(server_sock);
    return 0;
}
