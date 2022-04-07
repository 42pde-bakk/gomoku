//
// Created by Peer De bakker on 4/7/22.
//

#include "JobQueue.hpp"

std::mutex cerrMutex;

Job JobQueue::dequeue() {
	std::unique_lock<std::mutex> lock(this->_m);
	this->_c.wait(lock, [&]{ return (!this->_q.empty()); } );
	Job job = this->_q.front();
	this->_q.pop();

	if (--njobs_pending == 0) {
		// all jobs done
		main_condition.notify_one();
	}
	return job;
}

void JobQueue::waitFinished() {
	if (njobs_pending > 0) {
		std::unique_lock<std::mutex>	lock(main_mutex);
		main_condition.wait(lock);
	}
}

void JobQueue::enqueue(Job &job) {
	std::lock_guard<std::mutex>	lock_guard(this->_m);
	this->_q.push(job);
	this->njobs_pending++;

	this->_c.notify_one();
}

JobQueue&	getJobQueue() {
	static JobQueue	JobQueue;

	return (JobQueue);
}
