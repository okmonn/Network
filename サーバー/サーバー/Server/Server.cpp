#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Server.h"
#include <ws2tcpip.h>
#include <tchar.h>
#include <algorithm>

#pragma comment(lib,"ws2_32.lib")

// �N���C�A���g��
#define CLIENT_MAX 10

// �f�[�^�ő吔
#define DATA_MAX 256

// �R���X�g���N�^
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

// �f�X�g���N�^
Server::~Server()
{
	for (auto& i : client)
	{
		closesocket(i);
	}
	closesocket(sock);
	WSACleanup();
}

// �����X�^�[�g�A�b�v
int Server::StartUp(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\n�����X�^�[�g�A�b�v�F���s\n"));
	}

	return hr;
}

// �\�P�b�g�̐���
int Server::CreateSock(void)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		OutputDebugString(_T("\n�\�P�b�g�̐����F���s\n"));
		return -1;
	}

	return 0;
}

// �Z�b�g�A�b�v
int Server::SetUp(void)
{
	char port[5];
	{
		printf("�|�[�g�ԍ������\n");
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
		OutputDebugString(_T("\n�\�P�b�g�̃Z�b�g�F���s\n"));
	}

	return hr;
}

// �A�h���X�Ƃ̊֘A�t��
int Server::Bind(void)
{
	auto hr = bind(sock, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
	if (hr != 0)
	{
		OutputDebugString(_T("\n�A�h���X�Ƃ̊֘A�t���F���s\n"));
	}
	else
	{
		printf("�T�[�o�[�����グ�F����\n");
	}

	return hr;
}

// �ڑ��ҋ@
int Server::Listen(void)
{
	auto hr = listen(sock, 5);
	if (hr != 0)
	{
		OutputDebugString(_T("\n�ڑ��ҋ@�F���s\n"));
	}

	return hr;
}

// ������
void Server::Init(void)
{
	StartUp();
	CreateSock();
	SetUp();
	Bind();
	Listen();
}

// �N���C�A���g�Ƃ̐ڑ�
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
			printf("IP�F%s���ڑ�\n", inet_ntop(clientAddr[i].sin_family, &clientAddr[i].sin_addr, str, sizeof(str)));
		}
	}
}

// ���M
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
			auto n = ip + "�F" + tmp;
			auto hr = send(client[i], n.c_str(), int(n.size()), 0);
			if (hr != tmp.size())
			{
				printf("���M�F���s\n");
			}
			else
			{
				printf("���M�F����\n");
			}
			break;
		}
	}
	return 0;
}

// ��M
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
				printf("IP�F%s���ؒf���܂���\n", inet_ntop(clientAddr[clientIndex].sin_family, &clientAddr[clientIndex].sin_addr, str, sizeof(str)));
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

// ���M
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
			printf("���M�F���s\n");
		}
		else
		{
			printf("���M�F����\n");
		}
	}
}

// �N���C�A���g�ő吔
unsigned int Server::GetClientMax(void)
{
	return CLIENT_MAX;
}
