#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
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
    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "�����׽���ʧ��" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    // ���׽��ֵ�ָ���˿�
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "�󶨶˿�ʧ��" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "�������������У��˿ںţ�" << SERVER_PORT << endl;

    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        char buffer[BUFFER_SIZE];

        // �������Կͻ��˵���Ϣ
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            cerr << "��������ʧ��" << endl;
            continue;
        }

        buffer[bytesReceived] = '\0'; // �����յ������ݼ����ַ�����ֹ��

        //cout << "�յ���Ϣ: " << buffer << endl;
        // ��ȡ�ͻ��˵� IP ��ַ�Ͷ˿ں�
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        int clientPort = ntohs(clientAddr.sin_port);

        cout << "�յ����� " << clientIP << ":" << clientPort << " ����Ϣ: " << buffer << endl;

        // ����Ϣ�ش����ͻ���
        int bytesSent = sendto(serverSocket, buffer, bytesReceived, 0, (sockaddr*)&clientAddr, clientAddrSize);
        if (bytesSent == SOCKET_ERROR) {
            cerr << "��������ʧ��" << endl;
        }
    }

    // �ر��׽���
    closesocket(serverSocket);
    // ���� Winsock ��
    WSACleanup();
    return 0;
}







