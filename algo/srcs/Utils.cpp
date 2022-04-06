//
// Created by Peer De bakker on 3/30/22.
//

#include "Utils.hpp"

#ifdef __clang__
	template<size_t N1, size_t N2>
	std::bitset<N1 + N2> concat(const std::bitset<N1> &b1, const std::bitset<N2> &b2) {
		std::bitset<N1 + N2> res;
		for (size_t i = 0; i < N1; ++i) {
			res[i] = b1[i];
			res[N1 + i] = b2[i];
		}
		return res;
	}
	#else
	template <size_t N1, size_t N2 >
	std::bitset<N1 + N2> concat(const std::bitset<N1>& b1, const std::bitset<N2>& b2 ) {
		std::string first(b1.to_string());
		std::string second(b2.to_string());
		return (first + second);
	}
#endif
