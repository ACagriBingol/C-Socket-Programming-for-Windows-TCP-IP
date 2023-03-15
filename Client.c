#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")


#define SERVER_PORT 8888
#define SERVER_IP "127.0.0.1"


int main() {

    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in server;
    char message[100];
    char server_reply[200];

    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed. Error code: %d", WSAGetLastError());
        return 1;
    }
    printf("Winsock initialized.\n");



    // Create a socket for the client
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Could not create socket. Error code: %d", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");


    // Prepare the sockaddr_in structure with the server's IP address and port number
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);


    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection failed. Error code: %d", WSAGetLastError());
        return 1;
    }
    printf("Connected to server.\n");




    // Receive a welcome message from the server
    if (recv(clientSocket, server_reply, 200, 0) < 0) {
        printf("Receive failed. Error code: %d", WSAGetLastError());
        return 1;
    }

    printf("Server reply: %s\n", server_reply);

    // Send a message to the server
    strcpy(message, "Hello from the client!");
    if (send(clientSocket, message, strlen(message), 0) < 0) {
        printf("Send failed. Error code: %d", WSAGetLastError());
        return 1;
    }

    printf("Message sent to server.\n");

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
