#include <time.h>
#include <sys/time.h>
#include "log.h"
#include <cassert>

bool Log::init(const char *file_name, int close_log, int log_buf, int spilt_lines, int max_queue)
{
	if (max_queue > 0)
	{
		m_is_async = true;
		m_log_queue = new block_queue<std::string>(max_queue);
		pthread_t tid;
		pthread_create(&tid, nullptr, flush_log_thread, nullptr);
	}

	return false;
}

void Log::write_log(int level, const char *format, ...)
{
}

void Log::flush(void)
{
}

void *Log::flush_log_thread()
{
	return nullptr;
}
