//
// Created by Peer De bakker on 3/25/22.
//

//#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Colours.hpp"
#include <random>

extern bool g_uses_lookuptable;

const int middle_idx = 9 * 20 + 9;

void	place_stones_hash(Gamestate *gs) {
	gs->set(middle_idx, 0);
	gs->set(middle_idx + WEST, 0);
	gs->set(middle_idx + EAST, 0);
	gs->set(middle_idx + NORTHEAST, 0);
	gs->set(middle_idx + 2 * NORTHEAST, 0);
	gs->set(middle_idx + NORTHEAST + 2 * NORTH, 0);
	gs->set(middle_idx + 4 * WEST + 2 * SOUTH, 0);

	gs->set(middle_idx + 2 * NORTH, 1);
	gs->set(middle_idx + 2 * NORTH + EAST, 1);
	gs->set(middle_idx + NORTHWEST, 1);
	gs->set(middle_idx + 2 * WEST, 1);
	gs->set(middle_idx + 3 * WEST + SOUTH, 1);
	gs->set(middle_idx + 2 * WEST + SOUTH, 1);
	gs->set(middle_idx + WEST + SOUTH, 1);

	gs->calcH(middle_idx + WEST + SOUTH);
}

void	place_random_stone(Gamestate *gs) {
	unsigned int player = 0;
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0,REALBOARDSIZE - 1); // distribution in range [0, 720]
	while (true) {
		unsigned int random = dist6(rng);
		if (gs->tile_is_empty(random) && !Bitboard::isSeperatingBitIndex(random)) {
//			std::cerr << "placed stone on idx " << random << '\n';
			gs->set(random, player);
			player = !player;
			break;
		}
	}
}

TEST_CASE("Bitset hash", "[HashingTests]") {
	auto start_time = std::chrono::steady_clock::now();
	std::hash<bitboard> hash_fn;
	for (int i = 0; i < 720; i++) {
		Gamestate gs;
		place_random_stone(&gs);
		auto hash = hash_fn(gs.get());
		(void)hash;
//		std::cerr << "hash = " << hash << std::endl;
	}
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "hashing 720x took " << elapsed_time.count() << " ms\n" _END;
}

TEST_CASE_METHOD(Gamestate, "Hash test 1", "[HashingTests]") {
	std::cerr << "start Hash test 1\n";
	g_uses_lookuptable = true;
	place_stones_hash(this);
	std::cerr << "h = " << this->get_h() << '\n';

	auto test = new Gamestate();
	place_stones_hash(test);
	std::cerr << "h = " << test->get_h() << '\n';

	g_uses_lookuptable = false;
}
