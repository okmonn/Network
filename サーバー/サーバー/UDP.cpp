#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "UDP.h"
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

// �����ő吔
#define LENGTH_MAX 1024

// �R���X�g���N�^
UDP::UDP() : result(0)
{
	data = {};
	sock = INVALID_SOCKET;
	addr = {};
	connection.clear();
	r.clear();
	r.resize(LENGTH_MAX);


	LoadText("�ڑ����.txt");
	Init();
}

// �f�X�g���N�^
UDP::~UDP()
{
	closesocket(sock);
	WSACleanup();
}

// ��M
void UDP::Recv(void)
{
	memset(r.data(), 0, sizeof(r));
	recv(sock, r.data(), sizeof(r), 0);
}

// �e�L�X�g�̓ǂݍ���
void UDP::LoadText(std::string fileName, std::string mode)
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
int UDP::StartUp(void)
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

int UDP::CreateSock(void)
{
	//�A�h���X�̐ݒ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(std::atoi(connection[0].c_str()));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	try
	{
		//�\�P�b�g����
		sock = socket(addr.sin_family, SOCK_DGRAM, 0);
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
			printf("UDP�\�P�b�g�����G���[�F%d\n", WSAGetLastError());
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

		printf("UDP�\�P�b�g�̐����F����\n");
		printf("�ڑ��|�[�g�ԍ���%s�ł�\n", connection[0].c_str());
	}
	catch (int i)
	{
		if (i != 0)
		{
			printf("UDP�\�P�b�g�ݒ�F%d\n", WSAGetLastError());
		}
	}

	return result;
}

int UDP::Bind(void)
{
	try
	{
		result = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
		if (result != 0)
		{
			throw result;
		}

		printf("UDP�֘A�t���F����\n");
	}
	catch (int i)
	{
		if (i != 0)
		{
			printf("UDP�֘A�t���G���[�F%d\n", WSAGetLastError());
		}
	}

	return result;
}

void UDP::Init(void)
{
	result = StartUp();
	if (result != 0)
	{
		return;
	}

	result = CreateSock();
	if (result != 0)
	{
		return;
	}

	result = Bind();
	if (result != 0)
	{
		return;
	}
}
