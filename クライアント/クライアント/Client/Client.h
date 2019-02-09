#pragma once
#include <WinSock2.h>
#include <vector>

class Client
{
public:
	// デストラクタ
	~Client();

	// インスタンス変数の取得
	static Client& Get(void) {
		static Client instance;
		return instance;
	}

	// 受信
	std::string Recv(void);

	// 送信
	void Send(const char* data, const unsigned int& size);

private:
	// コンストラクタ
	Client();
	Client(const Client&) = delete;
	void operator=(const Client&) = delete;

	// 初期スタートアップ
	int StartUp(void);

	// ソケットの生成
	int CreateSock(void);

	// 接続
	int Connect(void);

	// セットアップ
	int SetUp(void);

	// 初期化
	void Init(void);


	// データ
	WSADATA data;

	// ソケット
	SOCKET sock;

	// 接続先アドレス
	sockaddr_in addr;

	// セレクトで使用
	fd_set fds;

	// セレクトで使用
	fd_set readfds;

	// タイムアウト
	timeval time;
};