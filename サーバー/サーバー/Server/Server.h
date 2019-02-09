#pragma once
#include <string>
#include <vector>
#include <winsock2.h> 

class Server
{
public:
	// デストラクタ
	~Server();

	// インスタンス変数の取得
	static Server& Get(void) {
		static Server instance;
		return instance;
	}

	// クライアントとの接続
	void Accept(void);

	// 受信
	std::string Recv(const unsigned int clientIndex = 0);

	// 送信
	void Send(const char* data, const unsigned int size);

	// クライアント最大数
	unsigned int GetClientMax(void);

private:
	// コンストラクタ
	Server();
	Server(const Server&) = delete;
	void operator=(const Server&) = delete;

	// 初期スタートアップ
	int StartUp(void);

	// ソケットの生成
	int CreateSock(void);

	// セットアップ
	int SetUp(void);

	// アドレスとの関連付け
	int Bind(void);

	// 接続待機
	int Listen(void);

	// 初期化
	void Init(void);

	// 送信
	int Send(const std::string& data);


	// データ
	WSADATA data;

	// ソケット
	SOCKET sock;

	// アドレス
	struct sockaddr_in addr;

	// セレクトで使用
	fd_set fds;

	// セレクトで使用
	fd_set readfds;

	// クライアント
	std::vector<SOCKET>client;

	// クライアントアドレス
	std::vector<struct sockaddr_in> clientAddr;

	// タイムアウト
	struct timeval time;
};
