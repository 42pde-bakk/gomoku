//
// Created by Peer De bakker on 4/7/22.
//

#include "AsyncQueue.hpp"

AsyncQueue<Gamestate*>&	getOutputQueue() {
	static AsyncQueue<Gamestate*>	outputQueue;

	return (outputQueue);
}

AsyncQueue<Job>&	getJobQueue() {
	static AsyncQueue<Job>	jobQueue;

	return (jobQueue);
}

std::mutex cerrMutex;
std::mutex fsMutex;
