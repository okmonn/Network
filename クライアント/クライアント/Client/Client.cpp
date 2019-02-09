#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Client.h"
#include <ws2tcpip.h>
#include <tchar.h>

#pragma comment(lib,"ws2_32.lib")

// データ最大数
#define DATA_MAX 256

// コンストラクタ
Client::Client() : 
	sock(INVALID_SOCKET)
{
	data    = {};
	addr    = {};
	fds     = {};
	readfds = {};
	time    = { 0, 1000 };

	Init();
}

// デストラクタ
Client::~Client()
{
	closesocket(sock);
	WSACleanup();
}

// 初期スタートアップ
int Client::StartUp(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n初期スタートアップ：失敗\n"));
	}

	return hr;
}

// ソケットの生成
int Client::CreateSock(void)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		OutputDebugString(_T("\nソケットの生成：失敗\n"));
		return -1;
	}

	return 0;
}

// 接続
int Client::Connect(void)
{
	auto hr = connect(sock,  reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (hr != 0)
	{
		OutputDebugString(_T("\nホストとの接続：失敗\n"));
		return hr;
	}

	FD_SET(sock, &readfds);

	return hr;
}

// セットアップ
int Client::SetUp(void)
{
	char port[5];
	printf("ポート番号を入力\n");
	scanf_s("%s", port, _countof(port));
	fflush(stdin);

	char tmp[INET_ADDRSTRLEN];
	printf("IPアドレスまたはPC名を入力\n");
	scanf_s("%s", tmp, _countof(tmp));

	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(std::atoi(port));
	addr.sin_addr.S_un.S_addr = inet_addr(tmp);

	auto hr = 0;

	FD_ZERO(&readfds);
	if (addr.sin_addr.S_un.S_addr == 0xffffffff)
	{
		hostent* host = nullptr;
		host = gethostbyname(tmp);
		if (host == nullptr)
		{
			OutputDebugString(_T("\nホストがみつかりません\n"));
			return -1;
		}

		auto list = reinterpret_cast<unsigned int**>(host->h_addr_list);
		while (*list != nullptr)
		{
			addr.sin_addr.S_un.S_addr = *(*list);
			hr = Connect();
			if (hr == 0)
			{
				break;
			}

			++list;
		}
	}
	else
	{
		hr = Connect();
	}

	return hr;
}

// 初期化
void Client::Init(void)
{
	StartUp();
	CreateSock();
	SetUp();
}

// 受信
std::string Client::Recv(void)
{
	memcpy(&fds, &readfds, sizeof(fd_set));
	auto len = select(0, &fds, nullptr, nullptr, &time);
	if (len <= 0)
	{
		return std::string();
	}

	std::string msg;
	if (FD_ISSET(sock, &fds))
	{
		msg.resize(DATA_MAX);
		auto hr = recv(sock, &msg[0], msg.size(), 0);
		if (hr <= 0)
		{
			if (hr == -1)
			{
				OutputDebugString(_T("\nサーバーが立ち上がっていない\n"));
			}
			return std::string();
		}
		msg = msg.substr(0, msg.find_first_of('\0'));
	}

	return msg;
}

// 送信
void Client::Send(const char * data, const unsigned int & size)
{
	auto hr = sendto(sock, data, size, 0, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (hr != size)
	{
		OutputDebugString(_T("\n送信：失敗\n"));
	}
}
