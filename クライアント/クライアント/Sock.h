#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <string>

// �����ő吔
#define LENGTH_MAX 1024

class Sock
{
public:
	// �R���X�g���N�^
	Sock();
	// �f�X�g���N�^
	~Sock();

	// ��M
	void Recv(void);

	// ���M
	void Send(void);

	// �I���t���O�̃Z�b�g
	void SetEnd(const bool flag) {
		end = flag;
	}
	// �I���t���O�̎擾
	constexpr bool GetEnd(void) const {
		return end;
	}

private:
	// �e�L�X�g�ǂݍ���
	void LoadText(std::string fileName, std::string mode = "rb");

	// ������
	int StartUp(void);

	// �\�P�b�g�̐���
	int CreateSock(void);

	// �T�[�o�[�Ƃ̐ڑ�
	int Connect(void);

	// �����Z�b�g
	void Init(void);

	// �\�P�b�g�����
	void Close(void);


	// �Q�ƌ���
	int result;

	// �f�[�^
	WSADATA data;

	// �\�P�b�g
	SOCKET sock;

	// �A�h���X
	struct sockaddr_in addr;

	// �T�[�o�[��PC��
	std::vector<std::string>connection;

	// �T�[�o�[�̃A�h���X���X�g
	std::vector<ULONG>list;

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

	// �I���t���O
	bool end;
};
