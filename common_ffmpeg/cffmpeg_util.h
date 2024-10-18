/***********************************************************************************************
created: 		2024-01-25

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


#ifndef _C_FFMPEG_UTIL_H_
#define _C_FFMPEG_UTIL_H_

#include <cstdint>
//#include <GL/eglew.h>
#include <vector>
//#include <ctime>
#include <string>
#include <mutex>
extern "C"
{
#include <libavutil/frame.h>
#include <libavutil/avutil.h>
//#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavutil/display.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavfilter/buffersink.h>
#include <libavutil/cpu.h>
}

#include <cassert>


#define USE_AV_INTERRUPT_CALLBACK (1)

 
#define CALC_FFMPEG_VERSION(a,b,c) ( a<<16 | b<<8 | c )

#define CV_UNUSED(name) (void)name


//#define USE_AV_HW_CODECS 0
#ifndef USE_AV_HW_CODECS
#if LIBAVUTIL_VERSION_MAJOR >= 56 // FFMPEG 4.0+
#define USE_AV_HW_CODECS 1
//#include "cap_ffmpeg_hw.hpp"
#else
#define USE_AV_HW_CODECS 0
#endif
#endif

#if LIBAVUTIL_BUILD >= (LIBAVUTIL_VERSION_MICRO >= 100 \
    ? CALC_FFMPEG_VERSION(52, 38, 100) : CALC_FFMPEG_VERSION(52, 13, 0))
#define USE_AV_FRAME_GET_BUFFER 1
#else
#define USE_AV_FRAME_GET_BUFFER 0
#ifndef AV_NUM_DATA_POINTERS // required for 0.7.x/0.8.x ffmpeg releases
#define AV_NUM_DATA_POINTERS 4
#endif
#endif

#ifndef USE_AV_SEND_FRAME_API
// https://github.com/FFmpeg/FFmpeg/commit/7fc329e2dd6226dfecaa4a1d7adf353bf2773726
#if LIBAVCODEC_VERSION_MICRO >= 100 \
    && LIBAVCODEC_BUILD >= CALC_FFMPEG_VERSION(57, 37, 100)
#define USE_AV_SEND_FRAME_API 1
#else
#define USE_AV_SEND_FRAME_API 0
#endif
#endif

// https://github.com/FFmpeg/FFmpeg/blob/b6af56c034759b81985f8ea094e41cbd5f7fecfb/doc/APIchanges#L602-L605
#if LIBAVFORMAT_BUILD < CALC_FFMPEG_VERSION(58, 9, 100)
#  define CV_FFMPEG_REGISTER
#endif

// https://github.com/FFmpeg/FFmpeg/blob/b6af56c034759b81985f8ea094e41cbd5f7fecfb/doc/APIchanges#L654-L657
#if LIBAVCODEC_BUILD < CALC_FFMPEG_VERSION(58, 9, 100)
#  define CV_FFMPEG_LOCKMGR
#endif

// https://github.com/FFmpeg/FFmpeg/blob/b6af56c034759b81985f8ea094e41cbd5f7fecfb/doc/APIchanges#L390-L392
#if LIBAVCODEC_BUILD >= CALC_FFMPEG_VERSION(58, 87, 100)
#include <libavcodec/bsf.h>
#endif

// https://github.com/FFmpeg/FFmpeg/blob/b6af56c034759b81985f8ea094e41cbd5f7fecfb/doc/APIchanges#L208-L210
#if LIBAVFORMAT_BUILD >= CALC_FFMPEG_VERSION(59, 0, 100)
#  define CV_FFMPEG_FMT_CONST const
#else
#  define CV_FFMPEG_FMT_CONST
#endif

// https://github.com/FFmpeg/FFmpeg/blob/b6af56c034759b81985f8ea094e41cbd5f7fecfb/doc/APIchanges#L623-L624
#if LIBAVFORMAT_BUILD >= CALC_FFMPEG_VERSION(58, 7, 100)
#  define CV_FFMPEG_URL
#endif

// AVStream.codec deprecated in favor of AVStream.codecpar
// https://github.com/FFmpeg/FFmpeg/blob/b6af56c034759b81985f8ea094e41cbd5f7fecfb/doc/APIchanges#L1039-L1040
#if LIBAVFORMAT_BUILD >= CALC_FFMPEG_VERSION(59, 16, 100)
//#if LIBAVFORMAT_BUILD >= CALC_FFMPEG_VERSION(57, 33, 100)
#  define CV_FFMPEG_CODECPAR
#  define CV_FFMPEG_CODEC_FIELD codecpar
#else
#  define CV_FFMPEG_CODEC_FIELD codec
#endif

#if LIBAVFORMAT_BUILD >= CALC_FFMPEG_VERSION(59, 16, 100)
#  define CV_FFMPEG_PTS_FIELD pts
#else
#  define CV_FFMPEG_PTS_FIELD pkt_pts
#endif

// https://github.com/FFmpeg/FFmpeg/blob/b6af56c034759b81985f8ea094e41cbd5f7fecfb/doc/APIchanges#L1757-L1758
#if LIBAVUTIL_BUILD < CALC_FFMPEG_VERSION(52, 63, 100)
inline static AVRational av_make_q(int num, int den)
{
	AVRational res;
	res.num = num;
	res.den = den;
	return res;
}
#endif

#if defined _WIN32
#include <windows.h>
#if defined _MSC_VER && _MSC_VER < 1900
struct timespec
{
    time_t tv_sec;
    long   tv_nsec;
};
#endif
#elif defined __linux__ || defined __APPLE__ || defined __HAIKU__
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#if defined __APPLE__
#include <sys/sysctl.h>
#include <mach/clock.h>
#include <mach/mach.h>
#endif
#endif


#ifdef __cplusplus
#  include <cmath>
#else
#  ifdef __BORLANDC__
#    include <fastmath.h>
#  else
#    include <math.h>
#  endif
#endif


#if defined(__CUDACC__)
// nothing, intrinsics/asm code is not supported
#else
#if ((defined _MSC_VER && defined _M_X64) \
      || (defined __GNUC__ && defined __x86_64__ && defined __SSE2__)) \
      && !defined(OPENCV_SKIP_INCLUDE_EMMINTRIN_H)
#include <emmintrin.h>
#endif

#if defined __PPC64__ && defined __GNUC__ && defined _ARCH_PWR8 \
      && !defined(OPENCV_SKIP_INCLUDE_ALTIVEC_H)
#include <altivec.h>
#undef vector
#undef bool
#undef pixel
#endif

#if defined(CV_INLINE_ROUND_FLT)
// user-specified version
// CV_INLINE_ROUND_DBL should be defined too
#elif defined __GNUC__ && defined __arm__ && (defined __ARM_PCS_VFP || defined __ARM_VFPV3__ || defined __ARM_NEON__) && !defined __SOFTFP__
// 1. general scheme
#define ARM_ROUND(_value, _asm_string) \
        int res; \
        float temp; \
        CV_UNUSED(temp); \
        __asm__(_asm_string : [res] "=r" (res), [temp] "=w" (temp) : [value] "w" (_value)); \
        return res
    // 2. version for double
#ifdef __clang__
#define CV_INLINE_ROUND_DBL(value) ARM_ROUND(value, "vcvtr.s32.f64 %[temp], %[value] \n vmov %[res], %[temp]")
#else
#define CV_INLINE_ROUND_DBL(value) ARM_ROUND(value, "vcvtr.s32.f64 %[temp], %P[value] \n vmov %[res], %[temp]")
#endif
// 3. version for float
#define CV_INLINE_ROUND_FLT(value) ARM_ROUND(value, "vcvtr.s32.f32 %[temp], %[value]\n vmov %[res], %[temp]")
#elif defined __PPC64__ && defined __GNUC__ && defined _ARCH_PWR8
// P8 and newer machines can convert fp32/64 to int quickly.
#define CV_INLINE_ROUND_DBL(value) \
        int out; \
        double temp; \
        __asm__( "fctiw %[temp],%[in]\n\tmfvsrwz %[out],%[temp]\n\t" : [out] "=r" (out), [temp] "=d" (temp) : [in] "d" ((double)(value)) : ); \
        return out;

    // FP32 also works with FP64 routine above
#define CV_INLINE_ROUND_FLT(value) CV_INLINE_ROUND_DBL(value)
#endif

#ifdef CV_INLINE_ISINF_FLT
  // user-specified version
  // CV_INLINE_ISINF_DBL should be defined too
#elif defined __PPC64__ && defined _ARCH_PWR9 && defined(scalar_test_data_class)
#define CV_INLINE_ISINF_DBL(value) return scalar_test_data_class(value, 0x30);
#define CV_INLINE_ISINF_FLT(value) CV_INLINE_ISINF_DBL(value)
#endif

#ifdef CV_INLINE_ISNAN_FLT
  // user-specified version
  // CV_INLINE_ISNAN_DBL should be defined too
#elif defined __PPC64__ && defined _ARCH_PWR9 && defined(scalar_test_data_class)
#define CV_INLINE_ISNAN_DBL(value) return scalar_test_data_class(value, 0x40);
#define CV_INLINE_ISNAN_FLT(value) CV_INLINE_ISNAN_DBL(value)
#endif

#if !defined(OPENCV_USE_FASTMATH_BUILTINS) && \
      (defined __GNUC__ || defined __clang__ || defined _MSC_VER)
    /* Let builtin C math functions when available. Dedicated hardware is available to
       round and convert FP values. */
#define OPENCV_USE_FASTMATH_BUILTINS 1
#endif

       /* Enable builtin math functions if possible, desired, and available.
          Note, not all math functions inline equally. E.g lrint will not inline
          without the -fno-math-errno option. */
#if defined(CV_ICC)
          // nothing
#elif defined(OPENCV_USE_FASTMATH_BUILTINS) && OPENCV_USE_FASTMATH_BUILTINS
#if defined(__clang__)
#define CV__FASTMATH_ENABLE_CLANG_MATH_BUILTINS
#if !defined(CV_INLINE_ISNAN_DBL) && __has_builtin(__builtin_isnan)
#define CV_INLINE_ISNAN_DBL(value) return __builtin_isnan(value);
#endif
#if !defined(CV_INLINE_ISNAN_FLT) && __has_builtin(__builtin_isnan)
#define CV_INLINE_ISNAN_FLT(value) return __builtin_isnan(value);
#endif
#if !defined(CV_INLINE_ISINF_DBL) && __has_builtin(__builtin_isinf)
#define CV_INLINE_ISINF_DBL(value) return __builtin_isinf(value);
#endif
#if !defined(CV_INLINE_ISINF_FLT) && __has_builtin(__builtin_isinf)
#define CV_INLINE_ISINF_FLT(value) return __builtin_isinf(value);
#endif
#elif defined(__GNUC__)
#define CV__FASTMATH_ENABLE_GCC_MATH_BUILTINS
#if !defined(CV_INLINE_ISNAN_DBL)
#define CV_INLINE_ISNAN_DBL(value) return __builtin_isnan(value);
#endif
#if !defined(CV_INLINE_ISNAN_FLT)
#define CV_INLINE_ISNAN_FLT(value) return __builtin_isnanf(value);
#endif
#if !defined(CV_INLINE_ISINF_DBL)
#define CV_INLINE_ISINF_DBL(value) return __builtin_isinf(value);
#endif
#if !defined(CV_INLINE_ISINF_FLT)
#define CV_INLINE_ISINF_FLT(value) return __builtin_isinff(value);
#endif
#elif defined(_MSC_VER)
#if !defined(CV_INLINE_ISNAN_DBL)
#define CV_INLINE_ISNAN_DBL(value) return isnan(value);
#endif
#if !defined(CV_INLINE_ISNAN_FLT)
#define CV_INLINE_ISNAN_FLT(value) return isnan(value);
#endif
#if !defined(CV_INLINE_ISINF_DBL)
#define CV_INLINE_ISINF_DBL(value) return isinf(value);
#endif
#if !defined(CV_INLINE_ISINF_FLT)
#define CV_INLINE_ISINF_FLT(value) return isinf(value);
#endif
#endif
#endif

#endif // defined(__CUDACC__)

#if USE_AV_INTERRUPT_CALLBACK

#define LIBAVFORMAT_INTERRUPT_OPEN_DEFAULT_TIMEOUT_MS (3000)

#define LIBAVFORMAT_INTERRUPT_READ_DEFAULT_TIMEOUT_MS (3000)


#define _TEST_DECOCDE_DELAY_ (0)

#define _DECODER_ONE_ (1)

#ifdef _WIN32
// http://stackoverflow.com/questions/5404277/porting-clock-gettime-to-windows

static
inline LARGE_INTEGER get_filetime_offset()
{
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return t;
}

static
inline void get_monotonic_time(timespec* tv)
{
    LARGE_INTEGER           t;
    FILETIME				f;
    double                  microseconds;
    static LARGE_INTEGER    offset;
    static double           frequencyToMicroseconds;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 0;

    if (!initialized)
    {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter)
        {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        }
        else
        {
            offset = get_filetime_offset();
            frequencyToMicroseconds = 10.;
        }
    }

    if (usePerformanceCounter)
    {
        QueryPerformanceCounter(&t);
    }
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = (LONGLONG)microseconds;
    tv->tv_sec = t.QuadPart / 1000000;
    tv->tv_nsec = (t.QuadPart % 1000000) * 1000;
}
#else
static
inline void get_monotonic_time(timespec* time)
{
#if defined(__APPLE__) && defined(__MACH__)
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    time->tv_sec = mts.tv_sec;
    time->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_MONOTONIC, time);
#endif
}
#endif

static
inline timespec get_monotonic_time_diff(timespec start, timespec end)
{
    timespec temp;
    if (end.tv_nsec - start.tv_nsec < 0)
    {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    }
    else
    {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

static
inline double get_monotonic_time_diff_ms(timespec time1, timespec time2)
{
    timespec delta = get_monotonic_time_diff(time1, time2);
    double milliseconds = delta.tv_sec * 1000 + (double)delta.tv_nsec / 1000000.0;

    return milliseconds;
}
#endif // USE_AV_INTERRUPT_CALLBACK

static int get_number_of_cpus(void)
{
#if defined _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    return (int)sysinfo.dwNumberOfProcessors;
#elif defined __linux__ || defined __HAIKU__
    return (int)sysconf(_SC_NPROCESSORS_ONLN);
#elif defined __APPLE__
    int numCPU = 0;
    int mib[4];
    size_t len = sizeof(numCPU);

    // set the mib for hw.ncpu
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    // get the number of CPUs from the system
    sysctl(mib, 2, &numCPU, &len, NULL, 0);

    if (numCPU < 1)
    {
        mib[1] = HW_NCPU;
        sysctl(mib, 2, &numCPU, &len, NULL, 0);

        if (numCPU < 1)
            numCPU = 1;
    }

    return (int)numCPU;
#else
    return 1;
#endif
    return 0;
}

namespace chen {




	extern std::mutex g_ffmpeg_lock;


	struct AVInterruptCallbackMetadata
	{
		timespec value;
		uint32_t timeout_after_ms;
		int32_t timeout;
        AVInterruptCallbackMetadata()
            : value()
            , timeout_after_ms(0)
            , timeout(0) {}
	};
    struct cimage_ffmpeg
    {
        unsigned char* data;
        int32_t step;
        int32_t width;
        int32_t height;
        int32_t cn;
        cimage_ffmpeg()
            : data(NULL)
            , step(0)
            , width(0)
            , height(0)
            , cn(0){}
    };
	namespace ffmpeg_util
	{
            const char* make_error_string(int err_code);
        static   inline int ffmpeg_interrupt_callback(void* ptr)
        {
            AVInterruptCallbackMetadata* metadata = (AVInterruptCallbackMetadata*)ptr;
            assert(metadata);

            if (metadata->timeout_after_ms == 0)
            {
                return 0; // timeout is disabled
            }

            timespec now;
            get_monotonic_time(&now);

            metadata->timeout = get_monotonic_time_diff_ms(metadata->value, now) > metadata->timeout_after_ms;

            return metadata->timeout ? -1 : 0;
        }

        /** @brief Rounds floating-point number to the nearest integer

 @param value floating-point number. If the value is outside of INT_MIN ... INT_MAX range, the
 result is not defined.
 */
          inline int round(double value);
	}
   
    
}
#endif // _C_FFMPEG_UTIL_H_
