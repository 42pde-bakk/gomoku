//
// Created by Peer De bakker on 4/7/22.
//

#ifndef GOMOKUBOT_JOBQUEUE_HPP
#define GOMOKUBOT_JOBQUEUE_HPP

# include "AsyncQueue.hpp"
# include <atomic>

class JobQueue : public AsyncQueue<const Job *> {
private:
	std::atomic<int> njobs_pending;
	std::mutex main_mutex;
	std::condition_variable main_condition;

public:
	void waitFinished();

	void pushJob(const Job *job);

	const Job *pop() override;
};

AsyncQueue<Job> &getJobQueue();

#endif //GOMOKUBOT_JOBQUEUE_HPP
