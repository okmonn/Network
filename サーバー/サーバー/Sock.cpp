#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Sock.h"
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

// �R���X�g���N�^
Sock::Sock() : result(0)
{
	data = {};
	sock = INVALID_SOCKET;
	addr = {};
	connection.clear();
	memset(&c_sock, INVALID_SOCKET, sizeof(c_sock));
	SecureZeroMemory(&c_addr, sizeof(c_addr));
	fds = {};
	readfds = {};
	time = { 0, 1000 };
	memset(&r, 0, sizeof(r));
	memset(&s, 0, sizeof(s));


	LoadText("�ڑ����.txt");
	Init();
}

// �f�X�g���N�^
Sock::~Sock()
{
	Close();
	closesocket(sock);
	WSACleanup();
}

// �e�L�X�g�̓ǂݍ���
void Sock::LoadText(std::string fileName, std::string mode)
{
	std::string buf;
	FILE* file;
	try 
	{
		//�t�@�C�����J��
		if (fopen_s(&file, fileName.c_str(), mode.c_str()) != 0)
		{
			throw - 1;
		}

		//�ǂݍ���
		char str;
		while (true)
		{
			int len = fread(&str, sizeof(char), 1, file);
			if (len != 0)
			{
				buf.push_back(str);
			}
			else
			{
				break;
			}
		}
		fclose(file);

		//�����񌟍�
		while (buf.size() > 0)
		{
			auto pos = buf.find_first_of(':');
			auto end = buf.find_first_of('\n');
			if (end >= buf.size())
			{
				end = buf.size();
			}
			else
			{
				end -= 1;
			}
			connection.push_back(buf.substr(pos + 1, end - pos - 1));
			end = buf.find_first_of('\n');
			if (end >= buf.size())
			{
				end = buf.size() - 1;
			}
			buf.erase(buf.begin(), buf.begin() + end + 1);
		}
	}
	catch (int i) 
	{
		if (i != 0)
		{
			printf("�t�@�C�����J���܂���ł���\n");
		}
	}
}

// ������
int Sock::StartUp(void)
{
	try
	{
		result = WSAStartup(MAKEWORD(2, 0), &data);
		if (result != 0)
		{
			throw result;
		}

		printf("�N�����̏������F����\n");
	}
	catch (int i)
	{
		switch (i)
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
		default:
			break;
		}
	}
	
	return result;
}

// �\�P�b�g�̐���
int Sock::CreateSock(void)
{
	//�A�h���X�̐ݒ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(std::atoi(connection[0].c_str()));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	try
	{
		//�\�P�b�g����
		sock = socket(addr.sin_family, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			result = -1;
			throw result;
		}

		result = 0;
	}
	catch (int i)
	{
		if (i != 0)
		{
			printf("�\�P�b�g�����G���[�F%d\n", WSAGetLastError());
		}
	}

	BOOL flag = 1;
	try
	{
		//�\�P�b�g�̐ݒ�̃Z�b�g
		result = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag));
		if (result != 0)
		{
			throw result;
		}

		printf("�\�P�b�g�̐����F����\n");
		printf("�ڑ��|�[�g�ԍ���%s�ł�\n", connection[0].c_str());
	}
	catch (int i)
	{
		if (i != 0)
		{
			printf("�\�P�b�g�ݒ�F%d\n", WSAGetLastError());
		}
	}
	
	return result;
}

// �A�h���X�Ƃ̊֘A�t��
int Sock::Bind(void)
{
	try
	{
		result = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
		if (result != 0)
		{
			throw result;
		}

		printf("�֘A�t���F����\n");
	}
	catch (int i)
	{
		if (i != 0)
		{
			printf("�֘A�t���G���[�F%d\n", WSAGetLastError());
		}
	}

	return result;
}

// �N���C�A���g�Ƃ̐ڑ��ҋ@
int Sock::Listen(void)
{
	try
	{
		result = listen(sock, 5);
		if (result != 0)
		{
			throw result;
		}

		printf("�ڑ������F����\n");
	}
	catch (int i)
	{
		if (i != 0)
		{
			printf("�ڑ��ҋ@�G���[�F%d\n", WSAGetLastError());
		}
	}

	return result;
}

// �N���C�A���g�Ƃ̐ڑ�
void Sock::Accept(void)
{
	FD_ZERO(&readfds);
	for (UINT i = 0; i < CLIENT_MAX; ++i)
	{
		if (c_sock[i] != INVALID_SOCKET)
		{
			continue;
		}
		else
		{
			int size = sizeof(sockaddr_in);
			c_sock[i] = accept(sock, (struct sockaddr *)&c_addr[i], &size);
			if (c_sock[i] != INVALID_SOCKET)
			{
				FD_SET(c_sock[i], &readfds);
				char str[INET_ADDRSTRLEN];
				printf("IP�F%s���ڑ����܂���\n", inet_ntop(c_addr[i].sin_family, &c_addr[i].sin_addr, str, sizeof(str)));
			}
		}
	}
}

// ��M
void Sock::Recv(void)
{
	memcpy(&fds, &readfds, sizeof(fd_set));
	int len = select(0, &fds, NULL, NULL, &time);
	if (len != 0)
	{
		for (UINT i = 0; i < CLIENT_MAX; ++i)
		{
			if (c_sock[i] == INVALID_SOCKET)
			{
				continue;
			}
			else
			{
				if (FD_ISSET(c_sock[i], &fds))
				{
					// �T�[�o����f�[�^����M
					memset(&r, 0, sizeof(r));
					result = recv(c_sock[i], r, sizeof(r), 0);
					if (result == -1)
					{
						char str[INET_ADDRSTRLEN];
						printf("%s�Ƃ̐ڑ����؂�܂���\n", inet_ntop(c_addr[i].sin_family, &c_addr[i].sin_addr, str, sizeof(str)));
						closesocket(c_sock[i]);
						c_sock[i] = INVALID_SOCKET;

					}
					else
					{
						char str[INET_ADDRSTRLEN];
						printf("%s����̎�M�F%s\n", inet_ntop(c_addr[i].sin_family, &c_addr[i].sin_addr, str, sizeof(str)), r);

						//�w��IP�A�h���X�ɑ��M
						std::string dummy;
						for (int i = 0; r[i] != '/' && i < LENGTH_MAX; ++i)
						{
							dummy.push_back(r[i]);
						}
						for (int i = 0; i < CLIENT_MAX; ++i)
						{
							if (inet_addr(dummy.c_str()) == c_addr[i].sin_addr.S_un.S_addr)
							{
								std::string p;
								p = dummy + "�F";
								for (int o = dummy.size() + 1; o < LENGTH_MAX; ++o)
								{
									p.push_back(r[o]);
								}
								if (send(c_sock[i], p.c_str(), sizeof(r), 0) == SOCKET_ERROR)
								{
									char str[INET_ADDRSTRLEN];
									printf("%s�ւ̑��M�F���s\n", inet_ntop(c_addr[i].sin_family, &c_addr[i].sin_addr, str, sizeof(str)));
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
	else
	{
		//printf("�^�C���A�E�g\n");
	}
}

// ���M
void Sock::Send(void)
{
	//���̓o�b�t�@�̃N���A
	fflush(stdin);

	memset(&s, 0, sizeof(s));
	scanf_s("%s", &s, sizeof(s));
	for (UINT i = 0; i < CLIENT_MAX; ++i)
	{
		if (c_sock[i] == INVALID_SOCKET)
		{
			continue;
		}
		else
		{
			if (send(c_sock[i], s, sizeof(s), 0) == SOCKET_ERROR)
			{
				char str[INET_ADDRSTRLEN];
				printf("%s�ւ̑��M�F���s\n", inet_ntop(c_addr[i].sin_family, &c_addr[i].sin_addr, str, sizeof(str)));
				printf("���M�G���[�F%d\n", WSAGetLastError());
			}
			else
			{
				char str[INET_ADDRSTRLEN];
				printf("%s�ւ̑��M�F����\n", inet_ntop(c_addr[i].sin_family, &c_addr[i].sin_addr, str, sizeof(str)));
			}
		}
	}
}

// ��������
void Sock::Init(void)
{
	result = Sock::StartUp();
	if (result != 0)
	{
		return;
	}

	result = CreateSock();
	if (result != 0)
	{
		return;
	}

	result = Sock::Bind();
	if (result != 0)
	{
		return;
	}

	result = Sock::Listen();
	if (result != 0)
	{
		return;
	}
}

// �N���C�A���g�Ƃ̐ڑ��I��
void Sock::Close(void)
{
	for (UINT i = 0; i < CLIENT_MAX; ++i)
	{
		if (c_sock[i] != INVALID_SOCKET)
		{
			closesocket(c_sock[i]);
			c_sock[i] = INVALID_SOCKET;
		}
	}
}

