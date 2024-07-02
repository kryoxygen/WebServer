#pragma once

#include <print>
#include<memory>
#include <pthread.h>
#include <sys/time.h>
#include "locker.hpp"

template <typename T>
class block_queue
{
public:
	block_queue(int max_size = 1000)
	{
		if (max_size <= 0)
		{
			std::println("max_size must > 0");
			exit(-1);
		}

		m_max_size = max_size;
		m_array = std::make_unique<T>(max_size);
		m_size = 0;
		m_front = -1;
		m_back = -1;
	}

	void clear()
	{
		m_locker.lock();
		m_size = 0;
		m_front = -1;
		m_back = -1;
		m_locker.unlock();
	}

	// ~block_queue()
	// {
	// 	m_locker.lock();
	// }

	bool is_full()
	{
		m_locker.lock();
		if (m_size == m_max_size)
		{
			return true;
		}
		m_locker.unlock();
		return false;
	}

	bool is_empty()
	{
		m_locker.lock();
		if (m_size== 0)
		{
			return true;
		}
		m_locker.unlock();
		return false;
	}

	bool front_value(T &value)
	{
		m_locker.lock();
		if (m_size == 0)
		{
			m_locker.unlock();
			return false;
		}
		value = m_array[m_front];
		m_locker.unlock();
		return true;
	}

	bool back_value(T &value)
	{
		m_locker.lock();
		if (m_size == 0)
		{
			m_locker.unlock();
			return false;
		}
		value = m_array[m_back];
		m_locker.unlock();
		return true;
	}

	int size()
	{
		auto temp = 0;
		m_locker.lock();
		temp = m_size;
		m_locker.unlock();
		return temp;
	}

	int max_size()
	{
		auto temp = 0;
		m_locker.lock();
		temp = m_max_size;
		m_locker.unlock();
		return temp;
	}

	bool push(const T &value)
	{
		m_locker.lock();
		if (m_size >= m_max_size)
		{
			m_cond.boardcast();
			m_locker.unlock();
			return false;
		}

		//? 为什么要%m_max_size
		m_back = (m_back+1)%m_max_size;
		m_array[m_back] = item;
		m_size++;
		m_cond.boardcast();
		m_locker.unlock();
		return true;
	}

	bool pop(T &value)
	{
		m_locker.lock();
		while(m_size<=0)
		{
			if (!m_cond.wait(m_locker.get()))
			{
				return false;
			}
		}
		m_front = (m_front+1)%m_max_size;
		value = m_array[m_front];
		m_size--;
		m_locker.unlock();
		return true;
	}

	//TODO: 增加超时处理
private:
	locker m_locker;
	condition m_cond;
	std::unique_ptr<T[]> m_array;
	int m_size;
	int m_max_size;
	int m_front;
	int m_back;
};