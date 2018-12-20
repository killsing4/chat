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


PIMAGE_NT_HEADERS GetImageNtHeaders(PBYTE pImageBase)
{
	PIMAGE_DOS_HEADER pImageDosHeader = (PIMAGE_DOS_HEADER)pImageBase;
	return (PIMAGE_NT_HEADERS)(pImageBase + pImageDosHeader->e_lfanew);
}
PIMAGE_SECTION_HEADER FindRDataSection(PBYTE pImageBase)
{
	static const std::string rdata = ".rdata";
	PIMAGE_NT_HEADERS pImageNtHeaders = GetImageNtHeaders(pImageBase);
	PIMAGE_SECTION_HEADER pImageSectionHeader = IMAGE_FIRST_SECTION(pImageNtHeaders);
	int n = 0;
	for (; n < pImageNtHeaders->FileHeader.NumberOfSections; ++n)
	{
		if (rdata == (char*)pImageSectionHeader[n].Name)
		{
			break;
		}
	}
	return &pImageSectionHeader[n];
}
void CheckGlobalFlagsClearInProcess()
{
	PBYTE pImageBase = (PBYTE)GetModuleHandle(NULL);
	PIMAGE_NT_HEADERS pImageNtHeaders = GetImageNtHeaders(pImageBase);
	PIMAGE_LOAD_CONFIG_DIRECTORY pImageLoadConfigDirectory = (PIMAGE_LOAD_CONFIG_DIRECTORY)(pImageBase
		+ pImageNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress);
	if (pImageLoadConfigDirectory->GlobalFlagsClear != 0)
	{
		std::cout << "Stop debugging program!" << std::endl;
		exit(-1);
	}
}
void CheckGlobalFlagsClearInFile()
{
	HANDLE hExecutable = INVALID_HANDLE_VALUE;
	HANDLE hExecutableMapping = NULL;
	PBYTE pMappedImageBase = NULL;
	__try
	{
		PBYTE pImageBase = (PBYTE)GetModuleHandle(NULL);
		PIMAGE_SECTION_HEADER pImageSectionHeader = FindRDataSection(pImageBase);
		TCHAR pszExecutablePath[MAX_PATH];
		DWORD dwPathLength = GetModuleFileName(NULL, pszExecutablePath, MAX_PATH);
		if (0 == dwPathLength) __leave;
		hExecutable = CreateFile(pszExecutablePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (INVALID_HANDLE_VALUE == hExecutable) __leave;
		hExecutableMapping = CreateFileMapping(hExecutable, NULL, PAGE_READONLY, 0, 0, NULL);
		if (NULL == hExecutableMapping) __leave;
		pMappedImageBase = (PBYTE)MapViewOfFile(hExecutableMapping, FILE_MAP_READ, 0, 0,
			pImageSectionHeader->PointerToRawData + pImageSectionHeader->SizeOfRawData);
		if (NULL == pMappedImageBase) __leave;
		PIMAGE_NT_HEADERS pImageNtHeaders = GetImageNtHeaders(pMappedImageBase);
		PIMAGE_LOAD_CONFIG_DIRECTORY pImageLoadConfigDirectory = (PIMAGE_LOAD_CONFIG_DIRECTORY)(pMappedImageBase
			+ (pImageSectionHeader->PointerToRawData
				+ (pImageNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress - pImageSectionHeader->VirtualAddress)));
		if (pImageLoadConfigDirectory->GlobalFlagsClear != 0)
		{
			std::cout << "Stop debugging program!" << std::endl;
			exit(-1);
		}
	}
	__finally
	{
		if (NULL != pMappedImageBase)
			UnmapViewOfFile(pMappedImageBase);
		if (NULL != hExecutableMapping)
			CloseHandle(hExecutableMapping);
		if (INVALID_HANDLE_VALUE != hExecutable)
			CloseHandle(hExecutable);
	}
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

typedef NTSTATUS(NTAPI *pfnNtSetInformationThread)(
	_In_ HANDLE ThreadHandle,
	_In_ ULONG  ThreadInformationClass,
	_In_ PVOID  ThreadInformation,
	_In_ ULONG  ThreadInformationLength
	);
const ULONG ThreadHideFromDebugger = 0x11;
void HideFromDebugger()
{
	HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));
	pfnNtSetInformationThread NtSetInformationThread = (pfnNtSetInformationThread)
		GetProcAddress(hNtDll, "NtSetInformationThread");
	NTSTATUS status = NtSetInformationThread(GetCurrentThread(),
		ThreadHideFromDebugger, NULL, 0);
}

int main()
{
	zxcv();
	CheckForDebugger();
	CheckGlobalFlagsClearInProcess();
	CheckGlobalFlagsClearInFile();
	ostanov();
	HideFromDebugger();
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