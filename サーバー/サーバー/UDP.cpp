#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "UDP.h"
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

// 文字最大数
#define LENGTH_MAX 1024

// コンストラクタ
UDP::UDP() : result(0)
{
	data = {};
	sock = INVALID_SOCKET;
	addr = {};
	connection.clear();
	r.clear();
	r.resize(LENGTH_MAX);


	LoadText("接続情報.txt");
	Init();
}

// デストラクタ
UDP::~UDP()
{
	closesocket(sock);
	WSACleanup();
}

// 受信
void UDP::Recv(void)
{
	memset(r.data(), 0, sizeof(r));
	recv(sock, r.data(), sizeof(r), 0);
}

// テキストの読み込み
void UDP::LoadText(std::string fileName, std::string mode)
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
int UDP::StartUp(void)
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

int UDP::CreateSock(void)
{
	//アドレスの設定
	addr.sin_family = AF_INET;
	addr.sin_port = htons(std::atoi(connection[0].c_str()));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	try
	{
		//ソケット生成
		sock = socket(addr.sin_family, SOCK_DGRAM, 0);
		if (sock == INVALID_SOCKET)
		{
			result = -1;
			throw result;
		}

		result = 0;
	}
	catch (int i)
	{
		if (i != 0)
		{
			printf("UDPソケット生成エラー：%d\n", WSAGetLastError());
		}
	}

	BOOL flag = 1;
	try
	{
		//ソケットの設定のセット
		result = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag));
		if (result != 0)
		{
			throw result;
		}

		printf("UDPソケットの生成：成功\n");
		printf("接続ポート番号は%sです\n", connection[0].c_str());
	}
	catch (int i)
	{
		if (i != 0)
		{
			printf("UDPソケット設定：%d\n", WSAGetLastError());
		}
	}

	return result;
}

int UDP::Bind(void)
{
	try
	{
		result = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
		if (result != 0)
		{
			throw result;
		}

		printf("UDP関連付け：成功\n");
	}
	catch (int i)
	{
		if (i != 0)
		{
			printf("UDP関連付けエラー：%d\n", WSAGetLastError());
		}
	}

	return result;
}

void UDP::Init(void)
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

	result = Bind();
	if (result != 0)
	{
		return;
	}
}
