//
// Created by Peer De bakker on 4/7/22.
//

#include "Async/Job.hpp"
#include "Gamestate.hpp"

Job::Job() : _parent(), _moveIdx(-1) { }

Job::Job(const Gamestate *parent, const unsigned int moveIdx) : _parent(parent), _moveIdx(moveIdx) { }

//Job::Job(const Job &x) : _parent(x._parent), _moveIdx(x._moveIdx) { }
//
//Job &Job::operator=(const Job& x) {
//	if (this != &x) {
//		this->_parent = x._parent;
//		this->_moveIdx = x._moveIdx;
//	}
//	return (*this);
//}

Job::~Job() = default;

const Gamestate *Job::get_parent() {
	return (this->_parent);
}

unsigned int Job::get_moveIdx() {
	return (this->_moveIdx);
}

Gamestate *Job::execute() const {
	auto	*child = new Gamestate(*this->_parent);
	child->place_stone(this->_moveIdx);
	return (child);
}

Job::Job(Job&& x) : _parent(std::move(x._parent)), _moveIdx(std::move(x._moveIdx)) { }
