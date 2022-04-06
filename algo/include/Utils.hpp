//
// Created by Peer De bakker on 3/28/22.
//

#ifndef GOMOKUBOT_UTILS_HPP
# define GOMOKUBOT_UTILS_HPP
# include <cstddef>
# include <bitset>

template <size_t N1, size_t N2 >
std::bitset<N1 + N2> concat(const std::bitset<N1>& b1, const std::bitset<N2>& b2 );


#endif //GOMOKUBOT_UTILS_HPP
