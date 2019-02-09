#include "Client/Client.h"
#include <thread>

int main()
{
	auto& s = Client::Get();
	std::thread recv([&]()->void {
		while (true)
		{
			auto tmp = s.Recv();

			if (tmp.size() > 0)
			{
				std::string n(tmp.begin(), tmp.end());
				printf("ŽóMF%s\n", n.c_str());
			}
		}
	});
	std::thread send([&]()->void {
		while (true)
		{
			char tmp[256];
			scanf_s("%s", tmp, _countof(tmp));
			s.Send(tmp, _countof(tmp));
		}
	});

	recv.join();
	send.join();

	return 0;
}