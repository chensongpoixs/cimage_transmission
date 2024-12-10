

#include <iostream>
#include "cdecoder.h"
#include "clog.h"
#include "ctransmission_gateway_server.h"
#include "nvjpeg2k_encode.h"

int main(int argc, const char* argv[])
{
	test_main(argc, argv);
	return EXIT_SUCCESS;
	// PostMessage(g_wnd, message_id, param1, param2);
	

	//return 0;
	//{
	//	unsigned long  mask = 0;
	//	for (int i = 0; i < 4; ++i)
	//	{
	//		mask |= (1 << i);
	//	}
	//	printf("mask = %u\n", mask);
	//}
	//int  i = 0;
	//i = 1 << 1;
	//int w = 1 << 2;
	//int y = 1 >> 2;


	////GetProcessAffinityMask();
	//SYSTEM_INFO info;
	//GetSystemInfo(&info);
	//// dwNumberOfProcessors
	//unsigned long  mask = 0;
	////OpenProcess();
	////SetThreadAffinityMask();
	////SetThreadAffinityMask();

	//return EXIT_SUCCESS;

	//RegisterSignal();



	const char* config_filename = "server.cfg";
	if (argc > 1)
	{
		config_filename = argv[1];
	}
	const char* log_path = "./";
	if (argc > 2)
	{
		log_path = argv[2];
	}

	 bool init = chen::g_transmission_gateway_server.Init(log_path, config_filename);

	if (init)
	{
		 chen::g_transmission_gateway_server.Loop();
	}

	chen::g_transmission_gateway_server.Destroy();
	
	/*using namespace chen;

	LOG::init("./log", "bt_gateway");
	LOG::set_level(ELogLevel_Num);

	chen::cdecoder decoder;
	std::string url = "rtsp://admin:admin12345@192.168.2.212:554/cam/realmonitor?channel=1&subtype=0";

	if (!decoder.init(url.c_str(), 0))
	{
		WARNING_EX_LOG("decoder init (url = %s)failed !!!", url.c_str());
		return -1;
	}

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}*/



	return EXIT_SUCCESS;
}