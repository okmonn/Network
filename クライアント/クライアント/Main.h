#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>

// ポート番号
#define PORT 50001
// 最大文字数
#define SIZE_MAX 1024

// データ
WSADATA wsaData;

// ソケット
SOCKET sock;

// アドレス
struct sockaddr_in addr;

// 
fd_set fds;

// 
fd_set readfds;

// 
struct timeval t_val;

// 
int len;

// 結果
int result;

// アドレス情報
std::vector<ULONG>no;

// 
bool flag;


// 初期化
int Startup(void);

// ソケットの生成
bool CreateSocket(void);

// サーバに接続
int Connect(void);

// 受信
void Recv(bool& flag);

// 送信
void Send(void);
