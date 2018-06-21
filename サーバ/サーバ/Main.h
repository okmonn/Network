#pragma once
#include <winsock2.h> 
#include <ws2tcpip.h>

// ポート番号
#define PORT 50001
// 同時接続数
#define CLIENT_MAX 32
// 最大文字数
#define SIZE_MAX 1024

// データ
WSADATA data;

// サーバ関係
SOCKET sock;
struct sockaddr_in addr;
int size;

// クライアント関係
SOCKET c_sock[CLIENT_MAX];
struct sockaddr_in client[CLIENT_MAX];

// その他
int result;
fd_set fds;
fd_set readfds;
struct timeval t_val;
int len;

// 
bool flag;


// 初期化
int Startup(void);

// ソケットの生成
bool CreateSocket(void);

// アドレスとの関連付け
void Bind(void);

// クライアントとの接続待機
void Listen(void);

// クライアントとの接続
void Accept(void);

// 受信
void Recv(bool& flag);

// 送信
void Send(void);

// クライアントとの接続終了
void Close(void);
