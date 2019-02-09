#pragma once
#include <WinSock2.h>
#include <vector>

class Client
{
public:
	// �f�X�g���N�^
	~Client();

	// �C���X�^���X�ϐ��̎擾
	static Client& Get(void) {
		static Client instance;
		return instance;
	}

	// ��M
	std::string Recv(void);

	// ���M
	void Send(const char* data, const unsigned int& size);

private:
	// �R���X�g���N�^
	Client();
	Client(const Client&) = delete;
	void operator=(const Client&) = delete;

	// �����X�^�[�g�A�b�v
	int StartUp(void);

	// �\�P�b�g�̐���
	int CreateSock(void);

	// �ڑ�
	int Connect(void);

	// �Z�b�g�A�b�v
	int SetUp(void);

	// ������
	void Init(void);


	// �f�[�^
	WSADATA data;

	// �\�P�b�g
	SOCKET sock;

	// �ڑ���A�h���X
	sockaddr_in addr;

	// �Z���N�g�Ŏg�p
	fd_set fds;

	// �Z���N�g�Ŏg�p
	fd_set readfds;

	// �^�C���A�E�g
	timeval time;
};