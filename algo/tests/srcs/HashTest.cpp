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

TEST_CASE_METHOD(Heuristic, "Copy ValueBoard", "[HashingTests]") {
	ValueBoard	first{},
				second{};

	first[0][OPEN_TWO] = 2;
	first[0][OPEN_THREE] = 3;
	first[0][HALF_OPEN_FOUR] = 4;
	first[0][FIVE] = 1;

	second = first;
	first[0][OPEN_TWO] = 1;
	REQUIRE(second[0][OPEN_TWO] == 2);
	std::cout << "ValueBoard deepcopied succesfully, unlike PYTHON!!!\n";
}

TEST_CASE_METHOD(Gamestate, "ffs", "[HashingTests]") {
	g_uses_lookuptable = true;
	tt.clear();

	this->set(middle_idx, 0);
	this->set(middle_idx + WEST, 0);
	this->set(middle_idx + EAST, 0);
	this->set(middle_idx + NORTH, 0);
	this->set(middle_idx + WEST + SOUTHWEST, 0);

	this->set(middle_idx + 2 * EAST, 1);
	this->set(middle_idx + 2 * EAST + NORTH, 1);
	this->set(middle_idx + 2 * WEST, 1);
	this->set(middle_idx + 2 * WEST + NORTH, 1);
	this->set(middle_idx + 2 * WEST + 2 * NORTH, 1);
	this->calcH(middle_idx + NORTH);
	std::cerr << *this;
	this->print_heuristic(std::cerr);

	g_uses_lookuptable = false;
}

TEST_CASE("not the frking same", "[HashingTests]") {
	auto a = new Gamestate();
	auto b = new Gamestate();
	std::hash<bitboard> hash_fn;
	unsigned long hash_a, hash_b;

	g_uses_lookuptable = true;
	{
		auto start = 7 * 20 + 6;
		a->set(start, 0);
		a->set(start + SOUTHEAST, 1);
		a->set(start + SOUTHEAST + EAST, 1);
		a->set(start + 2 * SOUTHEAST, 0);
		a->set(start + 2 * SOUTHEAST + EAST, 0);
//		std::cerr << *a;
		a->calcH(-1);
		hash_a = hash_fn(a->get_bitboard());
	}
	{
		auto start = 8 * 20 + 7;
		b->set(start, 1);
		b->set(start + EAST, 1);
		b->set(start + SOUTHEAST, 0);
		b->set(start + SOUTHEAST + EAST, 0);
		b->set(start + 2 * SOUTHEAST + EAST, 0);
//		std::cerr << *b;
		b->calcH(-1);
		hash_b = hash_fn(b->get_bitboard());
	}
#ifdef __APPLE__
	REQUIRE(hash_a == hash_b);
#endif
	REQUIRE(Heuristic::tt[a->get_bitboard()] != Heuristic::tt[b->get_bitboard()]);
	g_uses_lookuptable = false;
}
