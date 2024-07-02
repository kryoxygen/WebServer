#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void print_id(int id)
{
	std::unique_lock<std::mutex> lck(mtx);
	if (lck.owns_lock())
	{
		std::cout << "true1" << '\n';
	}
	std::cout << "id: " << id << std::endl;
	while (!ready)
		cv.wait(lck); // 等待条件变量，释放互斥锁

	if (lck.owns_lock())
	{
		std::cout << "true" << '\n';
	}

	std::cout << "Thread " << id << '\n';
}

void go()
{
	std::unique_lock<std::mutex> lck(mtx);
	ready = true;
	cv.notify_all(); // 通知所有等待线程
	std::cout << "Threads ready!\n"
			  << std::endl;
}

int main()
{
	std::thread threads[10];
	for (int i = 0; i < 10; ++i)
		threads[i] = std::thread(print_id, i);

	std::cout << "10 threads ready to race...\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	// go();  // 改变条件并通知所有等待线程

	for (auto &th : threads)
		th.join();

	return 0;
}
