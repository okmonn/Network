#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Main.h"
#include <iostream>
#include <thread>

#pragma comment(lib,"ws2_32.lib")

// 初期化
int Startup(void)
{
	result = WSAStartup(MAKEWORD(2, 0), &data);
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
	// ソケットの設定
	{
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
		addr.sin_addr.S_un.S_addr = INADDR_ANY;
	}

	// ソケットの生成
	{
		sock = socket(addr.sin_family, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			printf("ソケット生成エラー：%d\n", WSAGetLastError());
			return false;
		}
	}

	// ポート番号の再利用
	{
		BOOL flag = 1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag));
	}

	return true;
}

// アドレスとの関連付け
void Bind(void)
{
	bind(sock, (struct sockaddr *)&addr, sizeof(addr));
}

// クライアントとの接続待機
void Listen(void)
{
	listen(sock, 5);
}

// クライアントとの接続
void Accept(void)
{
	size = sizeof(client);
	FD_ZERO(&readfds);
	while (true)
	{
		for (int i = 0; i < CLIENT_MAX; ++i)
		{
			if (c_sock[i] == INVALID_SOCKET)
			{
				c_sock[i] = accept(sock, (struct sockaddr *)&client[i], &size);
				if (c_sock[i] != INVALID_SOCKET)
				{
					FD_SET(c_sock[i], &readfds);
					char str[INET_ADDRSTRLEN];
					printf("IP：%sが接続しました\n", inet_ntop(client[i].sin_family, &client[i].sin_addr, str, sizeof(str)));
				}
			}
		}
	}
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
			for (UINT i = 0; i < CLIENT_MAX; ++i)
			{
				if (c_sock[i] == INVALID_SOCKET)
				{
					continue;
				}

				if (FD_ISSET(c_sock[i], &fds))
				{
					// 受信
					{
						// サーバからデータを受信
						char buf[SIZE_MAX];
						memset(buf, 0, sizeof(buf));
						result = recv(c_sock[i], buf, sizeof(buf), 0);
						if (result == -1)
						{
							char str[INET_ADDRSTRLEN];
							printf("%sとの接続が切れました\n", inet_ntop(client[i].sin_family, &client[i].sin_addr, str, sizeof(str)));
							closesocket(c_sock[i]);
							c_sock[i] = INVALID_SOCKET;

						}
						else
						{
							char str[INET_ADDRSTRLEN];
							printf("%sからの受信：%s\n", inet_ntop(client[i].sin_family, &client[i].sin_addr, str, sizeof(str)),  buf);

							std::string m;

							for (int i = 0; buf[i] !='/' && i < SIZE_MAX; ++i)
							{
								m.push_back(buf[i]);
							}
							for (int i = 0; i < CLIENT_MAX; ++i)
							{
								if (inet_addr(m.c_str()) == client[i].sin_addr.S_un.S_addr)
								{
									std::string p;
									p = m + "：";
									for (int o = m.size() + 1; o < SIZE_MAX; ++o)
									{
										p.push_back(buf[o]);
									}
									if (send(c_sock[i], p.c_str(), sizeof(buf), 0) == SOCKET_ERROR)
									{
										char str[INET_ADDRSTRLEN];
										printf("%sへの送信：失敗\n", inet_ntop(client[i].sin_family, &client[i].sin_addr, str, sizeof(str)));
										printf("送信エラー：%d\n", WSAGetLastError());
									}
									
									break;
								}
							}
						}
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
		for (UINT i = 0; i < CLIENT_MAX; ++i)
		{
			if (c_sock[i] == INVALID_SOCKET)
			{
				continue;
			}

			if (send(c_sock[i], moji, sizeof(moji), 0) == SOCKET_ERROR)
			{
				char str[INET_ADDRSTRLEN];
				printf("%sへの送信：失敗\n", inet_ntop(client[i].sin_family, &client[i].sin_addr, str, sizeof(str)));
				printf("送信エラー：%d\n", WSAGetLastError());
			}
		}
	}

	fflush(stdin);
}

// クライアントとの接続終了
void Close(void)
{
	for (UINT i = 0; i < CLIENT_MAX; ++i)
	{
		if (c_sock[i] != INVALID_SOCKET)
		{
			closesocket(c_sock[i]);
		}
	}
}

// メイン関数
int main()
{
	// サーバ関係
	sock = INVALID_SOCKET;
	addr = {};
	size = 0;

	// クライアント関係
	memset(c_sock, INVALID_SOCKET, sizeof(c_sock));
	memset(client, 0, sizeof(client));

	// その他
	result = 0;
	t_val = { 0, 1000 };
	len = 0;
	flag = false;


	Startup();
	CreateSocket();
	Bind();
	Listen();


	std::thread a(Accept);
	std::thread r(Recv, std::ref(flag));
	std::thread s(Send);


	// 
	a.detach();
	r.join();
	s.detach();

	Close();
	// 終了処理
	WSACleanup();

	return 0;
}
