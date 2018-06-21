#pragma once
#include <winsock2.h> 
#include <ws2tcpip.h>

// �|�[�g�ԍ�
#define PORT 50001
// �����ڑ���
#define CLIENT_MAX 32
// �ő啶����
#define SIZE_MAX 1024

// �f�[�^
WSADATA data;

// �T�[�o�֌W
SOCKET sock;
struct sockaddr_in addr;
int size;

// �N���C�A���g�֌W
SOCKET c_sock[CLIENT_MAX];
struct sockaddr_in client[CLIENT_MAX];

// ���̑�
int result;
fd_set fds;
fd_set readfds;
struct timeval t_val;
int len;

// 
bool flag;


// ������
int Startup(void);

// �\�P�b�g�̐���
bool CreateSocket(void);

// �A�h���X�Ƃ̊֘A�t��
void Bind(void);

// �N���C�A���g�Ƃ̐ڑ��ҋ@
void Listen(void);

// �N���C�A���g�Ƃ̐ڑ�
void Accept(void);

// ��M
void Recv(bool& flag);

// ���M
void Send(void);

// �N���C�A���g�Ƃ̐ڑ��I��
void Close(void);
