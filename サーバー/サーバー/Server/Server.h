#pragma once
#include <string>
#include <vector>
#include <winsock2.h> 

class Server
{
public:
	// �f�X�g���N�^
	~Server();

	// �C���X�^���X�ϐ��̎擾
	static Server& Get(void) {
		static Server instance;
		return instance;
	}

	// �N���C�A���g�Ƃ̐ڑ�
	void Accept(void);

	// ��M
	std::string Recv(const unsigned int clientIndex = 0);

	// ���M
	void Send(const char* data, const unsigned int size);

	// �N���C�A���g�ő吔
	unsigned int GetClientMax(void);

private:
	// �R���X�g���N�^
	Server();
	Server(const Server&) = delete;
	void operator=(const Server&) = delete;

	// �����X�^�[�g�A�b�v
	int StartUp(void);

	// �\�P�b�g�̐���
	int CreateSock(void);

	// �Z�b�g�A�b�v
	int SetUp(void);

	// �A�h���X�Ƃ̊֘A�t��
	int Bind(void);

	// �ڑ��ҋ@
	int Listen(void);

	// ������
	void Init(void);

	// ���M
	int Send(const std::string& data);


	// �f�[�^
	WSADATA data;

	// �\�P�b�g
	SOCKET sock;

	// �A�h���X
	struct sockaddr_in addr;

	// �Z���N�g�Ŏg�p
	fd_set fds;

	// �Z���N�g�Ŏg�p
	fd_set readfds;

	// �N���C�A���g
	std::vector<SOCKET>client;

	// �N���C�A���g�A�h���X
	std::vector<struct sockaddr_in> clientAddr;

	// �^�C���A�E�g
	struct timeval time;
};
