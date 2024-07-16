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
    // ��ʼ�� Winsock ��
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup ��ʼ��ʧ��" << endl;
        return 1;
    }
    // ���� UDP �׽���
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "�����׽���ʧ��" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = htons(SERVER_PORT);

    while (true) {
        string message;
        cout << "������Ҫ���͵���Ϣ: ";
        getline(cin, message);  

        // ������Ϣ��������
        int bytesSent = sendto(clientSocket, message.c_str(), message.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
        if (bytesSent == SOCKET_ERROR) {
            cerr << "��������ʧ��" << endl;
            continue;
        }

        char buffer[BUFFER_SIZE];
        sockaddr_in fromAddr;
        int fromAddrSize = sizeof(fromAddr);

        // �������Է������Ļش���Ϣ
        int bytesReceived = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&fromAddr, &fromAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            cerr << "��������ʧ��" << endl;
            continue;
        }

        buffer[bytesReceived] = '\0'; // �����յ������ݼ����ַ�����ֹ��

        cout << "�յ��ش���Ϣ: " << buffer << endl;
    }

    // �ر��׽���
    closesocket(clientSocket);
    // ���� Winsock ��
    WSACleanup();
    return 0;
}






