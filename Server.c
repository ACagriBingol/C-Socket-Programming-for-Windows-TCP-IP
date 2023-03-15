#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define SERVER_PORT 8888

#pragma comment(lib, "ws2_32.lib")


int main() {

    WSADATA wsa;
    SOCKET listenSocket, clientSocket;
    struct sockaddr_in server, client;
    char message[100];




    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed. Error code: %d", WSAGetLastError());
        return 1;
    }
    printf("Winsock initialized.\n");





    // Create a socket for the server to listen on
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        printf("Could not create socket. Error code: %d", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");




    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT);




    // Bind the socket to a specific port
    if (bind(listenSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed. Error code: %d", WSAGetLastError());
        return 1;
    }
    printf("Bind done.\n");




    // Listen for incoming connections
    listen(listenSocket, 3);




    // Accept incoming connections
    printf("Waiting for incoming connections...\n");
    int c = sizeof(struct sockaddr_in);
    while ((clientSocket = accept(listenSocket, (struct sockaddr*)&client, &c)) != INVALID_SOCKET) {
        printf("Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        // Send a welcome message to the client
        strcpy(message, "Welcome to the server!\n");
        send(clientSocket, message, strlen(message), 0);
    }
    

    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed. Error code: %d", WSAGetLastError());
        return 1;
    }

    // Cleanup
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}
