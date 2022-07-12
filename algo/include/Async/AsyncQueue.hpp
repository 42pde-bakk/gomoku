//
// Created by Peer De bakker on 4/7/22.
//

#ifndef GOMOKUBOT_ASYNCQUEUE_HPP
#define GOMOKUBOT_ASYNCQUEUE_HPP

#include <array>
#include <mutex>
#include <condition_variable>
#include "Job.hpp"
#include <unistd.h>

template<typename T>
class AsyncQueue {
protected:
	std::vector<T> _q;
	mutable std::mutex _m;
	std::condition_variable _c;

public:
	AsyncQueue();

	~AsyncQueue();

	AsyncQueue(const AsyncQueue<T> &) = delete;

	AsyncQueue &operator=(const AsyncQueue<T> &) = delete;

	virtual void push(T &t);

	virtual T pop();

	bool empty() const;

	size_t size() const;

	void clear();

	void waitTillFinished();

	void notify_all();
};

template<typename T>
AsyncQueue<T>::AsyncQueue() : _q(), _m(), _c() {
	_q.reserve(REALBOARDSIZE + 1);
}

template<typename T>
AsyncQueue<T>::~AsyncQueue() = default;

template<typename T>
void AsyncQueue<T>::push(T &t) {
	std::lock_guard<std::mutex> lock_guard(this->_m);
	this->_q.push_back(std::move(t));
	this->_c.notify_one();
}

template<typename T>
T AsyncQueue<T>::pop() {
	std::unique_lock<std::mutex> lock(this->_m);
//	while(this->_q.empty()) {
//		// release lock as long as the wait and reaquire it afterwards.
//		this->_c.wait(lock);
//	}
	this->_c.wait(lock, [&] { return (!this->_q.empty()); });

	T t = std::move(this->_q.back());
	this->_q.pop_back();
	return t;
}

template<typename T>
bool AsyncQueue<T>::empty() const {
	bool ret;
	{
		std::unique_lock<std::mutex> lock(_m);
		ret = this->_q.empty();
	}
	return (ret);
}

template<typename T>
void AsyncQueue<T>::clear() {
	std::unique_lock<std::mutex> lock(_m);
	while (!this->_q.empty())
		this->_q.pop_back();
}

template<typename T>
size_t AsyncQueue<T>::size() const {
	std::unique_lock<std::mutex> lock(_m);
	return (this->_q.size());
}

template<typename T>
void AsyncQueue<T>::waitTillFinished() {
	while (!this->empty()) {
		usleep(5u);
	}
//	if (this->empty())
//		return ;
//	std::unique_lock<std::mutex> lock(this->_m);
//	this->_c.wait(lock, [&]{ return (this->_q.empty()); } );
}

AsyncQueue<Gamestate *> &getOutputQueue();

AsyncQueue<Job> &getJobQueue();

#endif //GOMOKUBOT_ASYNCQUEUE_HPP
