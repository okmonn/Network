#include "Server/Server.h"
#include <thread>

int main()
{
	auto& s = Server::Get();
	std::thread acc([&]()->void {
		while (true)
		{
			s.Accept();
		}
	});
	std::thread recv([&]()->void {
		while (true)
		{
			auto tmp = s.Recv();
			if (tmp.size() > 0)
			{
				printf("ŽóMF%s\n", tmp.c_str());
			}
		}
	});
	std::thread send([&]()->void {
		while (true)
		{
			char tmp[256];
			scanf_s("%s", tmp, unsigned int(_countof(tmp)));
			s.Send(tmp, _countof(tmp));
		}
	});

	acc.join();
	recv.join();
	send.join();

	return 0;
}