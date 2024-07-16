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
    // ���ļ��Զ�����ģʽд��
    ofstream file(savePath, ios::binary);
    if (!file) {
        cerr << "���ļ�д��ʱ����" << savePath << "\n";
        closesocket(clientSocket);
        return;
    }
    // �������ݲ�д���ļ�
    while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
        if (bytesRead == SOCKET_ERROR) {
            cerr << "��������ʱ���� " << WSAGetLastError() << "\n";
            file.close();
            closesocket(clientSocket);
            return;
        }
        file.write(buffer, bytesRead);
    }

    file.close();

    if (bytesRead == 0) {
        cout << "�ļ����ܲ����浽" << savePath << "\n";
    }
    else {
        cerr << "��������رղ����ִ��� " << WSAGetLastError() << "\n";
    }

    closesocket(clientSocket);
}

int main() {
    // ��ʼ�� Winsock ��
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup ʧ�ܣ����ִ��� " << WSAGetLastError() << "\n";
        return 1;
    }
    // ���� TCP �׽���
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "�����׽���ʱ���� " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }
    // ���÷�������ַ�Ͷ˿�
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);
    // �󶨵�ַ�Ͷ˿ڵ��׽���
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "��ʧ�ܲ����ִ��� " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    // ������������
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "����ʧ�ܲ����ִ���" << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    // ��ȡ�����ļ���·��
    cout << "�����յ����ļ��ı���·����";
    string savePath;
    getline(cin, savePath);

    cout << "���������ڼ����˿�" << SERVER_PORT << "...\n";

    while (true) {
        // ���ܿͻ�������
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "����ʧ�ܲ����ִ��� " << WSAGetLastError() << "\n";
            continue;
        }

        cout << "�ͻ��������ӡ�\n";
        // ����ͻ�������
        handleClient(clientSocket, savePath);
    }
    // �رշ������׽���
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}


