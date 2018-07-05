#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Sock.h"
#include <iostream>
#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")

// �����ő吔
#define LENGTH_MAX 1024

// �R���X�g���N�^
Sock::Sock() : result(0)
{
	data = {};
	sock = INVALID_SOCKET;
	addr = {};
	list.clear();
	fds = {};
	readfds = {};
	time = { 0, 1000 };
	end = false;

	LoadText("�ڑ����.txt");
	Init();
}

// �f�X�g���N�^
Sock::~Sock()
{
	Close();
}

// �e�L�X�g�ǂݍ���
void Sock::LoadText(std::string fileName, std::string mode)
{
	std::string buf;
	FILE* file;
	if (fopen_s(&file, fileName.c_str(), mode.c_str()) != 0)
	{
		printf("�t�@�C�����J���܂���\n");
		return;
	}
	else
	{
		char str;
		while (true)
		{
			int cnt = fread(&str, sizeof(char), 1, file);
			if (cnt != 0)
			{
				buf.push_back(str);
			}
			else
			{
				break;
			}
		}
	}
	fclose(file);

	//PC������
	auto pos = buf.find_first_of(':');
	auto end = buf.find_first_of('\r');
	sever = buf.substr(pos + 1, end - pos - 1);

	//�|�[�g�ԍ�����
	pos = buf.find_last_of(':');
	port = buf.substr(pos + 1, buf.size());
}

// ������
int Sock::StartUp(void)
{
	result = WSAStartup(MAKEWORD(2, 0), &data);
	if (result != 0)
	{
		switch (result)
		{
		case WSASYSNOTREADY:
			printf("�l�b�g���[�N�T�u�V�X�e�����l�b�g���[�N�ւ̐ڑ��������ł��Ă��Ȃ�\n");
			break;
		case WSAVERNOTSUPPORTED:
			printf("�v�����ꂽwinsock�̃o�[�W�������T�|�[�g����Ă��Ȃ�\n");
			break;
		case WSAEINPROGRESS:
			printf("�u���b�L���O����̎��s���ł��邩�A �܂��̓T�[�r�X�v���o�C�_���R�[���o�b�N�֐����������Ă���\n");
			break;
		case WSAEPROCLIM:
			printf("winsock�������ł���ő�v���Z�X���ɒB����\n");
			break;
		case WSAEFAULT:
			printf("�������ł���lpWSAData �͗L���ȃ|�C���^�ł͂Ȃ�\n");
			break;
		}
	}
	else
	{
		printf("�N�����̏������F����\n");
	}

	return result;
}

// �\�P�b�g�̐���
bool Sock::CreateSock(void)
{
	//�g�pPC���O
	char name[256];

	//�z�X�g���
	struct hostent *h;
	if (gethostname(name, 256) == 0)
	{
		printf("���̃R���s���[�^�̖��O��%s�ł�\n", name);

		h = gethostbyname(sever.c_str());
		printf("�z�X�g�T�[�o�[�̖��O��%s�ł�\n", h->h_name);
	}
	else
	{
		printf("�z�X�g���̎擾�Ɏ��s���܂����F%d\n", WSAGetLastError());
	}

	//�A�h���X���
	in_addr i_addr;
	for (int i = 0; h->h_addr_list[i]; ++i)
	{
		memcpy(&i_addr, h->h_addr_list[i], 4);
		list.push_back(i_addr.S_un.S_addr);
	}

	//�\�P�b�g�̐ݒ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(std::atoi(port.c_str()));
	addr.sin_addr.S_un.S_addr = i_addr.S_un.S_addr;

	//�\�P�b�g�̐���
	sock = socket(addr.sin_family, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		printf("�\�P�b�g�����F���s : %d\n", WSAGetLastError());
		return false;
	}
	else
	{
		printf("�\�P�b�g�����F����\n");
		printf("�ڑ��|�[�g�ԍ���%s�ł�\n", port.c_str());
	}

	return true;
}

// �T�[�o�[�Ƃ̐ڑ�
int Sock::Connect(void)
{
	FD_ZERO(&readfds);
	for (UINT i = 0; i < list.size(); ++i)
	{
		addr.sin_addr.S_un.S_addr = list[i];
		result = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
		if (result == 0)
		{
			char str[INET_ADDRSTRLEN];
			printf("%s�ɐڑ����܂���\n", inet_ntop(addr.sin_family, &addr.sin_addr, str, sizeof(str)));
			FD_SET(sock, &readfds);
			break;
		}
		else
		{
			printf("�T�[�o�[�ڑ��F���s : %d\n", WSAGetLastError());
			continue;
		}
	}

	return result;
}

// �����Z�b�g
void Sock::Init(void)
{
	result = StartUp();
	if (result != 0)
	{
		return;
	}

	if (CreateSock() != true)
	{
		return;
	}

	result = Connect();
	if (result != 0)
	{
		return;
	}
}

// ��M
void Sock::Recv(void)
{
	memcpy(&fds, &readfds, sizeof(fd_set));
	int len = select(0, &fds, NULL, NULL, &time);
	if (len != 0)
	{
		if (FD_ISSET(sock, &fds))
		{
			char buf[LENGTH_MAX];
			memset(buf, 0, sizeof(buf));
			result = recv(sock, buf, sizeof(buf), 0);
			if (result == -1)
			{
				printf("�T�[�o�[�������オ���Ă��܂���\n");
				end = true;
			}
			else
			{
				printf("��M�F%s\n", buf);
			}
		}
	}
}

// ���M
void Sock::Send(void)
{
	fflush(stdin);
	char buf[LENGTH_MAX];
	memset(buf, 0, sizeof(buf));
	scanf_s("%s", &buf, sizeof(buf));
	if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		printf("���M�F���s�F%d\n", WSAGetLastError());
	}
	else
	{
		printf("���M�F%s\n", &buf);
	}
}

// �\�P�b�g�����
void Sock::Close(void)
{
	if (sock != INVALID_SOCKET)
	{
		closesocket(sock);
		sock = INVALID_SOCKET;
	}
}