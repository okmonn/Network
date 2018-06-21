#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Main.h"
#include <iostream>
#include <thread>

#pragma comment(lib,"ws2_32.lib")

// ������
int Startup(void)
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

	return result;
}

// �\�P�b�g�̐���
bool CreateSocket(void)
{
	// �\�P�b�g�̐ݒ�
	{
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
		addr.sin_addr.S_un.S_addr = INADDR_ANY;
	}

	// �\�P�b�g�̐���
	{
		sock = socket(addr.sin_family, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			printf("�\�P�b�g�����G���[�F%d\n", WSAGetLastError());
			return false;
		}
	}

	// �|�[�g�ԍ��̍ė��p
	{
		BOOL flag = 1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag));
	}

	return true;
}

// �A�h���X�Ƃ̊֘A�t��
void Bind(void)
{
	bind(sock, (struct sockaddr *)&addr, sizeof(addr));
}

// �N���C�A���g�Ƃ̐ڑ��ҋ@
void Listen(void)
{
	listen(sock, 5);
}

// �N���C�A���g�Ƃ̐ڑ�
void Accept(void)
{
	size = sizeof(client);
	FD_ZERO(&readfds);
	while (true)
	{
		for (int i = 0; i < CLIENT_MAX; ++i)
		{
			if (c_sock[i] == INVALID_SOCKET)
			{
				c_sock[i] = accept(sock, (struct sockaddr *)&client[i], &size);
				if (c_sock[i] != INVALID_SOCKET)
				{
					FD_SET(c_sock[i], &readfds);
					char str[INET_ADDRSTRLEN];
					printf("IP�F%s���ڑ����܂���\n", inet_ntop(client[i].sin_family, &client[i].sin_addr, str, sizeof(str)));
				}
			}
		}
	}
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
			for (UINT i = 0; i < CLIENT_MAX; ++i)
			{
				if (c_sock[i] == INVALID_SOCKET)
				{
					continue;
				}

				if (FD_ISSET(c_sock[i], &fds))
				{
					// ��M
					{
						// �T�[�o����f�[�^����M
						char buf[SIZE_MAX];
						memset(buf, 0, sizeof(buf));
						result = recv(c_sock[i], buf, sizeof(buf), 0);
						if (result == -1)
						{
							char str[INET_ADDRSTRLEN];
							printf("%s�Ƃ̐ڑ����؂�܂���\n", inet_ntop(client[i].sin_family, &client[i].sin_addr, str, sizeof(str)));
							closesocket(c_sock[i]);
							c_sock[i] = INVALID_SOCKET;

						}
						else
						{
							char str[INET_ADDRSTRLEN];
							printf("%s����̎�M�F%s\n", inet_ntop(client[i].sin_family, &client[i].sin_addr, str, sizeof(str)),  buf);

							std::string m;

							for (int i = 0; buf[i] !='/' && i < SIZE_MAX; ++i)
							{
								m.push_back(buf[i]);
							}
							for (int i = 0; i < CLIENT_MAX; ++i)
							{
								if (inet_addr(m.c_str()) == client[i].sin_addr.S_un.S_addr)
								{
									std::string p;
									p = m + "�F";
									for (int o = m.size() + 1; o < SIZE_MAX; ++o)
									{
										p.push_back(buf[o]);
									}
									if (send(c_sock[i], p.c_str(), sizeof(buf), 0) == SOCKET_ERROR)
									{
										char str[INET_ADDRSTRLEN];
										printf("%s�ւ̑��M�F���s\n", inet_ntop(client[i].sin_family, &client[i].sin_addr, str, sizeof(str)));
										printf("���M�G���[�F%d\n", WSAGetLastError());
									}
									
									break;
								}
							}
						}
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
		for (UINT i = 0; i < CLIENT_MAX; ++i)
		{
			if (c_sock[i] == INVALID_SOCKET)
			{
				continue;
			}

			if (send(c_sock[i], moji, sizeof(moji), 0) == SOCKET_ERROR)
			{
				char str[INET_ADDRSTRLEN];
				printf("%s�ւ̑��M�F���s\n", inet_ntop(client[i].sin_family, &client[i].sin_addr, str, sizeof(str)));
				printf("���M�G���[�F%d\n", WSAGetLastError());
			}
		}
	}

	fflush(stdin);
}

// �N���C�A���g�Ƃ̐ڑ��I��
void Close(void)
{
	for (UINT i = 0; i < CLIENT_MAX; ++i)
	{
		if (c_sock[i] != INVALID_SOCKET)
		{
			closesocket(c_sock[i]);
		}
	}
}

// ���C���֐�
int main()
{
	// �T�[�o�֌W
	sock = INVALID_SOCKET;
	addr = {};
	size = 0;

	// �N���C�A���g�֌W
	memset(c_sock, INVALID_SOCKET, sizeof(c_sock));
	memset(client, 0, sizeof(client));

	// ���̑�
	result = 0;
	t_val = { 0, 1000 };
	len = 0;
	flag = false;


	Startup();
	CreateSocket();
	Bind();
	Listen();


	std::thread a(Accept);
	std::thread r(Recv, std::ref(flag));
	std::thread s(Send);


	// 
	a.detach();
	r.join();
	s.detach();

	Close();
	// �I������
	WSACleanup();

	return 0;
}
