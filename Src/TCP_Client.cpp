#include <winsock2.h>
#include <ws2tcpip.h> 
#include <iostream>
#include <fstream>
#include <string>
#pragma comment(lib, "ws2_32.lib")
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 54000
#define BUFFER_SIZE 1024
using namespace std;

int main() {
    //初始化 Winsock 库
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup 失败，出现错误： " << WSAGetLastError() << "\n";
        return 1;
    }
    // 创建 TCP 套接字
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "创建套接字时出错： " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }
    // 设置服务器地址和端口
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    // 将 IP 地址从文本转换为二进制形式
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        cerr << "无效的地址/不支持的地址： " << WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    // 连接到服务器
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "连接失败，出现错误： " << WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    // 获取用户输入的文件路径
    cout << "输入要发送的文件的路径：";
    string filePath;
    getline(cin, filePath);
    // 打开文件进行读取
    ifstream file(filePath, ios::binary);
    if (!file) {
        cerr << "打开文件读取时出错： " << filePath << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    // 读取文件并发送到服务器
    char buffer[BUFFER_SIZE];
    while (file.read(buffer, BUFFER_SIZE)) {
        if (send(clientSocket, buffer, file.gcount(), 0) == SOCKET_ERROR) {
            cerr << "发送失败并显示错误： " << WSAGetLastError() << "\n";
            file.close();
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    }

    // 处理最后一块数据
    if (file.gcount() > 0) {
        if (send(clientSocket, buffer, file.gcount(), 0) == SOCKET_ERROR) {
            cerr << "发送失败并显示错误： " << WSAGetLastError() << "\n";
            file.close();
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    }
    // 文件发送成功
    cout << "文件成功发送。\n";
    // 关闭文件和套接字
    file.close();
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}





