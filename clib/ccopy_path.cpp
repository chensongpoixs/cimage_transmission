/********************************************************************
created:	2019-05-13

author:		chensong

purpose:	×Ö·û´®¶ÁÈ¡¹¤¾ß
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
#include "ccopy_path.h"
#include <boost/filesystem.hpp>
#include "clog.h"
namespace chen {

    /*
    boost::filesystem接口总结
boost::filesystem::status(path)                                       查询文件或目录的状态，返回的是        boost::filesystem::file_status类型的对象
boost::filesystem::is_directory()                                    根据获取的状态判断是否是目录，返回bool
boost::filesystem::is_empty()                                        判断是否为空
boost::filesystem::is_regular_file()                                 根据获取的状态判断是否是普通文件，返回bool
boost::filesystem::is_symlink()                                      判断符号连接（在windows系统中，后缀为lnk的文件为连接文件）
boost::filesystem::exists()                                              判断是否存在
boost::filesystem::file_size()                                          返回文件的size，按bytes计算
boost::filesystem::last_write_time()                               返回文件最后一次修改的时间
boost::filesystem::space()                                              返回磁盘的总空间和剩余空间，
boost::filesystem::create_directory()                             创建目录
boost::filesystem::create_directories()                           递归创建整个目录结构
boost::filesystem::remove()                                           删除目录
boost::filesystem::remove_all()                                     递归删除整个目录结构
boost::filesystem::rename()                                           重命名目录
boost::filesystem::copy_file()                                         复制文件
boost::filesystem::copy_directory()                               复制目录
boost::filesystem::absolute()                                           获取文件或目录的绝对路径
boost::filesystem::current_path()                                   如果没有参数传入，则返回当前工作目录；否则，则将传入的目录设为当前工作目录
boost::filesystem::directory_iterator()                            迭代目录下的所有文件
boost::filesystem::recursive_directory_iterator()            递归地遍历一个目录和子目录，也就是迭代整个目录结构下的
 
    
    */
	namespace copy_path
	{
        bool create_directory(const std::string& path)
        {
            //boost::filesystem::path file_path(path);
            //if(boost::filesystem::exists(path)) 
            //{ // 如果路径已存在则不进行操作
            //    return true;
            //}
            // else 
            //{
            //     try {
            //         //boost::filesystem::create_directory(path); // 创建单个目录
            //         //std::cout << "成功创建目录：" << path << std::endl;
            //         boost::filesystem::recursive_directory_iterator end;
            //         // 若需要递归创建多级子目录，可以调用该函数来处理每个子目录
            //         for (boost::filesystem::recursive_directory_iterator  itr = boost::filesystem::recursive_directory_iterator(path) ; itr != end(); ++itr)
            //         {
            //             auto subPath = itr->path().string();

            //             if (!boost::filesystem::is_regular_file(subPath)) 
            //             { // 只对非文件类型（包括目录）进行操作
            //                 create_directory(subPath);
            //             }
            //         }
            //     }
            //     catch (std::exception const& e) 
            //     {
            //         WARNING_EX_LOG("create dir [%s]failed !!!", e.what());
            //         std::cerr << e.what() << std::endl;
            //     }
            //}

            return true;
        }
        bool copy_dir(const std::string& source, const std::string& destination)
		{
            //namespace fs = boost::filesystem;
            boost::filesystem::path source_path(source);
            
            boost::filesystem::path destination_path(destination);

            try
            {
                // Check whether the function call is valid
                if (
                    !boost::filesystem::exists(source) ||
                    !boost::filesystem::is_directory(source)
                    )
                {
                    /*std::cerr << "Source directory " << source.string()
                        << " does not exist or is not a directory." << '
                        '
                        ;*/
                    WARNING_EX_LOG("source directory = [%s] does not exist or is not a directory !!!", source.c_str());
                    return false;
                }
                if (boost::filesystem::exists(destination_path))
                {
                    /*std::cerr << "Destination directory " << destination.string()
                        << " already exists." << '
                        '
                        ;*/
                    boost::filesystem::path new_destination_path(destination + std::to_string(time(NULL)) + ".bak");
                   // boost::filesystem::rename(destination_path, new_destination_path);
                    WARNING_EX_LOG("rename --> old  directory = [%s] [new director = %s]already exists !!!", destination.c_str(), new_destination_path.string().c_str());
                   
                    return false;
                }
                // Create the destination directorycreate_directories(destination);
                if (!boost::filesystem::create_directories(destination_path))
                {
                    /*std::cerr << "Unable to create destination directory"
                        << destination.string() << '
                        '
                        ;*/
                    WARNING_EX_LOG("Destination directory = [%s] Unable to create destination directory !!!", destination.c_str());

                    return false;
                }
            }
            catch (boost::filesystem::filesystem_error const& e)
            {
                /*std::cerr << e.what() << '
                    ';*/
                WARNING_EX_LOG("source directory = [%s]  [error = %s] !!!", source.c_str(), e.what());

                    return false;
            }

            /*boost::system::error_code code;
            boost::filesystem::copy_directory(source_path, destination_path, code);
            if (boost::system::errc::success != code.value())
            {
                WARNING_EX_LOG("copy dir [srouce dir = %s][des dir = %s] failed !!!", source.c_str(), destination.c_str());
                return false;
            }
            return true;*/
            // Iterate through the source directory
            for (boost::filesystem::directory_iterator file(source_path); file != boost::filesystem::directory_iterator(); ++file )
            {
                try
                {
                    boost::filesystem::path current(file->path());
                    if (boost::filesystem::is_directory(current))
                    {
                        // Found directory: Recursion
                        if (
                            !copy_dir(
                                current.string(),
                                boost::filesystem::path(destination_path / current.filename()).string()
                            )
                            )
                        {
                            return false;
                        }
                    }
                    else
                    {
                        // Found file: Copy
                        boost::filesystem::copy_file(
                            current,
                            destination / current.filename()
                        );
                    }
                }
                catch (boost::filesystem::filesystem_error const& e)
                {
                   // std::cerr << e.what() << '
                   //     ';
                    WARNING_EX_LOG("[file copy error = %s]", e.what());
                }
            }
            return true;
		}

        bool remove_dir(const std::string& path)
        {
            try {
                if (boost::filesystem::exists(path)) 
                { // 判断文件夹是否存在
                    boost::filesystem::remove_all(path); // 删除文件夹及其所有内容
                   // std::cout << "成功删除文件夹" << std::endl;
                }
                else
                {
                    WARNING_EX_LOG("not find dir [%s] !!!", path.c_str());
                    //std::cerr << "文件夹不存在！" << std::endl;
                }
            }
            catch (const boost::filesystem::filesystem_error& e) 
            {
                return false;
               // std::cerr << "发生错误：" << e.what() << std::endl;
            }
            return true;
        }
        

        bool dir_all_dir_name(const std::string& dir, std::vector<std::string>& dir_names)
        {
            boost::filesystem::path path(dir);
            if (!boost::filesystem::exists(path))
            {
                return false;
            }

            boost::filesystem::directory_iterator end_iter;
            for (boost::filesystem::directory_iterator iter(path); iter != end_iter; ++iter)
            {
               /* if (boost::filesystem::is_regular_file(iter->status()))
                {
                    filenames.push_back(iter->path().string());
                }*/

                if (boost::filesystem::is_directory(iter->status()))
                {
                     
                    dir_names.push_back(iter->path().filename().string());
                   // get_filenames(iter->path().string(), filenames);
                }
            }

            return dir_names.size();
            
        }
	}
}