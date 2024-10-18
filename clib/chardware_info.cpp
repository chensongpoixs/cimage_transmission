/********************************************************************
created:	2019-03-24

author:		chensong

purpose:	×Ö·û´®´¦Àí¹¤¾ßÀà
输赢不重要，答案对你们有什么意义才重要。

光阴者，百代之过客也，唯有奋力奔跑，方能生风起时，是时代造英雄，英雄存在于时代。或许世人道你轻狂，可你本就年少啊。 看护好，自己的理想和激情。


我可能会遇到很多的人，听他们讲好2多的故事，我来写成故事或编成歌，用我学来的各种乐器演奏它。
然后还可能在一个国家遇到一个心仪我的姑娘，她可能会被我帅气的外表捕获，又会被我深邃的内涵吸引，在某个下雨的夜晚，她会全身淋透然后要在我狭小的住处换身上的湿衣服。
3小时候后她告诉我她其实是这个国家的公主，她愿意向父皇求婚。我不得已告诉她我是穿越而来的男主角，我始终要回到自己的世界。
然后我的身影慢慢消失，我看到她眼里的泪水，心里却没有任何痛苦，我才知道，原来我的心被丢掉了，我游历全世界的原因，就是要找回自己的本心。
于是我开始有意寻找各种各样失去心的人，我变成一块砖头，一颗树，一滴水，一朵白云，去听大家为什么会失去自己的本心。
我发现，刚出生的宝宝，本心还在，慢慢的，他们的本心就会消失，收到了各种黑暗之光的侵蚀。
从一次争论，到嫉妒和悲愤，还有委屈和痛苦，我看到一只只无形的手，把他们的本心扯碎，蒙蔽，偷走，再也回不到主人都身边。
我叫他本心猎手。他可能是和宇宙同在的级别 但是我并不害怕，我仔细回忆自己平淡的一生 寻找本心猎手的痕迹。
沿着自己的回忆，一个个的场景忽闪而过，最后发现，我的本心，在我写代码的时候，会回来。
安静，淡然，代码就是我的一切，写代码就是我本心回归的最好方式，我还没找到本心猎手，但我相信，顺着这个线索，我一定能顺藤摸瓜，把他揪出来。
*********************************************************************/

#include "chardware_info.h"
#include <stdio.h>
#include <stdlib.h>
#include "cbase64.h"
#include <string>
#include "clog.h"
namespace chen {


	namespace    hardware_info
	{

#ifdef _MSC_VER
#include <winsock2.h>
#include <iphlpapi.h>
#include <wlanapi.h>

		// Link with Iphlpapi.lib and Wlanapi.lib
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Wlanapi.lib")

#elif defined(__GNUC__) 
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

		bool net_comp(const std::string & local_net)
		{
			if (local_net == "lo")
			{
				return false;
			}

			if (local_net.size() > 6)
			{
				static std::string docker_net_name = "docker";
				for (size_t i = 0; i < 6; ++i)
				{
					if (local_net[i] != docker_net_name[i])
					{
						break;
					}
					if (i == 5)
					{
						return false;
					}
				}
			}

			return true;
		}

		std::string printMacAddress(const char* iface_name)
		{
			struct ifreq ifr;
			int sockfd, ret;
			//unsigned char mac_addr[6] = {0};
			// Open socket
			sockfd = socket(AF_INET, SOCK_DGRAM, 0);
			if (sockfd < 0) 
			{
				WARNING_EX_LOG("socket");
				return "";
			}

			// Get MAC address
			strncpy(ifr.ifr_name, iface_name, IFNAMSIZ - 1);
			ifr.ifr_name[IFNAMSIZ - 1] = '\0';
			ret = ioctl(sockfd, SIOCGIFHWADDR, &ifr);
			if (ret < 0) 
			{
				WARNING_EX_LOG("ioctl");
				close(sockfd);
				return "";
			}

			std::string buffer_mac;

			// Copy MAC address
			//memcpy(mac_addr, ifr.ifr_hwaddr.sa_data, 6);
			std::string wifi = base64_encode((std::uint8_t const*)&ifr.ifr_hwaddr.sa_data[0], 6);




			// Close socket
			close(sockfd);

			// Print MAC address
			//NORMAL_EX_LOG("Interface Name: %s\n", iface_name);
		//	NORMAL_EX_LOG("MAC Address: %s ", wifi.c_str());
			/*for (int i = 0; i < 6; i++) 
			{
				NORMAL_EX_LOG("%02X", mac_addr[i]);
				unsigned char buffer[5] = {0};
				sprintf(buffer, "%02X", mac_addr[i]);
				buffer_mac += buffer;

				if (i < 5) 
				{
					NORMAL_EX_LOG(":");
				}
			}*/
			//NORMAL_EX_LOG("\n\n");
			return 	wifi;
		}
#else 

#error unexpected c complier (msc/gcc), Need to implement this method for demangle

#endif // 
		std::string  network_info()
		{
			std::string wifi_net;
#ifdef _MSC_VER
			DWORD dwRetVal = 0;
			ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
			PIP_ADAPTER_INFO pAdapterInfo = NULL;
			pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
			if (pAdapterInfo == NULL) 
			{
				WARNING_EX_LOG("Error allocating memory needed to call GetAdaptersinfo\n");
				return wifi_net;
			}
			 
			// Make an initial call to GetAdaptersInfo to get the necessary size into ulOutBufLen
			if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
			{
				::free(pAdapterInfo);
				pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
				if (pAdapterInfo == NULL) 
				{
					WARNING_EX_LOG("Error allocating memory needed to call GetAdaptersinfo\n");
					return wifi_net;
				}
			}

			// Now make the call to GetAdaptersInfo
			if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
			{
				PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
				while (pAdapter) {
					//NORMAL_EX_LOG("Adapter Name: %s\n", pAdapter->AdapterName);
					wifi_net += pAdapter->AdapterName;
					//NORMAL_EX_LOG("Description : %s\n", pAdapter->Description);
					//NORMAL_EX_LOG("Adapter MAC Address: ");
					//for (UINT i = 0; i < pAdapter->AddressLength; i++) 
					//{
					//	if (i == (pAdapter->AddressLength - 1)) 
					//	{
					//		//NORMAL_EX_LOG("%.2X\n", (int)pAdapter->Address[i]);
					//	}
					//	else {
					//		NORMAL_EX_LOG("%.2X-", (int)pAdapter->Address[i]);
					//	}
					//}
					pAdapter = pAdapter->Next;
				}
			}
			else 
			{
				WARNING_EX_LOG("GetAdaptersInfo failed with error: %d\n", dwRetVal);
			}

			if (pAdapterInfo) 
			{
				free(pAdapterInfo);
			}
#elif defined(__GNUC__) 
			struct if_nameindex* if_ni, * i;
			if_ni = if_nameindex();
			if (if_ni == NULL)
			{
				WARNING_EX_LOG("if_nameindex");
				return wifi_net;
			}

			// Iterate through each interface
			for (i = if_ni; i->if_name != NULL; i++) 
			{
				if (net_comp(i->if_name))
				{
					//WARNING_EX_LOG("%s", i->if_name);
					wifi_net += printMacAddress(i->if_name);
				}
				else 
				{
					//WARNING_EX_LOG("====>%s", i->if_name);
				}
			}

			if_freenameindex(if_ni);
#else 

#error unexpected c complier (msc/gcc), Need to implement this method for demangle

#endif // 
			return wifi_net;
		}

		std::string  gpu_info()
		{
		
			return "";
		}

	}
}
