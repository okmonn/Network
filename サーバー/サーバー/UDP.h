#pragma once
#include <winsock2.h> 
#include <ws2tcpip.h>
#include <vector>
#include <string>

// �����ő吔
#define LENGTH_MAX 1024

class UDP
{
public:
	// �R���X�g���N�^
	UDP();
	// �f�X�g���N�^
	~UDP();

	// ��M
	void Recv(void);

private:
	// �e�L�X�g�̓ǂݍ���
	void LoadText(std::string fileName, std::string mode = "rb");

	// ������
	int StartUp(void);

	// �\�P�b�g�̐���
	int CreateSock(void);

	// �A�h���X�Ƃ̊֘A�t��
	int Bind(void);

	// ��������
	void Init(void);


	// �Q�ƌ���
	int result;

	// �f�[�^
	WSADATA data;

	// UDP�\�P�b�g
	SOCKET sock;

	// UDP�A�h���X
	struct sockaddr_in addr;

	// �|�[�g�ԍ�
	std::vector<std::string>connection;

	// ��M������
	std::vector<char>r;
};
