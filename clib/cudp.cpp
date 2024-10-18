/***********************************************************************************************
created: 		2019-05-12

author:			chensong

purpose:		cmem_pool


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
************************************************************************************************/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "cudp.h"
#include <cstring>
#include "clog.h"
#include <iostream>
#include <cstring>
namespace chen {



	namespace udp_util
	{
		struct  addrinfo* ip_resolve_host(const char* hostname, int32 port, int32 type, int32 family, int32 flags)
		{
			struct addrinfo hints = { 0 }, * res = 0;
			char sport[16] = {0};
			int32 error;
			const char* node = 0, * service = "0";
			if (port > 0) 
			{
				snprintf(sport, sizeof(sport), "%d", port);
				service = sport;
			}
			if ((hostname) && (hostname[0] != '\0') && (hostname[0] != '?')) 
			{
				node = hostname;
			}
			hints.ai_socktype = type;
			hints.ai_family = family;
			hints.ai_flags = flags;
			if ((error = ::getaddrinfo(node, service, &hints, &res))) {
				res = NULL;
				/*av_log(log_ctx, AV_LOG_ERROR, "getaddrinfo(%s, %s): %s\n",
					node ? node : "unknown",
					service,
					gai_strerror(error));*/
				WARNING_EX_LOG("getaddrinfo(%s, %s): %s", (node ? node : "unknown"), service, gai_strerror(error));
			}
			return res;
		}
		int32 udp_set_url(sockaddr_storage* addr, const char* hostname, int32 port)
		{
			struct addrinfo* res0 = NULL;
			int addr_len;

			res0 = ip_resolve_host( hostname, port, SOCK_DGRAM, AF_UNSPEC, 0);
			if (!res0)
			{
				return EIO;
			}
			::memcpy(addr, res0->ai_addr, res0->ai_addrlen);
			addr_len = res0->ai_addrlen;
			::freeaddrinfo(res0);

			return addr_len;
		}
		int32 udp_socket_create(sockaddr_storage* addr, socklen_t* addr_len, const char* localaddr)
		{
			int udp_fd = -1;
			struct addrinfo* res0 = NULL, * res = NULL;
			int family = AF_UNSPEC;

			//if (((struct sockaddr*)&s->dest_addr)->sa_family)
			//	family = ((struct sockaddr*)&s->dest_addr)->sa_family;
			/*res0 = ip_resolve_host(  (localaddr && localaddr[0]) ? localaddr : NULL, s->local_port,
				SOCK_DGRAM, family, AI_PASSIVE);*/
			if (!res0)
			{
				goto fail;
			}
			for (res = res0; res; res = res->ai_next) 
			{
				/*if (s->udplite_coverage)
					udp_fd = ff_socket(res->ai_family, SOCK_DGRAM, IPPROTO_UDPLITE, h);
				else*/
				udp_fd = create_socket(res->ai_family, SOCK_DGRAM, 0);
				if (udp_fd != -1)
				{
					break;
				}
				WARNING_EX_LOG("not create udp socket failed !!!");
				//ff_log_net_error(h, AV_LOG_ERROR, "socket");
			}

			if (udp_fd < 0)
				goto fail;

			memcpy(addr, res->ai_addr, res->ai_addrlen);
			*addr_len = res->ai_addrlen;

			freeaddrinfo(res0);

			return udp_fd;

		fail:
			if (udp_fd >= 0)
			{
				//closesocket(udp_fd);
#ifdef _MSC_VER
// 关闭套接字
				closesocket(udp_fd);
#elif defined(__GNUC__) 
			// 关闭套接字
				close(udp_fd);
#else 

			 // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö§ï¿½ÖµÄ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Òªï¿½Ô¼ï¿½Êµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
#error unexpected c complier (msc/gcc), Need to implement this method for demangle

#endif 
			}
			if (res0)
			{
				freeaddrinfo(res0);
			}
			return -1;
		}
		int32 create_socket(int32 af, int32 type, int32 proto)
		{
			int fd;

#ifdef SOCK_CLOEXEC
			fd = socket(af, type | SOCK_CLOEXEC, proto);
			if (fd == -1 && errno == EINVAL)
#endif
			{
				fd = socket(af, type, proto);
#if HAVE_FCNTL
				if (fd != -1) {
					if (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1)
						av_log(logctx, AV_LOG_DEBUG, "Failed to set close on exec\n");
				}
#endif
			}
#ifdef SO_NOSIGPIPE
			if (fd != -1) {
				if (setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &(int){1}, sizeof(int))) {
					av_log(logctx, AV_LOG_WARNING, "setsockopt(SO_NOSIGPIPE) failed\n");
				}
			}
#endif
			return fd;
		}
		bool test_udp_connect(const char* ip, const char* udp_ip, uint32 port)
		{
#ifdef _MSC_VER
			static bool udp = false;
			if (!udp)
			{
				WSADATA wsaData;
				WSAStartup(MAKEWORD(2, 2), &wsaData);

				udp = true;
			}
#endif // #ifdef _MSC_VER
			int sockfd;
			struct sockaddr_in addr;
			struct ip_mreq mreq;
			char buffer[1024]={0};

			// 创建 UDP 套接字
			if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
			{
				//perror("socket creation failed");
				return false;
			}

			// 设置套接字选项，允许端口重用
			int optval = 1;
			setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));

			// 配置地址结构
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 接收所有的网络接口
			addr.sin_port = htons(port);

			// 绑定套接字
			if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) 
			{
				//perror("bind failed");
				// closesocket(sockfd);
#ifdef _MSC_VER
// 关闭套接字
				closesocket(sockfd);
#elif defined(__GNUC__) 
			// 关闭套接字
				close(sockfd);
#else 

			 // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö§ï¿½ÖµÄ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Òªï¿½Ô¼ï¿½Êµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
#error unexpected c complier (msc/gcc), Need to implement this method for demangle

#endif 
				return false;
			}

			// 加入组播组
			mreq.imr_multiaddr.s_addr = inet_addr(udp_ip);
			mreq.imr_interface.s_addr = htonl(INADDR_ANY); // 接收所有的网络接口
			if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) < 0) {
				//perror("setsockopt");
				// closesocket(sockfd);
#ifdef _MSC_VER
// 关闭套接字
				closesocket(sockfd);
#elif defined(__GNUC__) 
			// 关闭套接字
				close(sockfd);
#else 

			 // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö§ï¿½ÖµÄ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Òªï¿½Ô¼ï¿½Êµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
#error unexpected c complier (msc/gcc), Need to implement this method for demangle

#endif 
				return false;
			}
#ifdef _MSC_VER
			u_long param = 1;
			 ::ioctlsocket(sockfd, FIONBIO, &param);
#elif defined(__GNUC__) 
			 fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK);
#else 
 
			// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö§ï¿½ÖµÄ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Òªï¿½Ô¼ï¿½Êµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
#error unexpected c complier (msc/gcc), Need to implement this method for demangle
			 
#endif 
 
		 	 
			sockaddr_in clientAddr;
			int clientAddrSize = sizeof(clientAddr);
			int32 count = 0;
			
			while (  count++ < 50 )
			{
				//
				// 接收数据
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				int32 num_bytes_received = ::recvfrom(sockfd, buffer, 1024, 0, (sockaddr*)&clientAddr, &clientAddrSize);
				if (num_bytes_received < 0) 
				{
					//perror("recvfrom failed");
					continue;
				}
				if (std::string(::inet_ntoa(clientAddr.sin_addr)) != std::string(ip))
				{
					WARNING_EX_LOG("recv [address = %s][ip = %s]", inet_ntoa(clientAddr.sin_addr), ip);
				}

				if (num_bytes_received > 0)
				{
					// ::closesocket(sockfd);
#ifdef _MSC_VER
					 // 关闭套接字
					 closesocket(sockfd);
#elif defined(__GNUC__) 
					 // 关闭套接字
					 close(sockfd);
#else 

					 // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö§ï¿½ÖµÄ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Òªï¿½Ô¼ï¿½Êµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
#error unexpected c complier (msc/gcc), Need to implement this method for demangle

#endif 
					return true;
				}
				// 显示接收到的数据
				//std::cout << "address = " << inet_ntoa(clientAddr.sin_addr) << ", len = " << num_bytes_received << ", Received message: " << buffer << std::endl;
			}

			
#ifdef _MSC_VER
			// 关闭套接字
			closesocket(sockfd);
#elif defined(__GNUC__) 
			// 关闭套接字
			close(sockfd);
#else 

			 // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö§ï¿½ÖµÄ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Òªï¿½Ô¼ï¿½Êµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
#error unexpected c complier (msc/gcc), Need to implement this method for demangle

#endif 
			return false;
		}
		void test_udp_client()
		{
//#include <iostream>
//#include <cstring>
//#include <arpa/inet.h>
//#include <sys/socket.h>
//#include <netinet/in.h>

			//udp://@224.1.1.3:20000
#define PORT 20000
 
#define GROUP "224.1.1.3"  // 组播地址
#define BUF_SIZE 1024
		//	WSADATA wsaData;
		//	WSAStartup(MAKEWORD(2, 2), &wsaData);
			 
			int sockfd;
			struct sockaddr_in addr;
			struct ip_mreq mreq;
			char buffer[BUF_SIZE];

			// 创建 UDP 套接字
			if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
				perror("socket creation failed");
				return  ;
			}

			// 设置套接字选项，允许端口重用
			int optval = 1;
			setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));

			// 配置地址结构
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 接收所有的网络接口
			addr.sin_port = htons(PORT);

			// 绑定套接字
			if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
				perror("bind failed");
				return  ;
			}

			// 加入组播组
			mreq.imr_multiaddr.s_addr = inet_addr(GROUP);
			mreq.imr_interface.s_addr = htonl(INADDR_ANY); // 接收所有的网络接口
			if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) < 0) {
				perror("setsockopt");
				return  ;
			}
#define HAVE_WINSOCK2_H 0
		//	u_long param = 1;
		//	  ::ioctlsocket(sockfd, FIONBIO, &param);
#if HAVE_WINSOCK2_H
			u_long param = enable;
			return ioctlsocket(socket, FIONBIO, &param);
#else
		//	if (enable)
				// fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK);
			/*else
				return fcntl(socket, F_SETFL, fcntl(socket, F_GETFL) & ~O_NONBLOCK);*/
#endif /* HAVE_WINSOCK2_H */
			std::cout << "Listening for multicast messages on " << GROUP << ":" << PORT << std::endl;
			sockaddr_in clientAddr;
			int clientAddrSize = sizeof(clientAddr);
			while (true) 
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				// 接收数据
				 int32 num_bytes_received = recvfrom(sockfd, buffer, BUF_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
				if (num_bytes_received < 0) {
					perror("recvfrom failed");
					continue;
				}

				// 显示接收到的数据
				std::cout << "address = "<<  inet_ntoa(clientAddr.sin_addr) << ", len = " <<  num_bytes_received << ", Received message: " << buffer << std::endl;
			}

			// 关闭套接字
		//	closesocket(sockfd);

#ifdef _MSC_VER
			// 关闭套接字
			closesocket(sockfd);
#elif defined(__GNUC__) 
			// 关闭套接字
			close(sockfd);
#else 

			// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö§ï¿½ÖµÄ±ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Òªï¿½Ô¼ï¿½Êµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
#error unexpected c complier (msc/gcc), Need to implement this method for demangle

#endif 
		}
	}
}
