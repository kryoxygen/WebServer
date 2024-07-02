#include <exception>
#include <print>
// 线程库
#include <pthread.h>
// 信号量
#include <semaphore.h>

class semphore
{
public:
	semphore();
	semphore(int num);
	~semphore();

public:
	bool wait()
	{
		return sem_wait(&m_sem);
	}
	bool post()
	{
		return sem_post(&m_sem);
	}

private:
	sem_t m_sem;
};
semphore::semphore()
{
	// sem 是指向信号量的指针。
	// pshared 表示信号量是否可以在进程间共享，如果是，则设置为非零值，否则设置为零。
	// value 表示信号量的初始值。
	if (sem_init(&m_sem, 0, 0) != 0)
	{
		throw std::exception();
	}
};
semphore::semphore(int num)
{
	if (sem_init(&m_sem, 0, num) != 0)
	{
		throw std::exception();
	}
}
semphore::~semphore()
{
	sem_destroy(&m_sem);
}

class locker
{
	locker()
	{
		if (pthread_mutex_init(&m_mutex, NULL) != 0)
		{
			throw std::exception();
		}
	}
	~locker()
	{
		pthread_mutex_destroy(&m_mutex);
	}

	bool lock()
	{
		return pthread_mutex_lock(&m_mutex);
	}
	bool unlock()
	{
		return pthread_mutex_unlock(&m_mutex);
	}
	pthread_mutex_t &get()
	{
		return m_mutex;
	}

private:
	pthread_mutex_t m_mutex;
};

class condition
{
public:
	condition()
	{
		if (pthread_cond_init(&m_cond, NULL) != 0)
		{
			throw std::exception();
		}
	}
	~condition()
	{
		pthread_cond_destroy(&m_cond);
	}

	// 等待条件变量
	bool wait(pthread_mutex_t *mutex)
	{
		int ret = 0;
		ret = pthread_cond_wait(&m_cond, mutex);
		return ret == 0;
	}

	// 等待条件变量超时
	bool timewait(pthread_mutex_t *mutex, struct timespec t)
	{
		int ret = 0;
		ret = pthread_cond_timedwait(&m_cond, mutex, &t);
		return ret == 0;
	}
	// 唤醒一个等待该条件变量的线程
	bool singal()
	{
		return pthread_cond_signal(&m_cond) == 0;
	}

	// 唤醒所有等待该条件变量的线程
	bool boardcast()
	{
		return pthread_cond_broadcast(&m_cond) == 0;
	}

private:
	pthread_cond_t m_cond;
};