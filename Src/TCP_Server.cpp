#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <string>
#pragma comment(lib, "ws2_32.lib")
#define SERVER_PORT 54000
#define BUFFER_SIZE 1024
using namespace std;

void handleClient(SOCKET clientSocket, const string& savePath) {
    char buffer[BUFFER_SIZE];
    int bytesRead;
    // 打开文件以二进制模式写入
    ofstream file(savePath, ios::binary);
    if (!file) {
        cerr << "打开文件写入时出错：" << savePath << "\n";
        closesocket(clientSocket);
        return;
    }
    // 接收数据并写入文件
    while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
        if (bytesRead == SOCKET_ERROR) {
            cerr << "接收数据时出错： " << WSAGetLastError() << "\n";
            file.close();
            closesocket(clientSocket);
            return;
        }
        file.write(buffer, bytesRead);
    }

    file.close();

    if (bytesRead == 0) {
        cout << "文件接受并保存到" << savePath << "\n";
    }
    else {
        cerr << "连接意外关闭并出现错误： " << WSAGetLastError() << "\n";
    }

    closesocket(clientSocket);
}

int main() {
    // 初始化 Winsock 库
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup 失败，出现错误： " << WSAGetLastError() << "\n";
        return 1;
    }
    // 创建 TCP 套接字
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "创建套接字时出错： " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }
    // 设置服务器地址和端口
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);
    // 绑定地址和端口到套接字
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "绑定失败并出现错误： " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    // 监听连接请求
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "监听失败并出现错误：" << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    // 获取保存文件的路径
    cout << "输入收到的文件的保存路径：";
    string savePath;
    getline(cin, savePath);

    cout << "服务器正在监听端口" << SERVER_PORT << "...\n";

    while (true) {
        // 接受客户端连接
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "接受失败并出现错误： " << WSAGetLastError() << "\n";
            continue;
        }

        cout << "客户端已连接。\n";
        // 处理客户端请求
        handleClient(clientSocket, savePath);
    }
    // 关闭服务器套接字
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}


