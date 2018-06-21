#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Main.h"
#include <iostream>
#include <tchar.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

const char* server = "PC64311";

// ������
int Startup(void)
{
	result = WSAStartup(MAKEWORD(2, 0), &wsaData);
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

	return result;
}

// �\�P�b�g�̐���
bool CreateSocket(void)
{
	// �g�pPC���O
	char name[256];

	// �z�X�g���
	struct hostent *h;
	if (gethostname(name, 256) == 0)
	{
		printf("���̃R���s���[�^�̖��O��%s�ł�\n", name);

		h = gethostbyname(server);
		printf("�z�X�g�̖��O��%s�ł�\n", h->h_name);
	}
	else
	{
		printf("�z�X�g���̎擾�Ɏ��s���܂����B\n");
		printf("�G���[%d���������܂���\n", WSAGetLastError());
	}

	// �A�h���X���
	in_addr i_addr;
	for (int i = 0; h->h_addr_list[i]; ++i)
	{
		memcpy(&i_addr, h->h_addr_list[i], 4);
		no.push_back(i_addr.S_un.S_addr);
	}


	// �\�P�b�g�̐ݒ�
	{
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
		addr.sin_addr.S_un.S_addr = i_addr.S_un.S_addr;
	}

	// �\�P�b�g�̐���
	{
		sock = socket(addr.sin_family, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			// �G���[�̕\��
			printf("error : %d\n", WSAGetLastError());
			return false;
		}
	}

	return true;
}

// �T�[�o�ɐڑ�
int Connect(void)
{
	// �T�[�o�ɐڑ�
	{
		for (unsigned int i = 0; i < no.size(); ++i)
		{
			addr.sin_addr.S_un.S_addr = no[i];
			result = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
			if (result == 0)
			{
				char str[INET_ADDRSTRLEN];
				printf("%s�ɐڑ����܂���\n", inet_ntop(addr.sin_family, &addr.sin_addr, str, sizeof(str)));
				break;
			}
		}
	}

	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);

	return result;
}

// ��M
void Recv(bool& flag)
{
	while (flag == false)
	{
		memcpy(&fds, &readfds, sizeof(fd_set));
		len = select(0, &fds, NULL, NULL, &t_val);

		if (len != 0)
		{
			if (FD_ISSET(sock, &fds))
			{
				// ��M
				{
					// �T�[�o����f�[�^����M
					char buf[SIZE_MAX];
					memset(buf, 0, sizeof(buf));
					result = recv(sock, buf, sizeof(buf), 0);
					if (result == -1)
					{
						flag = true;
					}
					else
					{
						printf("��M�F%s\n", buf);
					}
				}
			}
		}
	}
}

// ���M
void Send(void)
{
	while (true)
	{

		fflush(stdin);

		// ���M�f�[�^
		char moji[SIZE_MAX];
		memset(moji, 0, sizeof(moji));
		scanf_s("%s", &moji, sizeof(moji));
		if (sendto(sock, moji, strlen(moji), 0, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			printf("���M�F���s\n");
			printf("send : %d\n", WSAGetLastError());
		}
		else
		{
			printf("���M�F����_%s\n", &moji);
		}
	}

	fflush(stdin);
}

// ���C���֐�
int main()
{
	// �\�P�b�g
	sock = INVALID_SOCKET;

	// �A�h���X
	addr = {};

	// 
	t_val = { 0, 1000 };

	// 
	len = 0;

	// �Q�ƌ���
	result = 0;

	// 
	flag = false;


	Startup();
	CreateSocket();
	Connect();

	std::thread r(Recv, std::ref(flag));
	std::thread s(Send);

	// 
	r.join();
	s.detach();

	closesocket(sock);

	// winsock2�̏I������
	WSACleanup();

	return 0;
}