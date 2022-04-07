//
// Created by Peer De bakker on 4/7/22.
//

#ifndef GOMOKUBOT_ASYNCQUEUE_HPP
#define GOMOKUBOT_ASYNCQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include "Job.hpp"

template <typename T>
class AsyncQueue {
protected:
	std::queue<T>			_q;
	mutable std::mutex		_m;
	std::condition_variable	_c;

public:
	AsyncQueue();
	~AsyncQueue();

	virtual void	enqueue(T& t);
	virtual T		dequeue();
	bool	empty() const;
	void	clear();
};

template<typename T>
AsyncQueue<T>::AsyncQueue() : _q(), _m(), _c() {
}

template<typename T>
AsyncQueue<T>::~AsyncQueue() = default;

template<typename T>
void AsyncQueue<T>::enqueue(T& t) {
	std::lock_guard<std::mutex>	lock_guard(this->_m);
	this->_q.push(t);
	this->_c.notify_one();
}

template<typename T>
T AsyncQueue<T>::dequeue() {
	std::unique_lock<std::mutex> lock(this->_m);
//	while(this->_q.empty()) {
//		// release lock as long as the wait and reaquire it afterwards.
//		this->_c.wait(lock);
//	}
	this->_c.wait(lock, [&]{ return (!this->_q.empty()); } );
	T t = this->_q.front();
	this->_q.pop();
	return t;
}

template<typename T>
bool AsyncQueue<T>::empty() const {
	std::unique_lock<std::mutex>	lock(_m);
	return (this->_q.empty());
}

template<typename T>
void AsyncQueue<T>::clear() {
	std::unique_lock<std::mutex>	lock(_m);
	while (!this->_q.empty())
		this->_q.pop();
}

AsyncQueue<Gamestate*>&	getOutputQueue();

#endif //GOMOKUBOT_ASYNCQUEUE_HPP
