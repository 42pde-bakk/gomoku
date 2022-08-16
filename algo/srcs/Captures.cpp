//
// Created by Peer De bakker on 3/24/22.
//

#include "Gamestate.hpp"
#include "Directions.hpp"
#include <cassert>
#include <cstdio>

static uint32_t	CAPTURE_VALUE = 5000;

void		set_capture_value(uint32_t value) {
	CAPTURE_VALUE = value;
	fprintf(stderr, "Changed capture value to %u\n", CAPTURE_VALUE);
}

uint32_t	get_capture_value() {
	return (CAPTURE_VALUE);
}

unsigned int Gamestate::capture_check_dir(unsigned int idx, unsigned int dir) {
	static const unsigned int MAX_CAPTURES = 5;
	const unsigned int player_stone = this->player + 1;
	const unsigned int opp_stone = !this->player + 1;
	const unsigned int pos[3] = {idx + dir, idx + 2 * dir, idx + 3 * dir};

	assert(!Bitboard::isSeperatingBitIndex(idx));
	for (unsigned int po: pos) {
		if (po >= BOARDSIZE || isSeperatingBitIndex(po)) {
			return (0);
		}
	}

	const unsigned int values[3] = {bitboard_get(pos[0]), bitboard_get(pos[1]), bitboard_get(pos[2])};
	if (values[0] == opp_stone && values[1] == opp_stone && values[2] == player_stone) {
		this->clear_tile(pos[0]);
		this->clear_tile(pos[1]);
		this->captures[this->player] += 1;
		if (this->captures[this->player] >= MAX_CAPTURES) {
			// Perhaps I should make winning by captures worth more but let's see
			this->set_winner(this->player);
		}
		return (1);
	}
	return (0);
}

unsigned int Gamestate::perform_captures(unsigned int pos) {
	unsigned int ret = 0;
	static const std::array<int, 8> dirs = setup_all_dirs_singular();
	assert(pos < REALBOARDSIZE);

	for (int dir = 0; dir < 8; dir++) {
		ret |= this->capture_check_dir(pos, dirs[dir]);
	}
	return (ret);
}
