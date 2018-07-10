#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <string>

// 文字最大数
#define LENGTH_MAX 1024

class Sock
{
public:
	// コンストラクタ
	Sock();
	// デストラクタ
	~Sock();

	// 受信
	void Recv(void);

	// 送信
	void Send(void);

	// 終了フラグのセット
	void SetEnd(const bool flag) {
		end = flag;
	}
	// 終了フラグの取得
	constexpr bool GetEnd(void) const {
		return end;
	}

private:
	// テキスト読み込み
	void LoadText(std::string fileName, std::string mode = "rb");

	// 初期化
	int StartUp(void);

	// ソケットの生成
	int CreateSock(void);

	// サーバーとの接続
	int Connect(void);

	// 初期セット
	void Init(void);

	// ソケットを閉じる
	void Close(void);


	// 参照結果
	int result;

	// データ
	WSADATA data;

	// ソケット
	SOCKET sock;

	// アドレス
	struct sockaddr_in addr;

	// サーバーのPC名
	std::vector<std::string>connection;

	// サーバーのアドレスリスト
	std::vector<ULONG>list;

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

	// 終了フラグ
	bool end;
};
