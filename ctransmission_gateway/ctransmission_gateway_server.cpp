/***********************************************************************************************
created: 		2023-11-18

author:			chensong

purpose:		camera

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
#include "ctransmission_gateway_server.h"
#include "ccfg.h"
#include "clog.h"
#include "ctime_api.h"
#include "cwebsocket_server_mgr.h"
#include "cwebsocket_wan_server.h"
#include "ctime_elapse.h"
#include <turbojpeg.h>
#include "cbase64.h"
#include "cffmpeg_util.h"
#include "cwebsocket_wan_server.h"
#include "cdecoder.h"
#include "libyuv.h"
//#include <jpeglib.h>
#include <turbojpeg.h>

namespace chen {

 
	ctransmission_gateway_server  g_transmission_gateway_server;
	 
 
	static const char HEX[16] = {
			'0', '1', '2', '3',
			'4', '5', '6', '7',
			'8', '9', 'a', 'b',
			'c', 'd', 'e', 'f'
	};


#define _throw(action, message) { \
  printf("ERROR in line %d while %s:\n%s\n", __LINE__, action, message); \
  retval = -1;  goto bailout; \
}
#define _throwtj(action)  _throw(action, tjGetErrorStr2(tjInstance))





	void  encode_jpeg_by_cpu(const unsigned char* input, int32_t width, int32_t height, int32_t pixel_fromat, unsigned char** output, unsigned long* size)
	{
		int retval = 0;
		tjhandle tjInstance = NULL;	// 句柄
		unsigned char* jpegBuf = NULL;
		// pixelFormat = { TJPF_RGB, TJPF_BGR, TJPF_BGRX, TJPF_BGRX, TJPF_XRGB, TJPF_GRAY, TJPF_RGBA,
		// 				TJPF_BGRA, TJPF_ABGR, TJPF_ARGB, TJPF_CMYK, TJPF_UNKNOWN};

		*size = 0;
		if ((tjInstance = tjInitCompress()) == NULL)	// 创建一个TurboJPEG编码器实例
		{
			goto bailout;
		//	_throwtj("initializing compressor");
		}
		if (tjCompress2(tjInstance, (const unsigned char*)input, width, 0, height, pixel_fromat,	// 将图像编码为jpeg文件
			&jpegBuf, size, TJSAMP_444, 50, 0) < 0)     // 编码质量为90，取值范围为[1,100],超过95会导致性能下降严重。
		{
			//_throwtj("compressing image");
			goto bailout;
		}

		tjDestroy(tjInstance);  tjInstance = NULL;		// 销毁一个TurboJPEG编码器实例
		memcpy(*output, jpegBuf, *size);
		tjFree(jpegBuf);	jpegBuf = NULL;

	bailout:
		if (tjInstance) tjDestroy(tjInstance);
		if (jpegBuf) tjFree(jpegBuf);

	}


	 
	std::string get_hex_str(const void* _buf, int num)
	{
		std::string str;
		str.reserve(num << 1);
		const unsigned char* buf = (const unsigned char*)_buf;

		unsigned char tmp;
		for (int i = 0; i < num; ++i)
		{
			tmp = buf[i];
			str.append(1, HEX[tmp / 16]);
			str.append(1, HEX[tmp % 16]);
		}
		return str;
	}

	static unsigned char* byte_ptr = NULL;
	static unsigned char* yuv_ptr = NULL;
	static unsigned char* nv12_ptr = NULL;
	static  void nv12_callback_function(const AVFrame* frame)
	{
		if (!frame)
		{
			return;
		}
		if (!byte_ptr)
		{
			byte_ptr = (unsigned char*) ::malloc((frame->width * frame->height * 4) * sizeof(unsigned char));
		
		//	yuv_ptr = (unsigned char*) ::malloc((frame->width * frame->height * 4) * sizeof(unsigned char));
			nv12_ptr = (unsigned char*) ::malloc((frame->width * frame->height * 4) * sizeof(unsigned char));
		}
		//unsigned long  jpeg_size = 0;

	//	memcpy(nv12_ptr, frame->data[0], frame->height * frame->linesize[0]);
	//	memcpy(nv12_ptr + (frame->height * frame->linesize[0]), frame->data[1], frame->height * frame->linesize[1]/2);
		//
		//convertNV12toYUV420P(nv12_ptr, yuv_ptr, frame->width, frame->height);
		//tyuv2jpeg(yuv_ptr, frame->height * frame->linesize[0], frame->width,
		//	frame->height, &byte_ptr, &jpeg_size, 90); //60:qualit
		unsigned long size = 0;
		/*
		const uint8_t* src_y,
               int src_stride_y,
               const uint8_t* src_uv,
               int src_stride_uv,
               uint8_t* dst_argb,
               int dst_stride_argb,
               int width,
               int height
		*/
		libyuv::NV12ToARGB(frame->data[0], frame->linesize[0], frame->data[1], frame->linesize[1], 
			byte_ptr, frame->width* 4, frame->width, frame->height);
		encode_jpeg_by_cpu(byte_ptr, frame->width, frame->height, TJPF_BGRA, &nv12_ptr, &size);
		//encode_jpeg_by_cpu(nv12_ptr, frame->width, frame->height, TJPF_BGRA, (void*)yuv_ptr, &size);
		std::string s((char*)nv12_ptr, size);
		std::string input = chen::base64_encode(s);
		{
			//WARNING_EX_LOG("[base64 = %u]", input.size()/1024);
			g_websocket_wan_server.broadcast_msg(0, input.c_str(), input.length());
		}
	}
	bool ctransmission_gateway_server::Init(const char* log_path, const char* config_file)
	{
		printf("Log init ...\n");
		if (!LOG::init(log_path, "video_split"))
		{
			return false;
		}
		SYSTEM_LOG("config init ...");
		if (!g_cfg.init(config_file))
		{
			return false;
		}
		LOG::set_level(static_cast<ELogLevelType>(g_cfg.get_uint32(ECI_LogLevel)));
		ctime_base_api::set_time_zone(g_cfg.get_int32(ECI_TimeZone));
		ctime_base_api::set_time_adjust(g_cfg.get_int32(ECI_TimeAdjust));

		SYSTEM_LOG("websocket wan server  init ...");
		if (!g_websocket_wan_server.init())
		{
			return false;
		}
		SYSTEM_LOG("websocket wan server  startup ...");
		if (!g_websocket_wan_server.startup())
		{
			return false;
		}
		static chen::cdecoder decoder;
		std::string url = "rtsp://admin:admin12345@192.168.2.212:554/cam/realmonitor?channel=1&subtype=0";

		decoder.set_callback(& nv12_callback_function);
		if (!decoder.init(url.c_str(), 0))
		{
			WARNING_EX_LOG("decoder init (url = %s)failed !!!", url.c_str());
			return -1;
		}


		SYSTEM_LOG(" transmission gateway server init ok");


		//tyuv2jpeg(image.yuv420p, yuv_size, image.input_w,
		//	image.input_h, &jpeg_buf, &jpeg_size, 60); //60:quality


		return true;
	}
	


	void ctransmission_gateway_server::Loop()
	{
		static const uint32 TICK_TIME = 100;
		//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È´ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½ï¿½
		NORMAL_EX_LOG("");
		ctime_elapse time_elapse;
		uint32 uDelta = 0;
		while (!m_stoped)
		{
			uDelta += time_elapse.get_elapse();
			//	NORMAL_EX_LOG("");
	//			g_game_client.update(uDelta);
			//g_cmd_server.update(uDelta);
			g_websocket_wan_server.update(uDelta);
			//g_cmd_parser.update(uDelta);


			//g_global_logic_mgr.update(uDelta);
		//	g_http_queue_mgr.update();
			uDelta = time_elapse.get_elapse();

			if (uDelta < TICK_TIME)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(TICK_TIME - uDelta));
			}
		}

		SYSTEM_LOG("Leave main loop");
	}
	void ctransmission_gateway_server::Destroy()
	{
		g_websocket_wan_server.shutdown();
		g_websocket_wan_server.destroy();
		SYSTEM_LOG("g_wan_server Destroy OK!!!");





		SYSTEM_LOG(" cfg  destroy OK !!!");
		g_cfg.destroy();

		//g_client_msg_dispatch.destroy();
		//SYSTEM_LOG("msg dispath destroy OK !!!");

		//1 log
		LOG::destroy();
		printf("Decoder Server  Destroy OK end !\n");
	}
	void ctransmission_gateway_server::stop()
	{
		m_stoped = true;
	}
}