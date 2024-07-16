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
    //��ʼ�� Winsock ��
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup ʧ�ܣ����ִ��� " << WSAGetLastError() << "\n";
        return 1;
    }
    // ���� TCP �׽���
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "�����׽���ʱ���� " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }
    // ���÷�������ַ�Ͷ˿�
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    // �� IP ��ַ���ı�ת��Ϊ��������ʽ
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        cerr << "��Ч�ĵ�ַ/��֧�ֵĵ�ַ�� " << WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    // ���ӵ�������
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "����ʧ�ܣ����ִ��� " << WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    // ��ȡ�û�������ļ�·��
    cout << "����Ҫ���͵��ļ���·����";
    string filePath;
    getline(cin, filePath);
    // ���ļ����ж�ȡ
    ifstream file(filePath, ios::binary);
    if (!file) {
        cerr << "���ļ���ȡʱ���� " << filePath << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    // ��ȡ�ļ������͵�������
    char buffer[BUFFER_SIZE];
    while (file.read(buffer, BUFFER_SIZE)) {
        if (send(clientSocket, buffer, file.gcount(), 0) == SOCKET_ERROR) {
            cerr << "����ʧ�ܲ���ʾ���� " << WSAGetLastError() << "\n";
            file.close();
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    }

    // �������һ������
    if (file.gcount() > 0) {
        if (send(clientSocket, buffer, file.gcount(), 0) == SOCKET_ERROR) {
            cerr << "����ʧ�ܲ���ʾ���� " << WSAGetLastError() << "\n";
            file.close();
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    }
    // �ļ����ͳɹ�
    cout << "�ļ��ɹ����͡�\n";
    // �ر��ļ����׽���
    file.close();
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}





