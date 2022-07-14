//
// Created by Peer De bakker on 3/24/22.
//

#include <array>
#include "Bitboard.hpp"

std::array<int, 8> all_dirs() {
	static const std::array<int, 8> dirs = {
			EAST, // E
			SOUTHWEST, // SW
			SOUTH, // S
			SOUTHEAST,  // SE
			WEST, // W
			NORTHEAST, // NE
			NORTH, // N
			NORTHWEST  // NW
	};
	return (dirs);
}

std::array<int, 4> setup_dirs() {
	static const std::array<int, 4> southside = {
			EAST, // E
			SOUTHEAST, // SE
			SOUTH, // S
			SOUTHWEST  // SW
	};
	return (southside);
}

std::array<int, 4> setup_dirs_opposite() {
	static const std::array<int, 4> northside = {
			WEST, // W
			NORTHWEST,  // NW
			NORTH, // N
			NORTHEAST // NE
	};
	return (northside);
}

std::array<int, 8> setup_all_dirs_singular() {
	static const std::array<int, 8> allsides = {
			NORTH, // N
			NORTHEAST, // NE
			EAST, // E
			SOUTHEAST, // SE
			SOUTH, // S
			SOUTHWEST, // SW
			WEST, // W
			NORTHWEST, // NW
	};
	return (allsides);
}

std::array<int, 8> setup_all_dirs() {
	static const std::array<int, 8> allsides = {
			2 * -NORTH, // N
			2 * NORTHEAST, // NE
			2 * EAST, // E
			2 * SOUTHEAST, // SE
			2 * SOUTH, // S
			2 * SOUTHWEST, // SW
			2 * WEST, // W
			2 * NORTHWEST, // NW
	};
	return (allsides);
}
