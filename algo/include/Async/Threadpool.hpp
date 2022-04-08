//
// Created by Peer De bakker on 4/7/22.
//

#ifndef GOMOKUBOT_THREADPOOL_HPP
#define GOMOKUBOT_THREADPOOL_HPP
#include <thread>
#include <utility>
#include "AsyncQueue.hpp"
#include <vector>
#include <atomic>

// Singleton FTW
class Threadpool {
	const unsigned int			_numThreads;
	std::atomic<int>			busyWorkers;
	std::vector<std::thread>	_threads;
	AsyncQueue<Job>&			_jobQueue;
	AsyncQueue<Gamestate*>&		_outputQueue;

	void	launch_worker();
	explicit Threadpool(unsigned int numThreads = std::thread::hardware_concurrency());
public:
	Threadpool(const Threadpool& x) = delete;
	Threadpool& operator=(const Threadpool& x) = delete;
	~Threadpool();

	static Threadpool& GetInstance() {
		static Threadpool threadpool;

		return threadpool;
	}

	void WaitForWorkers() const;
};


#endif //GOMOKUBOT_THREADPOOL_HPP
