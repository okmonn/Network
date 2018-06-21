#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Main.h"
#include <iostream>
#include <tchar.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

const char* server = "PC64311";

// 初期化
int Startup(void)
{
	result = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (result != 0)
	{
		switch (result)
		{
		case WSASYSNOTREADY:
			printf("ネットワークサブシステムがネットワークへの接続を準備できていない\n");
			break;
		case WSAVERNOTSUPPORTED:
			printf("要求されたwinsockのバージョンがサポートされていない\n");
			break;
		case WSAEINPROGRESS:
			printf("ブロッキング操作の実行中であるか、 またはサービスプロバイダがコールバック関数を処理している\n");
			break;
		case WSAEPROCLIM:
			printf("winsockが処理できる最大プロセス数に達した\n");
			break;
		case WSAEFAULT:
			printf("第二引数であるlpWSAData は有効なポインタではない\n");
			break;
		}
	}

	return result;
}

// ソケットの生成
bool CreateSocket(void)
{
	// 使用PC名前
	char name[256];

	// ホスト情報
	struct hostent *h;
	if (gethostname(name, 256) == 0)
	{
		printf("このコンピュータの名前は%sです\n", name);

		h = gethostbyname(server);
		printf("ホストの名前は%sです\n", h->h_name);
	}
	else
	{
		printf("ホスト名の取得に失敗しました。\n");
		printf("エラー%dが発生しました\n", WSAGetLastError());
	}

	// アドレス情報
	in_addr i_addr;
	for (int i = 0; h->h_addr_list[i]; ++i)
	{
		memcpy(&i_addr, h->h_addr_list[i], 4);
		no.push_back(i_addr.S_un.S_addr);
	}


	// ソケットの設定
	{
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
		addr.sin_addr.S_un.S_addr = i_addr.S_un.S_addr;
	}

	// ソケットの生成
	{
		sock = socket(addr.sin_family, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			// エラーの表示
			printf("error : %d\n", WSAGetLastError());
			return false;
		}
	}

	return true;
}

// サーバに接続
int Connect(void)
{
	// サーバに接続
	{
		for (unsigned int i = 0; i < no.size(); ++i)
		{
			addr.sin_addr.S_un.S_addr = no[i];
			result = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
			if (result == 0)
			{
				char str[INET_ADDRSTRLEN];
				printf("%sに接続しました\n", inet_ntop(addr.sin_family, &addr.sin_addr, str, sizeof(str)));
				break;
			}
		}
	}

	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);

	return result;
}

// 受信
void Recv(bool& flag)
{
	while (flag == false)
	{
		memcpy(&fds, &readfds, sizeof(fd_set));
		len = select(0, &fds, NULL, NULL, &t_val);

		if (len != 0)
		{
			if (FD_ISSET(sock, &fds))
			{
				// 受信
				{
					// サーバからデータを受信
					char buf[SIZE_MAX];
					memset(buf, 0, sizeof(buf));
					result = recv(sock, buf, sizeof(buf), 0);
					if (result == -1)
					{
						flag = true;
					}
					else
					{
						printf("受信：%s\n", buf);
					}
				}
			}
		}
	}
}

// 送信
void Send(void)
{
	while (true)
	{

		fflush(stdin);

		// 送信データ
		char moji[SIZE_MAX];
		memset(moji, 0, sizeof(moji));
		scanf_s("%s", &moji, sizeof(moji));
		if (sendto(sock, moji, strlen(moji), 0, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			printf("送信：失敗\n");
			printf("send : %d\n", WSAGetLastError());
		}
		else
		{
			printf("送信：成功_%s\n", &moji);
		}
	}

	fflush(stdin);
}

// メイン関数
int main()
{
	// ソケット
	sock = INVALID_SOCKET;

	// アドレス
	addr = {};

	// 
	t_val = { 0, 1000 };

	// 
	len = 0;

	// 参照結果
	result = 0;

	// 
	flag = false;


	Startup();
	CreateSocket();
	Connect();

	std::thread r(Recv, std::ref(flag));
	std::thread s(Send);

	// 
	r.join();
	s.detach();

	closesocket(sock);

	// winsock2の終了処理
	WSACleanup();

	return 0;
}