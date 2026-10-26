#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "DisplayProtocol.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "WSAStartup failed" << endl;
        return 1;
    }

    SOCKET serverSocket =
        socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Socket creation failed" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket,
        (sockaddr*)&serverAddr,
        sizeof(serverAddr)) == SOCKET_ERROR)
    {
        cout << "Bind failed" << endl;

        closesocket(serverSocket);
        WSACleanup();

        return 1;
    }

    cout << "UDP Server started on port 8888" << endl;

    char buffer[1024];

    while (true)
    {
        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);

        int bytesReceived =
            recvfrom(
                serverSocket,
                buffer,
                sizeof(buffer),
                0,
                (sockaddr*)&clientAddr,
                &clientSize);

        if (bytesReceived == SOCKET_ERROR)
        {
            cout << "Receive error" << endl;
            continue;
        }

        try
        {
            vector<uint8_t> packet(
                reinterpret_cast<uint8_t*>(buffer),
                reinterpret_cast<uint8_t*>(buffer) + bytesReceived);

            DisplayProtocolParser parser;

            Command cmd = parser.Parse(packet);

            cout << "\nCommand: "
                << static_cast<int>(cmd.type)
                << endl;

            cout << "Parameters: ";

            for (int value : cmd.params)
            {
                cout << value << " ";
            }

            cout << endl;
        }
        catch (const exception& ex)
        {
            cout << "Error: "
                << ex.what()
                << endl;
        }
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}