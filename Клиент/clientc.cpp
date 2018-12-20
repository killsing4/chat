#include <iostream>
#include "md5.h"
#include <winsock2.h>
#include <time.h>
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "work.lib")
#pragma comment(lib, "md5hash.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <cstdlib>
#include <Ws2tcpip.h>
#include <Windows.h>
#pragma warning(disable : 4996)

#define SELF_REMOVE_STRING TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del \"%s\"") 


#include <tchar.h> 
#include <string>
#include <Strsafe.h>
#include "work.h"

using namespace md5;
using namespace std;
void Help() {

	TCHAR szModuleName[MAX_PATH];
	TCHAR szCmd[2 * MAX_PATH];
	STARTUPINFO si;
	PROCESS_INFORMATION pinf;
	GetModuleFileName(NULL, szModuleName, MAX_PATH);
	StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);
	CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pinf);
	CloseHandle(pinf.hThread);
	CloseHandle(pinf.hProcess);
}

void CheckForDebugger() {
	time_t start, end;
	start = clock();
	HANDLE hOlly = FindWindow(TEXT("OLLYDBG"), NULL);
	HANDLE WireShark = FindWindow(TEXT("Wireshark"), NULL);
	HANDLE CommomView = FindWindow(TEXT("cv"), NULL);
	HANDLE hWinDbg = FindWindow(TEXT("WinDbgFrameClass"), NULL);
	HANDLE hScylla1 = FindWindow(NULL, TEXT("Scylla x86 v0.9.7c"));
	HANDLE hScylla2 = FindWindow(NULL, TEXT("Scylla x64 v0.9.7c"));
	HANDLE x32_dbg = FindWindow(NULL, TEXT("x32_dbg"));
	HANDLE x64_dbg = FindWindow(NULL, TEXT("x64_dbg"));
	HANDLE IDA = FindWindow(NULL, TEXT("IDA"));
	if (IsDebuggerPresent()) {
		exit(0);
	}
	if (hOlly) {
		Help();
		exit(0);
	}
	if (hWinDbg) {
		Help();
		exit(0);
	}
	if (hScylla1) {
		Help();
		exit(0);
	}
	if (hScylla2) {
		Help();
		exit(0);
	}
	if (x32_dbg) {
		Help();
		exit(0);
	}
	if (x64_dbg) {
		Help();
		exit(0);
	}
	if (WireShark) {
		Help();
		exit(0);
	}
	if (CommomView) {
		Help();
		exit(0);
	}
	if (IDA) {
		Help();
		exit(0);
	}
	end = clock();
	double elapsed = (double)(end - start)*10000.0 / CLOCKS_PER_SEC;
	if (elapsed > 10) {
		exit(0);
	}
}

DWORD WINAPI ReceivingMessages(LPVOID client_socket);
char buff[1024];
int nsize;
char lg;
string l = "OmXbZ3ZfqY"; 
string ll;

void prhsh() {
	time_t start1, end1;
	start1 = clock();
	string ps;
	cout << "\nВведите пароль: ";
	cin >> ps;
	ll = l + ps;

	string df = md5::md5(ll);

	end1 = clock();
	double dif = difftime(end1, start1) / 1000;
	if (dif > 20) {
		exit(0);
	}
	cout << endl;

	if (df == "e2ce63e2a464e3768d6d1ee021868274")
	{
		lg = 'l';
	}
	else lg = '1';


}



void ostanov()
{
	CONTEXT ctx = {};
	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	if (GetThreadContext(GetCurrentThread(), &ctx))
	{
		if (ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0)
		{
			std::cout << "Stop debugging program!" << std::endl;
			exit(-1);
		}
	}
}

int main()
{
	zxcv();
	CheckForDebugger();
	ostanov();
	system("chcp 1251");
	printf("***TCP Client***\n\n");
	char response[1024] = { 0 };
	WSADATA ws;
	BOOL isDebuggerPresent = FALSE;
	if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent))
	{
		if (isDebuggerPresent)
		{
			std::cout << "Stop debugging program!" << std::endl;
			exit(-1);
		}
	}
	if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws)))
	{
		cout << "[WSAStartup]: ERROR #" << WSAGetLastError() << endl;
	}
	else {

	}

	SOCKET s;
	if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
	{
		cout << "[Socket]: ERROR #" << WSAGetLastError() << endl;
	}
	else {
	
	}


	sockaddr_in adr;
	adr.sin_family = AF_INET;
	prhsh();


	if (lg == '1')
		exit(0);

	if (lg == 'l') {

		cout << "Введите local или host: ";
		char extra[16];
		cin >> extra;
		if (!strcmp(extra, "local"))
		{
			adr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
			adr.sin_port = htons(2131);
		}
		else if (!strcmp(extra, "host"))
		{
			char host[16];
			u_short port;
			cout << "IP: ";
			cin >> host;
			cout << "Port: ";
			cin >> port;
			cout << "***Подключение***\n";
			adr.sin_port = htons(port);
			adr.sin_addr.S_un.S_addr = inet_addr(host);
		}

		if (SOCKET_ERROR == (connect(s, (sockaddr *)&adr, sizeof(adr))))
		{
			cout << "[Connect]: ERROR #" << WSAGetLastError() << "\n\n\n";
		}
		else {
			
			DWORD thID;
			CreateThread(NULL, NULL, ReceivingMessages, &s, NULL, &thID);

			while (1)
			{
				fgets(&buff[0], sizeof(buff) - 1, stdin);

				// проверка на "quit"
				if (!strcmp(&buff[0], "quit\n"))
				{
					// Корректный выход
					printf("***Exit***");
					closesocket(s);
					WSACleanup();
					return 0;
				}

				// передаем строку клиента серверу
				send(s, &buff[0], nsize, 0);
			}
		}

		closesocket(s);
		WSACleanup();
		system("PAUSE");
		return 0;
	}
}

DWORD WINAPI ReceivingMessages(LPVOID client_socket)
{

	SOCKET my_sock;
	my_sock = ((SOCKET *)client_socket)[0];
	while (1)
	{

		nsize = recv(my_sock, buff, sizeof(buff), 0);
		buff[nsize] = 0;


		printf("Recd: %s\n", buff);
	}
}