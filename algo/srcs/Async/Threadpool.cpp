//
// Created by Peer De bakker on 4/7/22.
//

#include "Threadpool.hpp"
#include "JobQueue.hpp"
#include "Gomoku.hpp"
#include <unistd.h>

#if THREADED

Threadpool::Threadpool(unsigned int numThreads)
	: _numThreads(numThreads), _busyWorkers(numThreads), _m(), _parent_mutex(), _threads(), _jobQueue(getJobQueue())
{
	this->_threads.reserve(this->_numThreads);
	for (unsigned int i = 0; i < _numThreads; i++) {
		this->_threads.emplace_back( [this] { launch_worker();} );
	}
	// Assert that the threadpool is Ready
	while (hasTasks()) {
		usleep(50);
	}
	std::cout << "done setting up threadpool\n";
}

Threadpool::~Threadpool() {
	this->stop_threads();
}

void Threadpool::launch_worker() {
	while (true) {
		{
//			std::cout << "decrementing _busyWorkers\n";
			std::unique_lock<std::mutex> lock(this->_m);
			--_busyWorkers;
		}
		Job job = this->_jobQueue.pop();
		if (job.get_moveIdx() == static_cast<unsigned int>(-1)) {
//			std::cerr << "got a faulty job, let's get outta here\n";
			break;
		}
		{
			std::unique_lock<std::mutex> lock(this->_m);
			++_busyWorkers;
		}
		auto *gs = job.execute();
		{
			std::unique_lock<std::mutex> lock(this->_parent_mutex);
			job.get_parent()->add_child(gs);
		}
	}
}

void Threadpool::WaitForWorkers() const {
	while (hasTasks())
		usleep(5u);
}

bool Threadpool::hasTasks() const {
	std::unique_lock<std::mutex>	lock(this->_m);
	return (this->_busyWorkers);
}

Threadpool& Threadpool::GetInstance() {
	static Threadpool threadpool;

	return threadpool;
}

void Threadpool::stop_threads() {
	this->WaitForWorkers();
	for (unsigned int i = 0; i < this->_numThreads; i++) {
		Job job;
		this->_jobQueue.push(job);
	}

	for (auto& thread : this->_threads) {
		thread.join();
	}
	this->_threads.clear();
}

#endif
