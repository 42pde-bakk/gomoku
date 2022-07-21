//
// Created by Peer De bakker on 4/7/22.
//

#ifndef GOMOKUBOT_JOB_HPP
#define GOMOKUBOT_JOB_HPP

#include "Gamestate.hpp"

class Job {
private:
	Gamestate *const _parent;
	const unsigned int _moveIdx;

public:
	Job();

	Job(Gamestate *const parent, unsigned int moveIdx);

	Job(Job &&x);

	Job(const Job &x) = delete;

	Job &operator=(const Job &x) = delete;

	~Job();

	Gamestate *get_parent() const;

	unsigned int get_moveIdx() const;

	[[nodiscard]] Gamestate *execute() const;

	friend std::ostream &operator<<(std::ostream &o, const Job &x);
};


#endif //GOMOKUBOT_JOB_HPP
