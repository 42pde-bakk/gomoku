//
// Created by Peer De bakker on 3/24/22.
//

#include "Gamestate.hpp"
#include "Directions.hpp"

unsigned int Gamestate::capture_check_dir(int idx, int dir) {
	const unsigned int player_stone = this->get_player() + 1;
	const unsigned int opp_stone = !this->get_player() + 1;
	const int	pos[3] = {idx + dir, idx + 2 * dir, idx + 3 * dir };

	assert(!Bitboard::isSeperatingBitIndex(idx));
	for (int po : pos) {
		if (po < 0 || po >= BOARDSIZE || isSeperatingBitIndex(po)) {
			return (0);
		}
	}

	const unsigned int values[3] = {bitboard_get(pos[0]), bitboard_get(pos[1]), bitboard_get(pos[2])};
	if (values[0] == opp_stone && values[1] == opp_stone && values[2] == player_stone) {
		this->clear_tile(pos[0]);
		this->clear_tile(pos[1]);
		this->captures[this->get_player()] += 2;
		return (1);
	}
	return (0);
}

unsigned int Gamestate::perform_captures(int pos) {
	unsigned int ret = 0;
	static const std::array<int, 8>	dirs = setup_all_dirs_singular();
	assert(pos < REALBOARDSIZE);

	for (int dir = 0; dir < 8; dir++) {
		ret |= this->capture_check_dir(pos, dirs[dir]);
	}
	return (ret);
}
