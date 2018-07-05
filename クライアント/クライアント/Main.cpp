#include "Main.h"
#include "Sock.h"
#include <thread>

// クラスの生成
void Create(void)
{
	sock = std::make_shared<Sock>();
}

// 受信
void Recv(void)
{
	while (sock->GetEnd() == false)
	{
		sock->Recv();
	}
}

// 送信
void Send(void)
{
	while (true)
	{
		sock->Send();
	}
}

// エントリーポイント
int main()
{
	Create();

	std::thread recv(Recv);
	std::thread send(Send);

	recv.join();
	send.detach();


	return 0;
}
