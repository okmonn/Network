#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include <ws2tcpip.h>
#include <tchar.h>
#include <algorithm>

#pragma comment(lib,"ws2_32.lib")

// クライアント数
#define CLIENT_MAX 10

// データ最大数
#define DATA_MAX 256

// コンストラクタ
Server::Server() : 
	sock(INVALID_SOCKET)
{
	data    = {};
	addr    = {};
	fds     = {};
	readfds = {};
	time    = { 0, 1000 };

	client.resize(CLIENT_MAX, INVALID_SOCKET);
	clientAddr.resize(CLIENT_MAX);

	Init();
}

// デストラクタ
Server::~Server()
{
	for (auto& i : client)
	{
		closesocket(i);
	}
	closesocket(sock);
	WSACleanup();
}

// 初期スタートアップ
int Server::StartUp(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n初期スタートアップ：失敗\n"));
	}

	return hr;
}

// ソケットの生成
int Server::CreateSock(void)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		OutputDebugString(_T("\nソケットの生成：失敗\n"));
		return -1;
	}

	return 0;
}

// セットアップ
int Server::SetUp(void)
{
	char port[5];
	{
		printf("ポート番号を入力\n");
		scanf_s("%s", port, unsigned int(_countof(port)));
		fflush(stdin);
	}

	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(std::atoi(port));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	int flag = 1;
	auto hr = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&flag), sizeof(flag));
	if (hr != 0)
	{
		OutputDebugString(_T("\nソケットのセット：失敗\n"));
	}

	return hr;
}

// アドレスとの関連付け
int Server::Bind(void)
{
	auto hr = bind(sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
	if (hr != 0)
	{
		OutputDebugString(_T("\nアドレスとの関連付け：失敗\n"));
	}
	else
	{
		printf("サーバー立ち上げ：成功\n");
	}

	return hr;
}

// 接続待機
int Server::Listen(void)
{
	auto hr = listen(sock, 5);
	if (hr != 0)
	{
		OutputDebugString(_T("\n接続待機：失敗\n"));
	}

	return hr;
}

// 初期化
void Server::Init(void)
{
	StartUp();
	CreateSock();
	SetUp();
	Bind();
	Listen();
}

// クライアントとの接続
void Server::Accept(void)
{
	FD_ZERO(&readfds);
	for (unsigned int i = 0; i < client.size(); ++i)
	{
		if (client[i] != INVALID_SOCKET)
		{
			continue;
		}

		int size = static_cast<int>(sizeof(sockaddr_in));
		client[i] = accept(sock, reinterpret_cast<struct sockaddr*>(&clientAddr[i]), &size);
		if (client[i] != INVALID_SOCKET)
		{
			FD_SET(client[i], &readfds);
			char str[INET_ADDRSTRLEN];
			printf("IP：%sが接続\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
		}
	}
}

// 送信
int Server::Send(const std::string & data)
{
	if (data.find('/') == std::string::npos)
	{
		return -1;
	}

	std::string ip(data.substr(0, data.find_first_of('/')));
	for (unsigned int i = 0; i < client.size(); ++i)
	{
		if (clientAddr[i].sin_addr.S_un.S_addr == inet_addr(ip.c_str()))
		{
			std::string tmp(data.substr(data.find_first_of('/') + 1, data.find_first_of('\0')));
			auto n = ip + "：" + tmp;
			auto hr = send(client[i], n.c_str(), int(n.size()), 0);
			if (hr != tmp.size())
			{
				printf("送信：失敗\n");
			}
			else
			{
				printf("送信：成功\n");
			}
			break;
		}
	}
	return 0;
}

// 受信
std::string Server::Recv(const unsigned int clientIndex)
{
	memcpy(&fds, &readfds, sizeof(fd_set));
	auto len = select(0, &fds, nullptr, nullptr, &time);
	if (len <= 0)
	{
		return std::string();
	}

	if (clientIndex >= CLIENT_MAX || client[clientIndex] == INVALID_SOCKET)
	{
		return std::string();
	}

	std::string msg;
	if (FD_ISSET(client[clientIndex], &fds))
	{
		msg.resize(DATA_MAX);
		auto hr = recv(client[clientIndex], &msg[0], int(msg.size()), 0);
		if (hr <= 0)
		{
			if (hr == -1)
			{
				char str[INET_ADDRSTRLEN];
				printf("IP：%sが切断しました\n", inet_ntop(clientAddr[clientIndex].sin_family, &clientAddr[clientIndex].sin_addr, str, sizeof(str)));
				closesocket(client[clientIndex]);
				client[clientIndex] = INVALID_SOCKET;
			}
			return std::string();
		}
		msg = msg.substr(0, msg.find_first_of('\0'));
		Send(msg);
	}

	return msg;
}

// 送信
void Server::Send(const char * data, const unsigned int size)
{
	for (unsigned int i = 0; i < client.size(); ++i)
	{
		if (client[i] == INVALID_SOCKET)
		{
			continue;
		}

		auto hr = send(client[i], data, size, 0);
		if (hr != size)
		{
			printf("送信：失敗\n");
		}
		else
		{
			printf("送信：成功\n");
		}
	}
}

// クライアント最大数
unsigned int Server::GetClientMax(void)
{
	return CLIENT_MAX;
}
