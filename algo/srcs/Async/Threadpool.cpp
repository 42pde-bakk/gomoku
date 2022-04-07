//
// Created by Peer De bakker on 4/7/22.
//

#include "Threadpool.hpp"
#include "JobQueue.hpp"

Threadpool::Threadpool(unsigned int numThreads)
	: _numThreads(numThreads), _jobQueue(getJobQueue()), _outputQueue(getOutputQueue())
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
		Job job = this->_jobQueue.dequeue();
		auto gs = job.execute();
		this->_outputQueue.enqueue(gs);
	}
}
