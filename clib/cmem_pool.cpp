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
#include "cmem_pool.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include "clog.h"
namespace chen {


	namespace cmem_util_pool
	{
#pragma  pack(push, 1)
		struct cmem_block;
		//ï¿½Úµï¿½ï¿½ï¿½Ï¢
		struct cmem_node
		{
			cmem_block		*ptr;
			unsigned int	m_size;
            int	            m_offset;
			char			m_buf[1];
		};
#pragma	 pack(pop)


#pragma pack(push, 1)
		struct cmem_block
		{
		public:
			cmem_block();
			static cmem_block * create_me(unsigned int pool_index, unsigned int block_total, unsigned int node_size);
			static void			release_me(cmem_block * ptr);

			cmem_node * tack_node(bool & flag);
			void		return_node(cmem_node *ptr);
			bool		empty() const { return head_tack_node == NULL; }
			bool		not_used() const {	return m_use_size == 0;	}
			cmem_block *	pre;
			cmem_block *	next;

			cmem_node *		head_tack_node;

			unsigned int		m_pool_index;
			int					m_use_size;
			int					m_free_quety;
			char				buf[1];
		};

		cmem_block::cmem_block()
			: pre(NULL)
			, next(NULL)
			, head_tack_node(NULL)
			, m_use_size(0)
			, m_free_quety(0)
			/*, m_node_size(0)*/
		{
		}

		
		cmem_block * cmem_block::create_me(unsigned int pool_index, unsigned int block_total, unsigned int node_size)
		{ // 32768   32
			const unsigned int alloc_size = sizeof(cmem_block) - 1 + (block_total *node_size);
			cmem_block * block_ptr = static_cast<cmem_block*>(::malloc(alloc_size));
			if (!block_ptr)
			{
				ERROR_EX_LOG("alloc cmem_blcok fail\n");
				return NULL;
			}
			block_ptr->pre = NULL;
			block_ptr->next = NULL;
			block_ptr->m_pool_index = pool_index;
			block_ptr->head_tack_node = (cmem_node *)(block_ptr->buf);
			block_ptr->m_free_quety = 0;
			block_ptr->m_use_size = 0;
			unsigned int offset = 0;
			cmem_node * node_ptr = NULL;
			//printf("[block_ptr->buf = %p]\n", block_ptr->buf);
			for (unsigned int i = 0; i < block_total; ++i)
			{
				node_ptr = (cmem_node*)((char *)block_ptr->buf + offset);
			//	printf("[i = %d][node_ptr = %p]\n", i, node_ptr);
				offset += node_size;
				node_ptr->m_offset = offset;
				node_ptr->ptr = block_ptr;
				node_ptr->m_size = node_size;
			}
			//printf("[ptr = %p]\n", node_ptr);
			node_ptr->m_offset = -1;
			return block_ptr;
		}

		void cmem_block::release_me(cmem_block * ptr)
		{
			::free(ptr);
		}

		cmem_node * cmem_block::tack_node(bool & flag)
		{
			assert(head_tack_node);
			cmem_node * node_ptr = head_tack_node;
			if (head_tack_node->m_offset != -1)
			{
				head_tack_node = (cmem_node *)(buf + node_ptr->m_offset);
				node_ptr->m_offset = -1;
			}
			else
			{
				head_tack_node = NULL;
			}
			++m_use_size;
			//printf("[mem_block use = %d]\n", m_use_size);
			++m_free_quety;
			return node_ptr;
		}

		void cmem_block::return_node(cmem_node * ptr)
		{
			//assert(head_tack_node);
			if (head_tack_node )
			{
				ptr->m_offset = static_cast<int>((char*)head_tack_node - buf);
			}
			head_tack_node = ptr;
			--m_use_size;
			
		}
#pragma pack(pop)

		class cblock_pool
		{
		public:
			cblock_pool();
			bool init(unsigned int pool_index, unsigned int block_total, unsigned int node_size);
			cmem_node*   tack_node(bool &flag);
			void		return_node(cmem_node* ptr);
			void		move_front_node(cmem_block *p);
			cmem_block * get_release_back();
			void		 release_block(cmem_block* ptr);
			unsigned int gc();
		public:
			unsigned int	get_node_size() const { return m_node_size; }
			unsigned int	get_pool_index() const { return m_pool_index; }
			bool			empty() const { return m_head == NULL; }
		private:
			void	_remove_block(cmem_block * ptr);
		private:
			cmem_block *		m_head;
			cmem_block *		m_tail;
			unsigned	int		m_pool_index;  // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½id
			unsigned	int		m_block_total; //ï¿½Úµï¿½Ä¸ï¿½ï¿½ï¿½
			unsigned	int		m_node_size; // ï¿½Úµï¿½Ä´ï¿½Ð¡
		};
		cblock_pool::cblock_pool()
			: m_head(NULL)
			, m_tail(NULL)
			, m_pool_index(0)
			, m_block_total(0)
			, m_node_size(0)
		{
		}


		bool cblock_pool::init(unsigned int pool_index, unsigned int block_total, unsigned int node_size)
		{
			m_pool_index = pool_index;
			m_block_total = block_total;
			m_node_size = node_size;
			return true;
		}
		cmem_node * cblock_pool::tack_node(bool &flag)
		{
			if (m_head)
			{
				cmem_node* node_ptr = m_head->tack_node(flag);
				cmem_block* block_ptr = node_ptr->ptr;
				if ( block_ptr->empty())
				{
					
					if (m_head->next)
					{
						m_head = m_head->next;
						m_head->pre = NULL;
						block_ptr->next = NULL;
					}
					else
					{
						m_head = NULL;
						m_tail = NULL;
					}
				}
				return node_ptr;
			}
			
			cmem_block* block_ptr = cmem_block::create_me(m_pool_index, m_block_total, m_node_size);
			if (!block_ptr)
			{
				ERROR_EX_LOG("create_me alloc fail ");
				return NULL;
			}
			m_head = block_ptr;
			m_tail = block_ptr;
			flag = true;
			return block_ptr->tack_node(flag);
		}
		void cblock_pool::return_node(cmem_node * ptr)
		{
			cmem_block * block_ptr = ptr->ptr;
			bool block_empty = block_ptr->empty();
			block_ptr->return_node(ptr);
			
			if (block_empty && !block_ptr->empty())
			{
				if (m_head)
				{ //ï¿½ï¿½ï¿½ï¿½Ä©Î²
					m_tail->next = block_ptr;
				//	m_tail->next->pre = m_tail; 
					block_ptr->pre = m_tail;
					block_ptr->next = NULL;
					m_tail = block_ptr;
				}
				else
				{
					m_head = block_ptr;
					m_tail = block_ptr;
					block_ptr->next = NULL;
					block_ptr->pre = NULL;
				}
			}
		}

		void cblock_pool::move_front_node(cmem_block * p)
		{
			cmem_block * block_ptr = NULL;
			if (m_head == p)
			{
				return;
			}
			if (m_head->next)
			{
				block_ptr = m_head->next;
				if (block_ptr == p)
				{
					return;
				}
				//ERROR_EX_LOG("move_front_block");
				_remove_block(p);
				block_ptr->pre = p;
				p->next = block_ptr;
				m_head->next = p;
				p->pre = m_head;
				return;
			}
		}

		cmem_block * cblock_pool::get_release_back()
		{
			cmem_block * block_ptr = NULL;
			for (cmem_block * p = m_head; p != NULL ; p = p->next)			
			{
				if (p->not_used())
				{
					if (p->m_free_quety == 0)
					{
						return p;
					}
					if (!block_ptr ||  /*p->empty() ||*/ block_ptr->m_free_quety > p->m_free_quety)
					{
						block_ptr = p;
					}
				}
			}
			return block_ptr;
		}

		void cblock_pool::release_block(cmem_block * ptr)
		{
			_remove_block(ptr);
			cmem_block::release_me(ptr);
		}

		unsigned int cblock_pool::gc()
		{
			unsigned int count = 0;
			cmem_block * block_ptr = NULL;
			//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð·ï¿½ï¿½Õµï¿½Òª×¢ï¿½ï¿½ï¿½ for ï¿½ï¿½É¾ï¿½ï¿½ï¿½Úµï¿½ï¿½ï¿½ï¿½ï¿½
			for (cmem_block *p = m_head; p != NULL; /*p = p->next*/)
			{
				block_ptr = p->next;
				if (p->not_used())
				{
					++count;
					release_block(p);
				}
				p = block_ptr;
			}
			return count;
		}

		void cblock_pool::_remove_block(cmem_block * ptr)
		{
			cmem_block * block_ptr = ptr->pre;
			if (m_head == ptr)
			{
				m_head = ptr->next;
				ptr->pre = NULL;
				/*
				ï¿½ï¿½ï¿½Ô¶ï¿½ï¿½Í·ï¿½ï¿½Ú´ï¿½Ê± Ë«ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
				if (m_head->next)
				{
					block_ptr = m_head->next;
					m_head->next = NULL;
					block_ptr->pre = NULL;
					m_head = block_ptr;
				}
				else
				{
					m_head == NULL;
					m_tail == NULL;
				}*/
				return;
			}
			if (m_tail == ptr)
			{
				block_ptr = m_tail->pre;
				block_ptr->next = NULL;
				m_tail = block_ptr;
				return;
			}
			if (ptr->pre)
			{
				block_ptr->next = ptr->next;
			}
			if (ptr->next)
			{
				ptr->next->pre = block_ptr;
			}
			
			//if (m_head->next == ptr)
			//{
			//	//ptr->pre = NULL;
			//	m_head->next = ptr->next;
			//	//ptr->next->pre = m_head;
			//	ptr->next = NULL;
			//	//return;
			//}
			//if (m_tail == ptr)
			//{
			//	block_ptr = m_tail->pre;
			//	block_ptr->next = NULL;
			//	m_tail = block_ptr;
			//	return;
			//}

			//block_ptr = ptr->next;
			//ptr->pre->next = block_ptr;
			//block_ptr->pre = ptr->pre;
			//ptr->next = NULL;
			//ptr->pre = NULL;
			/*if (ptr->next)
			{
				block_ptr = ptr->next;
				ptr->next->pre = block_ptr;
			}
			if (ptr->pre)
			{
				block_ptr = ptr->pre;
				ptr->pre->next = block_ptr;
			}*/
		}
	}


	using cmem_util_pool::cmem_node;
	using cmem_util_pool::cmem_block;

	cmem_pool::cmem_pool(bool safe)
		: m_pool(NULL)
		, m_max_block_size(0)
		, m_block_size(0)
		, m_start_size(0)
		, m_pool_size(0)
		, m_current_size(0)
		, m_use_size(0)
		, m_free_size(0)
		, m_safe(safe)

	{
	}
	bool cmem_pool::init(unsigned int max_block, unsigned int block_size, unsigned int start_size)
	{
		if (max_block < 10)
		{
			ERROR_EX_LOG("max block tail small");
			return false;
		}
		if (block_size % 32 != 0)
		{
			ERROR_EX_LOG("block_size error");
			return false;
		}

		if (start_size % 32 != 0)
		{
			ERROR_EX_LOG("start_size error");
			return false;
		}

		unsigned int node_size = start_size;
		while (node_size != block_size)
		{
			++m_pool_size;
			node_size *= 2;
		}

		m_pool = new cmem_util_pool::cblock_pool[m_pool_size];
		if (!m_pool)
		{
			ERROR_EX_LOG("alloc block pool fail");
			return false;
		}
		node_size = start_size;
		for (unsigned int i = 0; i < m_pool_size; ++i)
		{
			m_pool[i].init(i, block_size / node_size, node_size);
			node_size *= 2;
		}
		m_max_block_size = max_block;
		m_block_size = block_size;
		m_start_size = start_size;
		return true;
	}
	void * cmem_pool::alloc(unsigned int bytes)
	{
		unsigned int realsize = sizeof(cmem_node) - 1 + bytes;
		int index = _sizeindex(realsize);
		
		if (-1 != index)
		{
			
			if (m_safe)
			{
				m_lock.lock();
			}
			bool flag = false;
			//printf("[use index = %d]\n", index);
			
		    cmem_node* node_ptr = m_pool[index].tack_node(flag);
			if (!node_ptr)
			{
				ERROR_EX_LOG("mem_pool alloc fail ");
				if (m_safe)
				{
					m_lock.unlock();
				}
				return NULL;
			}
			if (flag)
			{
				m_current_size += m_block_size;
				++m_block_count;
			}
			else if (1 == node_ptr->ptr->m_use_size)
			{
				//ERROR_EX_LOG("take_nod m_free_size = %d", m_free_size);
				--m_free_size;
				//ERROR_EX_LOG("take_nod m_free_size = %d", m_free_size);
			}
			m_current_size -= realsize;
			m_use_size += realsize;
			if (m_safe)
			{
				m_lock.unlock();
			}
			return node_ptr->m_buf;
		}

		cmem_node* node_ptr = static_cast<cmem_node*>(::malloc(realsize));
		if (!node_ptr)
		{
			ERROR_EX_LOG("mem_pool malloc fail ");
			if (m_safe)
			{
				m_lock.unlock();
			}
			return NULL;
		}
		node_ptr->ptr = NULL;
		node_ptr->m_offset = -1;
		node_ptr->m_size = realsize;
		m_current_size += realsize;
		m_use_size += realsize;
		
		return node_ptr->m_buf;
	}
	void cmem_pool::free(void * const ptr)
	{
		cmem_node * node_ptr = reinterpret_cast<cmem_node*>((char*)ptr - sizeof(cmem_node) + 1);
		if (node_ptr->ptr)
		{
			if (m_safe)
			{
				m_lock.lock();
			}

			if (m_block_count > m_max_block_size && m_free_size > 0)
			{
				//ERROR_EX_LOG("take_nod m_free_size = %d", m_free_size);
				_gc_one();
			}

			cmem_block *block_ptr = node_ptr->ptr;
			m_pool[block_ptr->m_pool_index].return_node(node_ptr);

			if (block_ptr->not_used())
			{
				m_pool[block_ptr->m_pool_index].move_front_node(block_ptr);
				++m_free_size;
			}
			m_current_size += node_ptr->m_size;
			m_use_size -= node_ptr->m_size;
			if (m_safe)
			{
				m_lock.unlock();
			}
			return;
		}

		::free(node_ptr);
	}
	void cmem_pool::set_max_block(unsigned int block_size)
	{
		m_max_block_size = block_size;
	}
	void cmem_pool::gc()
	{
		if (m_safe)
		{
			m_lock.lock();
		}
		unsigned int count = 0;
		for (unsigned int i = 0; i < m_pool_size; ++i )
		{
			ERROR_EX_LOG("gc i = %d", i);
			if (m_pool[i].empty())
			{
				continue;
			}
			count = m_pool[i].gc();
			m_current_size -= (count * m_block_size);
			m_block_count -= count;
		}

		if (m_safe)
		{
			m_lock.unlock();
		}
	}
	void cmem_pool::destroy()
	{
		if (m_pool)
		{
			delete[] m_pool;
			m_pool = NULL;
		}
	}
	void cmem_pool::show_info() const
	{
		SYSTEM_LOG("[alloc size      = %d]", get_current_size());
		SYSTEM_LOG("[use size        = %d]", get_use_size());
		SYSTEM_LOG("[block total     = %d]", get_block_count());
		SYSTEM_LOG("[free block size = %d]", get_free_size());
	}
	unsigned int cmem_pool::get_current_size() const
	{
		return m_current_size;
	}
	unsigned int cmem_pool::get_use_size() const
	{
		return m_use_size;
	}
	unsigned int cmem_pool::get_block_count() const
	{
		return m_block_count;
	}
	unsigned int cmem_pool::get_free_size() const
	{
		return m_free_size;
	}
	int cmem_pool::_sizeindex(unsigned int& realsize)
	{
		for (unsigned int i = 0; i < m_pool_size; ++i)
		{
			if (realsize <= m_pool[i].get_node_size())
			{
				realsize = m_pool[i].get_node_size();
				return i;
			}
		}
		return -1;
	}
	void cmem_pool::_gc_one()
	{
		cmem_block * p = NULL;
		for (int i = m_pool_size - 1; i >= 0; --i)
		{
			if (m_pool[i].empty())
			{
				continue;
			}
			p = m_pool[i].get_release_back();
			if (p)
			{
				break;
			}
			//	/*p->not_used()*/
			//	/*cmem_block::release_me(p);*/
			//	m_pool[i].release_block(p);
			//}
			//if (m_pool[i].get_node_size())
		}
		//printf("=======================\n");
		if (p)
		{
			m_current_size -= m_block_size;
			--m_block_count;
			--m_free_size;
			m_pool[p->m_pool_index].release_block(p);
			//SYSTEM_LOG("m_free_size = %d", m_free_size);
			//ERROR_EX_LOG("take_nod m_free_size = %d", m_free_size);
			//show_info();
		}
	}
} // namespace chen



