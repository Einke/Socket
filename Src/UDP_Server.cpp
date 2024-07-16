#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
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
    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "创建套接字失败" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    // 绑定套接字到指定端口
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "绑定端口失败" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "服务器正在运行，端口号：" << SERVER_PORT << endl;

    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        char buffer[BUFFER_SIZE];

        // 接收来自客户端的信息
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            cerr << "接收数据失败" << endl;
            continue;
        }

        buffer[bytesReceived] = '\0'; // 将接收到的数据加上字符串终止符

        //cout << "收到消息: " << buffer << endl;
        // 获取客户端的 IP 地址和端口号
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        int clientPort = ntohs(clientAddr.sin_port);

        cout << "收到来自 " << clientIP << ":" << clientPort << " 的消息: " << buffer << endl;

        // 将消息回传给客户端
        int bytesSent = sendto(serverSocket, buffer, bytesReceived, 0, (sockaddr*)&clientAddr, clientAddrSize);
        if (bytesSent == SOCKET_ERROR) {
            cerr << "发送数据失败" << endl;
        }
    }

    // 关闭套接字
    closesocket(serverSocket);
    // 清理 Winsock 库
    WSACleanup();
    return 0;
}







