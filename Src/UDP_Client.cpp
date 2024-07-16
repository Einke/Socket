#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#pragma comment(lib, "Ws2_32.lib")
#define SERVER_PORT 8888
#define BUFFER_SIZE 1024
using namespace std;

int main() {
    WSADATA wsaData;
    // 初始化 Winsock 库
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup 初始化失败" << endl;
        return 1;
    }
    // 创建 UDP 套接字
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "创建套接字失败" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = htons(SERVER_PORT);

    while (true) {
        string message;
        cout << "请输入要发送的消息: ";
        getline(cin, message);  

        // 发送消息到服务器
        int bytesSent = sendto(clientSocket, message.c_str(), message.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
        if (bytesSent == SOCKET_ERROR) {
            cerr << "发送数据失败" << endl;
            continue;
        }

        char buffer[BUFFER_SIZE];
        sockaddr_in fromAddr;
        int fromAddrSize = sizeof(fromAddr);

        // 接收来自服务器的回传消息
        int bytesReceived = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&fromAddr, &fromAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            cerr << "接收数据失败" << endl;
            continue;
        }

        buffer[bytesReceived] = '\0'; // 将接收到的数据加上字符串终止符

        cout << "收到回传消息: " << buffer << endl;
    }

    // 关闭套接字
    closesocket(clientSocket);
    // 清理 Winsock 库
    WSACleanup();
    return 0;
}






