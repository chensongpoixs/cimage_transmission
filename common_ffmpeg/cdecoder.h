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


#ifndef _C_DECODER_H_
#define _C_DECODER_H_

#include "cffmpeg_util.h"
#include "cffmpeg_util.h"
#include <list>
#include "cnet_type.h"
#include <thread>
/*

大华摄像头的RTSP地址格式
rtsp地址格式:

代码语言：javascript
复制
rtsp://username:password@ip:port/cam/realmonitor?channel=1&subtype=0
参数解释:

username: 摄像头登录用户名 (就是登录摄像头web管理页面的用户名和密码)

 password: 摄像头登录密码

 ip:  摄像头设备本身的IP

 port: 端口号

 channel: 通道号，起始为1。例如通道2，则为channel=2

 subtype: 码流类型，主码流（subtype=0），辅码流（subtype=1）

示例:  这是我的摄像头访问地址

rtsp://admin:admin12345@192.168.2.212:554/cam/realmonitor?channel=1&subtype=0
*/
namespace chen {




	typedef void (*nv12_call_back)(const AVFrame * frame);

	class cdecoder
	{
	private:
		typedef   std::mutex						clock_type;
		typedef   std::lock_guard<clock_type>       clock_guard;
	public:
		explicit cdecoder()
		: m_stoped(false)
		, m_url("")
		, m_gpu_index(0)
		, m_fmt_ctx_ptr(NULL)
		, m_codec_ctx_ptr(NULL)
		, m_video_stream_index(-1)
		, m_video_stream_ptr(NULL)
		, m_callback_ptr(NULL){}


		virtual ~cdecoder();


	public:
		bool init(const char * url, uint32 gpu_index = 0);
		void update(uint32 DataTime);
		void destroy();
		void set_callback( nv12_call_back callback) { m_callback_ptr = callback; }
	private:
		void _push_packet(AVPacket* packet);
		AVPacket* _pop_packet();


		bool  _writable_packet_list();

	private:
		void _read_pthread();
		void _decode_pthread();



	
	public:



		bool			m_stoped;
		std::string		m_url;
		uint32			m_gpu_index;
		
		
		AVFormatContext* m_fmt_ctx_ptr;

		// 解码
		AVCodecContext* m_codec_ctx_ptr;
		
		// 视频流索引
		int32_t	   m_video_stream_index;
		// 视频流
		AVStream* m_video_stream_ptr;


		std::thread			m_read_thread;
		std::thread			m_decode_thread;
		
		clock_type			m_packet_lock;
		std::list<AVPacket* > m_packet_list;
		nv12_call_back		m_callback_ptr;
		
	};



















}

#endif //_C_DECODER_H_