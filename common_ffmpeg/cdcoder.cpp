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


//#include "cdecoder.h"

#include "cdcoder.h"
#include "clog.h"

namespace chen {
	 
	cdecoder::~cdecoder()
	{
	}
	bool cdecoder::init(const char * url, uint32 gpu_index)
	{
		std::lock_guard<std::mutex> lock(g_ffmpeg_lock);
		/*if (!m_stoped)
		{
			return false;
		}*/
		m_gpu_index = gpu_index;
		m_url = url;
		
		m_fmt_ctx_ptr = ::avformat_alloc_context();
		if (!m_fmt_ctx_ptr)
		{
			WARNING_EX_LOG("[url = %s]alloc avformat  context failed !!!", m_url.c_str());
			return false;
		}
		int32 ret = ::avformat_open_input(&m_fmt_ctx_ptr, m_url.c_str(), NULL, NULL);
		if (ret != 0)
		{
			WARNING_EX_LOG("avformat open (url = %s) input (%s) failed !!!", m_url.c_str(), ffmpeg_util::make_error_string(ret));
			return false;
		}

		//2.读取文件信息
		ret = ::avformat_find_stream_info(m_fmt_ctx_ptr, NULL);
		if (ret < 0)
		{
			WARNING_EX_LOG("avformat_find_stream_info   (url = %s)     %s failed !!!",  m_url.c_str(), ffmpeg_util::make_error_string(ret));
			return false;
		}

		//3.获取目标流索引
		for (unsigned int i = 0; i < m_fmt_ctx_ptr->nb_streams; i++)
		{
			AVStream* stream = m_fmt_ctx_ptr->streams[i];
			if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				if (!m_video_stream_ptr)
				{
					m_video_stream_index = i;
					m_video_stream_ptr = stream;
				}
				else
				{
					stream->discard = AVDISCARD_ALL;
				}
			}
			else
			{
				stream->discard = AVDISCARD_ALL;
			}
		}
		const AVCodec* codec = NULL;
		//4.查找解码器
		if (AV_CODEC_ID_H264 == m_video_stream_ptr->codecpar->codec_id)
		{
			codec = ::avcodec_find_decoder_by_name("h264_cuvid");
		}
		else if (AV_CODEC_ID_HEVC == m_video_stream_ptr->codecpar->codec_id)
		{
			codec = ::avcodec_find_decoder_by_name("hevc_cuvid");
		}
		else
		{
			codec = ::avcodec_find_decoder(m_video_stream_ptr->codecpar->codec_id);
		}
		if (!codec)
		{
			WARNING_EX_LOG("can't find codec, codec id:%d ", m_video_stream_ptr->codecpar->codec_id);
			return false;
		}
		//AVBufferRef* hw_device_ctx = NULL;
		////创建GPU设备 默认第一个设备  也可以指定gpu 索引id 
		//std::string gpu_index = "0";
		//ret = av_hwdevice_ctx_create(&hw_device_ctx, AV_HWDEVICE_TYPE_CUDA, gpu_index.c_str(), NULL, 0);

		//5.创建解码器上下文
		if (!(m_codec_ctx_ptr = avcodec_alloc_context3(codec)))
		{
			WARNING_EX_LOG("avcodec_alloc_context3 failed !!! ");
			return false;
		}

		//6.从输入流复制编解码器参数到输出编解码器上下文
		if ((ret = avcodec_parameters_to_context(m_codec_ctx_ptr, m_video_stream_ptr->codecpar)) < 0)
		{
			WARNING_EX_LOG("Failed to copy %s codec parameters to decoder context ",
				av_get_media_type_string(m_video_stream_ptr->codecpar->codec_type));
			return false;
		}
		{
			/* set hw_frames_ctx for encoder's AVCodecContext */
			//if ((ret = set_hwframe_ctx(m_codec_ctx_ptr, hw_device_ctx)) < 0) {
			//	WARNING_EX_LOG("Failed to set hwframe context.\n");
			//	//goto close;
			//}
			//m_codec_ctx_ptr->hw_device_ctx = av_buffer_ref(hw_device_ctx);
			//m_codec_ctx_ptr->get_format = get_format;
		}
		AVDictionary* codec_opts = NULL;
		//av_dict_set(&codec_opts, "b", "2.5M", 0);
		av_dict_set(&codec_opts, "gpu", std::to_string(m_gpu_index).c_str(), 0);
		av_dict_set(&codec_opts, "threads", "auto", 0);
		av_dict_set(&codec_opts, "flags", "+copy_opaque", AV_DICT_MULTIKEY);
		/*	name dirtc[key = gpu][value = 1]
				name dirtc[key = threads][value = auto]
				name dirtc[key = flags][value = +copy_opaque]*/
		NORMAL_EX_LOG("use gpu index = %u", m_gpu_index);
		//7. 打开解码器上下文 */
		if ((ret = avcodec_open2(m_codec_ctx_ptr, codec, &codec_opts)) < 0)
		{
			::av_dict_free(&codec_opts);
			WARNING_EX_LOG("Failed to open %s [gpu = %u] codec (%s)",
				av_get_media_type_string(m_video_stream_ptr->codecpar->codec_type), m_gpu_index, ffmpeg_util::make_error_string(ret));
			return false;
		}
		return true;
	}
	void cdecoder::update(uint32 DataTime)
	{
	}
	void cdecoder::destroy()
	{
	}
}