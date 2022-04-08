//
// Created by Peer De bakker on 4/7/22.
//

#include "Threadpool.hpp"
#include "JobQueue.hpp"
#include "Gomoku.hpp"
#include <unistd.h>
#if THREADED

Threadpool::Threadpool(unsigned int numThreads)
	: _numThreads(numThreads), busyWorkers(numThreads), _jobQueue(getJobQueue()), _outputQueue(getOutputQueue())
{
	this->_threads.reserve(this->_numThreads);
	for (unsigned int i = 0; i < _numThreads; i++) {
		this->_threads.emplace_back( [this] { launch_worker();} );
	}
}

Threadpool::~Threadpool() {
	for (auto& thread : this->_threads)
		thread.join();
}

void Threadpool::launch_worker() {
	while (true) {
		--busyWorkers;
		Job job = this->_jobQueue.pop();
		++busyWorkers;
		auto gs = job.execute();
		this->_outputQueue.push(gs);
	}
}

void Threadpool::WaitForWorkers() const {
	while (this->busyWorkers) {
		// dummy
		usleep(10u);
	}
}

#endif
