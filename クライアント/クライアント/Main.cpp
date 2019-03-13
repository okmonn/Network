#include "Client/Client.h"
#include <thread>

int main()
{
	bool threadFlag = true;
	auto& s = Client::Get();
	std::thread recv([&]()->void {
		while (threadFlag)
		{
			auto tmp = s.Recv();
			if (tmp == std::nullopt)
			{
				threadFlag = false;
				continue;
			}

			if (tmp->size() > 0)
			{
				printf("ŽóMF%s\n", tmp->c_str());
			}
		}
	});
	std::thread send([&]()->void {
		while (threadFlag)
		{
			char tmp[256];
			scanf_s("%s", tmp, unsigned int(_countof(tmp)));
			s.Send(tmp, _countof(tmp));
		}
	});

	send.detach();
	recv.join();

	return 0;
}