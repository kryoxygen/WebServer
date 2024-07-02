#include <print>
#include <string>
#include <pthread.h>
#include "locker.hpp"
#include "block_queue.hpp"

class Log
{
public:
	static Log *Instance()
	{
		Log *log = new Log();
	}

	static void *flush_log_thread(void *args)
	{
		// Log::Instance()->async_write_log();
	}
	// 初始化日志
	bool init(const char *file_name, int close_log, int log_buf = 8192, int spilt_lines = 5000000, int max_queue = 0);
	// 写日志
	void write_log(int level, const char *format, ...);
	// 刷新日志
	void flush(void);

private:
	Log();
	~Log();
	void *flush_log_thread();

private:
	char dir_name[128]; // 路径名
	char log_name[128]; // log文件名
	int m_split_lines;	// 日志最大行数
	int m_log_buf_size; // 日志缓冲区大小
	long long m_count;	// 日志行数记录
	int m_today;		// 因为按天分类,记录当前时间是那一天
	FILE *m_fp;			// 打开log的文件指针
	char *m_buf;
	block_queue<std::string> *m_log_queue; // 阻塞队列
	bool m_is_async;					   // 是否同步标志位
	locker m_mutex;
	int m_close_log; // 关闭日志
};

#define LOG_DEBUG(format, ...)                                \
	if (0 == m_close_log)                                     \
	{                                                         \
		Log::Instance()->write_log(0, format, ##__VA_ARGS__); \
	}
#define LOG_INFO(format, ...)                                 \
	if (0 == m_close_log)                                     \
	{                                                         \
		Log::Instance()->write_log(1, format, ##__VA_ARGS__); \
	}
#define LOG_WARN(format, ...)                                 \
	if (0 == m_close_log)                                     \
	{                                                         \
		Log::Instance()->write_log(2, format, ##__VA_ARGS__); \
	}