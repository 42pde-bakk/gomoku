//
// Created by Peer De bakker on 4/7/22.
//

#ifndef GOMOKUBOT_JOB_HPP
#define GOMOKUBOT_JOB_HPP
#include "Gamestate.hpp"

class Job {
private:
	const Gamestate*	_parent;
	const unsigned int	_moveIdx;

public:
	Job();
	Job(const Gamestate* parent, unsigned int moveIdx);
	Job(Job&& x);
	Job(const Job& x) = delete;
	Job& operator=(const Job& x) = delete;
	~Job();

	const Gamestate*	get_parent();
	unsigned int		get_moveIdx();

	[[nodiscard]] Gamestate*	execute() const;
};


#endif //GOMOKUBOT_JOB_HPP
