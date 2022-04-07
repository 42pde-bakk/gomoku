//
// Created by Peer De bakker on 4/7/22.
//

#include "JobQueue.hpp"
#include <fstream>

std::mutex cerrMutex;
std::mutex fsMutex;

Job JobQueue::pop() {
//	Job job = AsyncQueue<Job>::pop();
	Job job;
	{
		std::unique_lock<std::mutex> lock(this->_m);
		this->_c.wait(lock, [this] { return (!(this->_q.empty())); } );
//	this->_c.wait(lock, [&]{ return (!this->_q.empty()); } );
		job = this->_q.top();
		this->_q.pop();
	}

	if (--njobs_pending == 0) {
		// all jobs done
		main_condition.notify_one();
	}
	return job;
}

void JobQueue::waitFinished() {
//	while (njobs_pending > 0);
	if (njobs_pending > 0) {
		std::unique_lock<std::mutex>	lock(main_mutex);
		main_condition.wait(lock);
	}
}

void JobQueue::push(Job& job) {
//	std::fstream fs("log/generate_children.txt", std::fstream::out | std::fstream::trunc);
//	auto push_start = std::chrono::steady_clock::now();
	{
		std::lock_guard<std::mutex>	lock_guard(this->_m);
		this->_q.push(job);
	}
//	auto current_time = std::chrono::steady_clock::now();
//	auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(current_time - push_start).count();
//	fs << "JobQueue::push::waiting for mutex took " << elapsed_time << " microseconds.\n";
	this->njobs_pending++;
	this->_c.notify_one();
}

JobQueue&	getJobQueue() {
	static JobQueue	JobQueue;

	return (JobQueue);
}
