#pragma once
#include <winsock2.h> 
#include <ws2tcpip.h>
#include <vector>
#include <string>

// �N���C�A���g�ő�ڑ���
#define CLIENT_MAX 10

// �����ő吔
#define LENGTH_MAX 1024

class Sock
{
public:
	// �R���X�g���N�^
	Sock();
	// �f�X�g���N�^
	~Sock();

	// �N���C�A���g�Ƃ̐ڑ�
	void Accept(void);

	// ��M
	void Recv(void);

	// ���M
	void Send(void);

private:
	// �ǂݍ���
	void LoadText(std::string fileName, std::string mode = "rb");

	// ������
	int StartUp(void);

	// �\�P�b�g�̐���
	bool CreateSock(void);

	// �A�h���X�Ƃ̊֘A�t��
	int Bind(void);

	// �N���C�A���g�Ƃ̐ڑ��ҋ@
	int Listen(void);

	// �N���C�A���g�Ƃ̐ڑ��I��
	void Close(void);

	// ��������
	void Init(void);


	// �Q�ƌ���
	int result;

	// �f�[�^
	WSADATA data;

	// �T�[�o�[�\�P�b�g
	SOCKET sock;

	// �A�h���X
	struct sockaddr_in addr;

	// �|�[�g�ԍ�
	std::vector<std::string>connection;

	// �N���C�A���g�\�P�b�g
	SOCKET c_sock[CLIENT_MAX];

	// �N���C�A���g�A�h���X
	struct sockaddr_in c_addr[CLIENT_MAX];

	//
	fd_set fds;

	// 
	fd_set readfds;

	// �^�C���A�E�g
	struct timeval time;

	// ��M����
	char r[LENGTH_MAX];

	// ���M����
	char s[LENGTH_MAX];
};
