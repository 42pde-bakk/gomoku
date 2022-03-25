//
// Created by Peer De bakker on 3/24/22.
//

#include "Gamestate.hpp"
#include "Directions.hpp"

int Gamestate::capture_check_dir(int idx, int dir, int p) {
	int opp = !p;

	int	pos[3] = {idx + dir, idx + 2 * dir, idx + 3 * dir };
	for (int po : pos) {
		if (po < 0 || po >= BOARDSIZE || po % 20 == 0) {
			return (0);
		}
	}

	if (boards[opp][pos[0]] && boards[opp][pos[1]] && boards[p][pos[2]]) {
		boards[opp][pos[0]] = false;
		boards[opp][pos[1]] = false;
		this->captures[p] += 2;
		return (1);
	}
	return (0);
}


int Gamestate::perform_captures(int pos) {
	int ret = 0;
	static const std::array<int, 8>	dirs = all_dirs();

	for (int dir = 0; dir < 8; dir++) {
		ret += this->capture_check_dir(pos, dirs[dir], player);
	}
	return (ret);
}
