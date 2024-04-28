#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <cstring>
#define DEFAULT_PORT "8080"
#define DEFAULT_BUFLEN 512

#pragma comment(lib, "Ws2_32.lib")

int main() {
    // for recv and send functions
    char recvbuf[DEFAULT_BUFLEN];
    int iResult, iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;

    WSADATA wsaData;

    // initialize WSAStartup 
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup Failed";
        exit(1);
    }

    // set addrinfo and pointers to addrinfo for getaddrinfo();
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // use getaddrino and check to make sure it executed correctly
    if (getaddrinfo("localhost", DEFAULT_PORT, &hints, &result) != 0) {
        std::cerr << "getaddrinfo Failed: \n" << WSAGetLastError();;
        WSACleanup();
        exit(1);
    }

    //Create a socket to listen for client connections
    SOCKET listenSocket = INVALID_SOCKET;

    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket Failed: \n" << WSAGetLastError();;
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }
    
    // bind the socket and check for errors
    if (bind(listenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        std::cerr << "Bind Failed: \n" << WSAGetLastError();
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }
    // if no error the socket has been bound and the infomation in result is no longer needed
    freeaddrinfo(result);

    // call the listen function to listen for incoming requests, check for errors
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen Failed: \n" << WSAGetLastError();
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }

    // Create a temp socket to accept connections from clients, check for errors
    SOCKET clientSocket = INVALID_SOCKET;
    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept Failed: \n" << WSAGetLastError();;
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }

    // close the listenSocket to prevent more connections
    closesocket(listenSocket);

    // time to receive and send info till client disconnects
    
    do {
        iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            std::cout << "Bytes received: \n" << iResult << std::endl;
            std::string utf8Text(recvbuf, strlen(recvbuf));
            std:: cout << "Message: " << utf8Text << std::endl;
            iSendResult = send(clientSocket, recvbuf, strlen(recvbuf), 0);
            if (iSendResult == SOCKET_ERROR) {
                std::cerr << "Send Failed: \n" << WSAGetLastError();
                closesocket(clientSocket);
                WSACleanup();
                exit(1);
            }
            std::cout << "Bytes sent: \n" << iSendResult << std::endl;
        }
        else if (iResult == 0) {
            std::cout << "Connection closing...\n";
        }
        else {
            std::cerr << "Recv Failed: \n" << WSAGetLastError();
            closesocket(clientSocket);
            WSACleanup();
            exit(1);
        }
    } while (iResult > 0);

    // Stop sending information to client since we are done
    iResult = shutdown(clientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "Shutdown Failed: \n" << WSAGetLastError();
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }

    // cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}