#pragma once
#include <winsock2.h> 
#include <ws2tcpip.h>
#include <vector>
#include <string>

// 文字最大数
#define LENGTH_MAX 1024

class UDP
{
public:
	// コンストラクタ
	UDP();
	// デストラクタ
	~UDP();

	// 受信
	void Recv(void);

private:
	// テキストの読み込み
	void LoadText(std::string fileName, std::string mode = "rb");

	// 初期化
	int StartUp(void);

	// ソケットの生成
	int CreateSock(void);

	// アドレスとの関連付け
	int Bind(void);

	// 初期処理
	void Init(void);


	// 参照結果
	int result;

	// データ
	WSADATA data;

	// UDPソケット
	SOCKET sock;

	// UDPアドレス
	struct sockaddr_in addr;

	// ポート番号
	std::vector<std::string>connection;

	// 受信文字列
	std::vector<char>r;
};
