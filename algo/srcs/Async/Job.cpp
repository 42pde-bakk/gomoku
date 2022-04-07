//
// Created by Peer De bakker on 4/7/22.
//

#include "Async/Job.hpp"
#include "Gamestate.hpp"

Job::Job(const Gamestate *parent, const unsigned int moveIdx) : _parent(parent), _moveIdx(moveIdx) {
}

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
