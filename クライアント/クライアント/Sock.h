#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <string>

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
	bool CreateSock(void);

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

	// ポート番号
	std::string port;

	// サーバーのPC名
	std::string sever;

	// サーバーのアドレスリスト
	std::vector<ULONG>list;

	// 
	fd_set fds;

	// 
	fd_set readfds;

	// タイムアウト
	struct timeval time;

	// 終了フラグ
	bool end;
};
