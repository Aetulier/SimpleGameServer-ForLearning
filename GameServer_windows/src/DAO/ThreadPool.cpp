#include"ThreadPool.h"


ThreadPool::ThreadPool(size_t threadNum) : stop_(false) {
	for (size_t i = 0; i < threadNum; ++i) {
		workers_.emplace_back([this]() {
			for (;;) {
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> ul(mtx_);
					cv_.wait(ul, [this]() { return stop_ || !tasks_.empty(); });
					if (stop_ && tasks_.empty()) { return; }
					task = std::move(tasks_.front());
					tasks_.pop();
				}
				task();
			}
		});
	}
}

ThreadPool::~ThreadPool() {
{
	std::unique_lock<std::mutex> ul(mtx_);
	stop_ = true;
}
	cv_.notify_all();
	for (auto& worker : workers_) {
		worker.join();
	}
}