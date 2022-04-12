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
	Gamestate& operator=(const Gamestate& x);
protected:
	std::vector<Move>	moves;
	const Gamestate	*parent{};
	std::vector<Gamestate*> children;

	friend Client;
public:
	Gamestate();
	Gamestate(const Gamestate& x);
	~Gamestate();

//	void	update_heuristic(unsigned int move_idx);

	void generate_children();
	std::vector<Gamestate*>& get_children() { return (this->children); }

	bool	operator==(const Gamestate& rhs) const { return (this->board == rhs.board); }
	bool	operator!=(const Gamestate& rhs) const { return !(*this == rhs); }

	bool	operator<(const Gamestate& rhs) const { return (this->h < rhs.h); }
	bool	operator>(const Gamestate& rhs) const { return (rhs < *this); }
	bool	operator<=(const Gamestate& rhs) const { return !(rhs < *this); }
	bool	operator>=(const Gamestate& rhs) const { return !(*this < rhs); }

	[[nodiscard]] const Move& get_first_move() const;

	void	place_stone(unsigned int move_idx);
	void	clear_children();

	void	add_child(Gamestate* child);

	void	print_history(std::ostream& o) const;
protected:
	int		change_player();
	void	write_to_file() const;
	// Captures.cpp
	unsigned int perform_captures(unsigned int pos);
	unsigned int capture_check_dir(unsigned int idx, unsigned int dir);
};

typedef bool (*compareFunc)(const Gamestate* a, const Gamestate* b);

#endif //GOMOKUBOT_GAMESTATE_HPP
