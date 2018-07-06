#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Sock.h"
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

// コンストラクタ
Sock::Sock() : result(0)
{
	data = {};
	sock = INVALID_SOCKET;
	addr = {};
	connection.clear();
	memset(&c_sock, INVALID_SOCKET, sizeof(c_sock));
	SecureZeroMemory(&c_addr, sizeof(c_addr));
	fds = {};
	readfds = {};
	time = { 0, 1000 };
	memset(&r, 0, sizeof(r));
	memset(&s, 0, sizeof(s));


	LoadText("接続情報.txt");
	Init();
}

// デストラクタ
Sock::~Sock()
{
	Close();
}

// 読み込み
void Sock::LoadText(std::string fileName, std::string mode)
{
	std::string buf;
	FILE* file;
	if (fopen_s(&file, fileName.c_str(), mode.c_str()) != 0)
	{
		printf("ファイルを開けませんでした\n");
		return;
	}
	else
	{
		char str;
		while (true)
		{
			int len = fread(&str, sizeof(char), 1, file);
			if (len != 0)
			{
				buf.push_back(str);
			}
			else
			{
				break;
			}
		}

	}
	fclose(file);

	while (buf.size() > 0)
	{
		auto pos = buf.find_first_of(':');
		auto end = buf.find_first_of('\n');
		if (end >= buf.size())
		{
			end = buf.size();
		}
		else
		{
			end -= 1;
		}
		connection.push_back(buf.substr(pos + 1, end - pos - 1));
		end = buf.find_first_of('\n');
		if (end >= buf.size())
		{
			end = buf.size() - 1;
		}
		buf.erase(buf.begin(), buf.begin() + end + 1);
	}
}

// 初期化
int Sock::StartUp(void)
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
		default:
			break;
		}
	}
	else
	{
		printf("起動時の初期化：成功\n");
	}

	return result;
}

// ソケットの生成
bool Sock::CreateSock(void)
{
	//アドレスの設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(std::atoi(connection[0].c_str()));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	//ソケット生成
	sock = socket(addr.sin_family, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		printf("ソケット生成エラー：%d\n", WSAGetLastError());
		return false;
	}

	//ソケットの設定のセット
	BOOL flag = 1;
	result = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag));
	if (result != 0)
	{
		printf("ソケット設定：%d\n", WSAGetLastError());
	}
	else
	{
		printf("ソケットの生成：成功\n");
		printf("接続ポート番号は%sです\n", connection[0].c_str());
	}

	return true;
}

// アドレスとの関連付け
int Sock::Bind(void)
{
	result = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (result != 0)
	{
		printf("関連付けエラー：%d\n", WSAGetLastError());
	}
	else
	{
		printf("関連付け：成功\n");
	}

	return result;
}

// クライアントとの接続待機
int Sock::Listen(void)
{
	result = listen(sock, 5);
	if (result != 0)
	{
		printf("接続待機エラー：%d\n", WSAGetLastError());
	}
	else
	{
		printf("接続準備：成功\n");
	}

	return result;
}

// クライアントとの接続
void Sock::Accept(void)
{
	FD_ZERO(&readfds);
	for (UINT i = 0; i < CLIENT_MAX; ++i)
	{
		if (c_sock[i] != INVALID_SOCKET)
		{
			//FD_SET(c_sock[i], &readfds);
			continue;
		}
		else
		{
			int size = sizeof(sockaddr_in);
			c_sock[i] = accept(sock, (struct sockaddr *)&c_addr[i], &size);
			if (c_sock[i] != INVALID_SOCKET)
			{
				FD_SET(c_sock[i], &readfds);
				char str[INET_ADDRSTRLEN];
				printf("IP：%sが接続しました\n", inet_ntop(c_addr[i].sin_family, &c_addr[i].sin_addr, str, sizeof(str)));
			}
		}
	}
}

// 受信
void Sock::Recv(void)
{
	memcpy(&fds, &readfds, sizeof(fd_set));
	int len = select(0, &fds, NULL, NULL, &time);
	if (len != 0)
	{
		for (UINT i = 0; i < CLIENT_MAX; ++i)
		{
			if (c_sock[i] == INVALID_SOCKET)
			{
				continue;
			}
			else
			{
				if (FD_ISSET(c_sock[i], &fds))
				{
					// サーバからデータを受信
					memset(r, 0, sizeof(r));
					result = recv(c_sock[i], r, sizeof(r), 0);
					if (result == -1)
					{
						char str[INET_ADDRSTRLEN];
						printf("%sとの接続が切れました\n", inet_ntop(c_addr[i].sin_family, &c_addr[i].sin_addr, str, sizeof(str)));
						closesocket(c_sock[i]);
						c_sock[i] = INVALID_SOCKET;

					}
					else
					{
						char str[INET_ADDRSTRLEN];
						printf("%sからの受信：%s\n", inet_ntop(c_addr[i].sin_family, &c_addr[i].sin_addr, str, sizeof(str)), r);

						//指定IPアドレスに送信
						std::string dummy;
						for (int i = 0; r[i] != '/' && i < LENGTH_MAX; ++i)
						{
							dummy.push_back(r[i]);
						}
						for (int i = 0; i < CLIENT_MAX; ++i)
						{
							if (inet_addr(dummy.c_str()) == c_addr[i].sin_addr.S_un.S_addr)
							{
								std::string p;
								p = dummy + "：";
								for (int o = dummy.size() + 1; o < LENGTH_MAX; ++o)
								{
									p.push_back(r[o]);
								}
								if (send(c_sock[i], p.c_str(), sizeof(r), 0) == SOCKET_ERROR)
								{
									char str[INET_ADDRSTRLEN];
									printf("%sへの送信：失敗\n", inet_ntop(c_addr[i].sin_family, &c_addr[i].sin_addr, str, sizeof(str)));
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
	else
	{
		//printf("タイムアウト\n");
	}
}

// 送信
void Sock::Send(void)
{
	//入力バッファのクリア
	fflush(stdin);

	memset(s, 0, sizeof(s));
	scanf_s("%s", &s, sizeof(s));
	for (UINT i = 0; i < CLIENT_MAX; ++i)
	{
		if (c_sock[i] == INVALID_SOCKET)
		{
			continue;
		}
		else
		{
			if (send(c_sock[i], s, sizeof(s), 0) == SOCKET_ERROR)
			{
				char str[INET_ADDRSTRLEN];
				printf("%sへの送信：失敗\n", inet_ntop(c_addr[i].sin_family, &c_addr[i].sin_addr, str, sizeof(str)));
				printf("送信エラー：%d\n", WSAGetLastError());
			}
			else
			{
				char str[INET_ADDRSTRLEN];
				printf("%sへの送信：成功\n", inet_ntop(c_addr[i].sin_family, &c_addr[i].sin_addr, str, sizeof(str)));
			}
		}
	}
}

// クライアントとの接続終了
void Sock::Close(void)
{
	for (UINT i = 0; i < CLIENT_MAX; ++i)
	{
		if (c_sock[i] != INVALID_SOCKET)
		{
			closesocket(c_sock[i]);
			c_sock[i] = INVALID_SOCKET;
		}
	}
}

// 初期処理
void Sock::Init(void)
{
	result = Sock::StartUp();
	if (result != 0)
	{
		return;
	}

	if (CreateSock() == false)
	{
		return;
	}

	result = Sock::Bind();
	if (result != 0)
	{
		return;
	}

	result = Sock::Listen();
	if (result != 0)
	{
		return;
	}
}
