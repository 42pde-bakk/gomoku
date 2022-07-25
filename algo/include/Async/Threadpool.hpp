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
	const unsigned int _numThreads;
	unsigned int _busyWorkers;
	mutable std::mutex _m;
	mutable std::mutex _parent_mutex;
	std::vector<std::thread> _threads;
	AsyncQueue<Job> &_jobQueue;

	void launch_worker();

	bool hasTasks() const;

	explicit Threadpool(unsigned int numThreads = std::thread::hardware_concurrency());

public:
	Threadpool(const Threadpool &x) = delete;

	Threadpool &operator=(const Threadpool &x) = delete;

	~Threadpool();

	static Threadpool &GetInstance();

	void WaitForWorkers() const;

	void stop_threads();
};


#endif //GOMOKUBOT_THREADPOOL_HPP
