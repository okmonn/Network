#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>

// �|�[�g�ԍ�
#define PORT 50001
// �ő啶����
#define SIZE_MAX 1024

// �f�[�^
WSADATA wsaData;

// �\�P�b�g
SOCKET sock;

// �A�h���X
struct sockaddr_in addr;

// 
fd_set fds;

// 
fd_set readfds;

// 
struct timeval t_val;

// 
int len;

// ����
int result;

// �A�h���X���
std::vector<ULONG>no;

// 
bool flag;


// ������
int Startup(void);

// �\�P�b�g�̐���
bool CreateSocket(void);

// �T�[�o�ɐڑ�
int Connect(void);

// ��M
void Recv(bool& flag);

// ���M
void Send(void);
