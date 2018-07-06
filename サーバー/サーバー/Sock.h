#pragma once
#include <winsock2.h> 
#include <ws2tcpip.h>
#include <vector>
#include <string>

// クライアント最大接続数
#define CLIENT_MAX 10

// 文字最大数
#define LENGTH_MAX 1024

class Sock
{
public:
	// コンストラクタ
	Sock();
	// デストラクタ
	~Sock();

	// クライアントとの接続
	void Accept(void);

	// 受信
	void Recv(void);

	// 送信
	void Send(void);

private:
	// 読み込み
	void LoadText(std::string fileName, std::string mode = "rb");

	// 初期化
	int StartUp(void);

	// ソケットの生成
	bool CreateSock(void);

	// アドレスとの関連付け
	int Bind(void);

	// クライアントとの接続待機
	int Listen(void);

	// クライアントとの接続終了
	void Close(void);

	// 初期処理
	void Init(void);


	// 参照結果
	int result;

	// データ
	WSADATA data;

	// サーバーソケット
	SOCKET sock;

	// アドレス
	struct sockaddr_in addr;

	// ポート番号
	std::vector<std::string>connection;

	// クライアントソケット
	SOCKET c_sock[CLIENT_MAX];

	// クライアントアドレス
	struct sockaddr_in c_addr[CLIENT_MAX];

	//
	fd_set fds;

	// 
	fd_set readfds;

	// タイムアウト
	struct timeval time;

	// 受信文字
	char r[LENGTH_MAX];

	// 送信文字
	char s[LENGTH_MAX];
};
