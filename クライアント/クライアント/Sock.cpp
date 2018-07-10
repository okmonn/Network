#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Sock.h"
#include <iostream>
#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")

// コンストラクタ
Sock::Sock() : result(0)
{
	data = {};
	sock = INVALID_SOCKET;
	addr = {};
	connection.clear();
	list.clear();
	fds = {};
	readfds = {};
	time = { 0, 1000 };
	memset(&r, 0, sizeof(r));
	memset(&s, 0, sizeof(s));
	end = false;

	LoadText("接続情報.txt");
	Init();
}

// デストラクタ
Sock::~Sock()
{
	Close();
	WSACleanup();
}

// テキスト読み込み
void Sock::LoadText(std::string fileName, std::string mode)
{
	std::string buf;
	FILE* file;
	try
	{
		//ファイルを開く
		if (fopen_s(&file, fileName.c_str(), mode.c_str()) != 0)
		{
			throw - 1;
		}

		//読み込み
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
		fclose(file);

		//文字列検索
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
	catch (int i)
	{
		if (i != 0)
		{
			printf("ファイルを開けませんでした\n");
		}
	}
}

// 初期化
int Sock::StartUp(void)
{
	try
	{
		result = WSAStartup(MAKEWORD(2, 0), &data);
		if (result != 0)
		{
			throw result;
		}

		printf("起動時の初期化：成功\n");
	}
	catch (int i)
	{
		switch (i)
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

	return result;
}

// ソケットの生成
int Sock::CreateSock(void)
{
	//使用PC名前
	char name[256];

	//ホスト情報
	struct hostent *h;

	try
	{
		result = gethostname(name, 256);
		if (result != 0)
		{
			throw result;
		}

		printf("このコンピュータの名前は%sです\n", name);

		h = gethostbyname(connection[0].c_str());
		printf("ホストサーバーの名前は%sです\n", h->h_name);
	}
	catch (int i)
	{
		if (i != 0)
		{
			printf("ホスト名の取得に失敗しました：%d\n", WSAGetLastError());
		}
	}

	//アドレス情報
	in_addr i_addr;
	for (int i = 0; h->h_addr_list[i]; ++i)
	{
		memcpy(&i_addr, h->h_addr_list[i], 4);
		list.push_back(i_addr.S_un.S_addr);
	}

	//ソケットの設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(std::atoi(connection[1].c_str()));
	addr.sin_addr.S_un.S_addr = i_addr.S_un.S_addr;

	try
	{
		//ソケットの生成
		sock = socket(addr.sin_family, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			result = -1;
			throw result;
		}

		result = 0;
		printf("ソケット生成：成功\n");
		printf("接続ポート番号は%sです\n", connection[1].c_str());
	}
	catch (int i)
	{
		if (i != 0)
		{
			printf("ソケット生成：失敗 : %d\n", WSAGetLastError());
		}
	}
	
	return result;
}

// サーバーとの接続
int Sock::Connect(void)
{
	FD_ZERO(&readfds);
	for (UINT i = 0; i < list.size(); ++i)
	{
		addr.sin_addr.S_un.S_addr = list[i];
		try
		{
			result = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
			if (result != 0)
			{
				throw result;
			}

			char str[INET_ADDRSTRLEN];
			printf("%sに接続しました\n", inet_ntop(addr.sin_family, &addr.sin_addr, str, sizeof(str)));
			FD_SET(sock, &readfds);
			break;
		}
		catch (int i)
		{
			if (i != 0)
			{
				printf("サーバー接続：失敗 : %d\n", WSAGetLastError());
				continue;
			}
		}
	}

	return result;
}

// 初期セット
void Sock::Init(void)
{
	result = StartUp();
	if (result != 0)
	{
		return;
	}

	result = CreateSock();
	if (result != 0)
	{
		return;
	}

	result = Connect();
	if (result != 0)
	{
		return;
	}
}

// 受信
void Sock::Recv(void)
{
	memcpy(&fds, &readfds, sizeof(fd_set));
	int len = select(0, &fds, NULL, NULL, &time);
	if (len != 0)
	{
		if (FD_ISSET(sock, &fds))
		{
			memset(r, 0, sizeof(r));
			result = recv(sock, r, sizeof(r), 0);
			if (result == -1)
			{
				printf("サーバーが立ち上がっていません\n");
				end = true;
			}
			else
			{
				printf("受信：%s\n", r);
			}
		}
	}
}

// 送信
void Sock::Send(void)
{
	fflush(stdin);
	memset(s, 0, sizeof(s));
	scanf_s("%s", &s, sizeof(s));
	if (sendto(sock, s, strlen(s), 0, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		printf("送信：失敗：%d\n", WSAGetLastError());
	}
	else
	{
		printf("送信：%s\n", &s);
	}
}

// ソケットを閉じる
void Sock::Close(void)
{
	if (sock != INVALID_SOCKET)
	{
		closesocket(sock);
		sock = INVALID_SOCKET;
	}
}