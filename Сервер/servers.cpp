#include <iostream>
#include <winsock2.h>
#include <string>

#pragma comment(lib, "wsock32.lib")
#pragma warning(disable : 4996)
#include <windows.h>  
#include <tchar.h> 
#include <fstream>
#include <Strsafe.h> 

using namespace std;

DWORD WINAPI WorkWithClient(LPVOID client_socket);

SOCKET massiv_socket[64];
int num_massiv_socket = 0;


int main()
{

	system("chcp 1251");
	printf("***TCP Server***\n\n");

	int client_number = 0;
	char response[1024] = { 0 };

	WSADATA ws;
	if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws)))
	{
		cout << "[WSAStartup]: ERROR #" << WSAGetLastError() << endl;
	}
	else {
		//cout << "[WSAStartup]: SPI - Success" << endl;
	}

	SOCKET s;
	if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
	{
		cout << "[Socket]: ERROR #" << WSAGetLastError() << endl;
	}
	else {
		//cout << "[Socket]: SPI - Success" << endl;
	}

	sockaddr_in adr;
	adr.sin_addr.s_addr = inet_addr("127.0.0.1");
	adr.sin_port = htons(2131);
	adr.sin_family = AF_INET;

	if (bind(s, (sockaddr *)&adr, sizeof(adr)) == SOCKET_ERROR)
	{
		cout << "[BIND]: ERROR #" << WSAGetLastError() << endl;
	}
	else {
		//cout << "[BIND]: SPI - Success" << endl;
	}

	if (listen(s, 0x100) == SOCKET_ERROR)
	{
		cout << "[LISTEN]: ERROR #" << WSAGetLastError() << endl;
	}
	else {
		//cout << "[LISTEN]: SPI - Success" << endl;
	}

	printf("\n***Ожидание подключения***\n\n");

	SOCKET client_socket;
	sockaddr_in client_addr;
	int client_addr_size = sizeof(client_addr);


	while (1)
	{
		client_socket = accept(s, (sockaddr *)&client_addr, &client_addr_size);
		massiv_socket[num_massiv_socket] = client_socket;
		num_massiv_socket++;

		HOSTENT *hst;
		hst = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, 4, AF_INET);
		printf("[%s] Входящее подключение.\n", inet_ntoa(client_addr.sin_addr));

		DWORD thID;
		CreateThread(NULL, NULL, WorkWithClient, &client_socket, NULL, &thID);
	}

	closesocket(s);
	WSACleanup();
	system("PAUSE");
	return 0;

}

DWORD WINAPI WorkWithClient(LPVOID client_socket)
{
	SOCKET my_sock;
	my_sock = ((SOCKET *)client_socket)[0];
	char buff[30 * 1024] = "123123";
#define sHELLO "Добро пожаловать!\r\n"

	send(my_sock, sHELLO, sizeof(sHELLO), 0);
	int bytes_recv = 0;
	while (1)
	{
		bytes_recv = recv(my_sock, buff, sizeof(buff), 0);
		if (bytes_recv <= 0) break;
		printf("Recd %d bytes || Messages: %s\n", bytes_recv, buff);

		for (int i = 0; i < num_massiv_socket; i++)
		{
			if (massiv_socket[i] != my_sock)
			{
				send(massiv_socket[i], buff, bytes_recv, 0);
			}
		}
		Sleep(1);
	}
	num_massiv_socket--;
	closesocket(my_sock);
	return 0;
}