//
// Created by Peer De bakker on 3/22/22.
//

#ifndef CLUSTER_GAMESTATE_HPP
#define CLUSTER_GAMESTATE_HPP
#include <array>
#include <vector>
#include <Move.hpp>
#define BOARDLENGTH 19
#define BOARDSIZE BOARDLENGTH * BOARDLENGTH


enum class STONE {
	EMPTY,
	PLAYER1,
	PLAYER2
};

class Client;

class Gamestate {
public:
	std::array<int, BOARDSIZE> board{};
	std::array<int, 2>	captures{};
	std::vector<Move>	moves;
	Gamestate	*parent{};
	std::vector<Gamestate*> children;
	int turn{},
		h{},
		winner{};

	friend Client;

public:
	Gamestate() {}

	[[nodiscard]] bool	has_winner() const { return (this->winner != 0); }
	void	set_heuristic(int heuristic) { this->h = heuristic; }

	void	generate_children() {
		// TODO
	}
	std::vector<Gamestate*>& get_children() { return this->children; }

	bool	operator==(const Gamestate& rhs) const { return (this->h == rhs.h); }
	bool	operator!=(const Gamestate& rhs) const { return !(*this == rhs); }

	bool	operator<(const Gamestate& rhs) const { return (this->h < rhs.h); }
	bool	operator>(const Gamestate& rhs) const { return (rhs < *this); }
	bool	operator<=(const Gamestate& rhs) const { return !(rhs < *this); }
	bool	operator>=(const Gamestate& rhs) const { return !(*this < rhs); }

	void	print_board(std::ostream& o) const;
	[[nodiscard]] int		get_player() const;

};


#endif //CLUSTER_GAMESTATE_HPP
