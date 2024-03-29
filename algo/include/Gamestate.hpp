//
// Created by Peer De bakker on 3/22/22.
//

#ifndef GOMOKUBOT_GAMESTATE_HPP
#define GOMOKUBOT_GAMESTATE_HPP

# include "Gomoku.hpp"
# include <array>
# include <vector>
# include <Move.hpp>
# include <unordered_map>
# include <bitset>
# include "Heuristic.hpp"

class Client;

class Gamestate : public Heuristic {
	Gamestate &operator=(const Gamestate &x);

protected:
	Move lastmove;
	const Gamestate *parent{};
	std::vector<Gamestate *> children;

	friend Client;
public:
	Gamestate();

	Gamestate(const Gamestate &x);

	~Gamestate();

	void generate_children(unsigned int depth_level);

	std::vector<Move> generate_moves() const;

	void apply_move(const Move &move);

	std::vector<Gamestate *> &get_children() { return (this->children); }

	bool operator==(const Gamestate &rhs) const { return (this->board == rhs.board); }

	bool operator!=(const Gamestate &rhs) const { return !(*this == rhs); }

	bool operator<(const Gamestate &rhs) const { return (this->h < rhs.h); }

	bool operator>(const Gamestate &rhs) const { return (rhs < *this); }

	bool operator<=(const Gamestate &rhs) const { return !(rhs < *this); }

	bool operator>=(const Gamestate &rhs) const { return !(*this < rhs); }

	[[nodiscard]] const Move &get_first_move(const Gamestate *root) const;

	[[nodiscard]] Move get_move() const;

	bool place_stone(unsigned int move_idx);

	void clear_children();
	void	reset();

	void add_child(Gamestate *child);

	bool has_children() const;

	void sort_children();

	Gamestate *calcH(const unsigned int new_stone_idx);

	const Gamestate *get_parent();

	void print_history(std::ostream &o, bool colours) const;

protected:
	int change_player();

	// Captures.cpp
	unsigned int perform_captures(unsigned int pos);

	unsigned int capture_check_dir(unsigned int idx, unsigned int dir);
};

typedef bool (*compareFunc)(const Gamestate *a, const Gamestate *b);

#endif //GOMOKUBOT_GAMESTATE_HPP
