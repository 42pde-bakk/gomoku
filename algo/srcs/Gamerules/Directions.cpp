//
// Created by Peer De bakker on 3/24/22.
//

#include <array>

std::array<int, 8>	all_dirs() {
	static const std::array<int, 8> dirs = {
			1, // E
			19, // SW
			20, // S
			21,  // SW
			-1, // W
			-19, // NE
			-20, // N
			-21  // NW
	};
	return (dirs);
}

std::array<int, 4> setup_dirs() {
	static const std::array<int, 4> southside = {
			1, // E
			21, // SE
			20, // S
			19  // SW
	};
	return (southside);
}

std::array<int, 4> setup_dirs_opposite() {
	static const std::array<int, 4> northside = {
			-1, // W
			-19, // NE
			-20, // N
			-21  // NW
	};
	return (northside);
}

std::array<int, 8> setup_all_dirs_singular() {
	static const std::array<int, 8> allsides = {
			-20, // N
			-19, // NE
			1, // E
			21, // SE
			20, // S
			19, // SW
			-1, // W
			-21, // NW
	};
	return (allsides);
}

std::array<int, 8> setup_all_dirs() {
	static const std::array<int, 8> allsides = {
			2 * -20, // N
			2 * -19, // NE
			2 * 1, // E
			2 * 21, // SE
			2 * 20, // S
			2 * 19, // SW
			2 * -1, // W
			2 * -21, // NW
	};
	return (allsides);
}
