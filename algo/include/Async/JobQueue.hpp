//
// Created by Peer De bakker on 4/7/22.
//

#ifndef GOMOKUBOT_JOBQUEUE_HPP
#define GOMOKUBOT_JOBQUEUE_HPP
# include "AsyncQueue.hpp"

class JobQueue : public AsyncQueue<Job> {
private:
	std::atomic<int>	njobs_pending;
	std::mutex			main_mutex;
	std::condition_variable	main_condition;

public:
	void	waitFinished();
	void	enqueue(Job& job) override;
	Job		dequeue() override;
};

JobQueue&		getJobQueue();

#endif //GOMOKUBOT_JOBQUEUE_HPP
