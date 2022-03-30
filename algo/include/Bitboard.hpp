//
// Created by Peer De bakker on 3/29/22.
//

#ifndef GOMOKUBOT_BITBOARD_HPP
#define GOMOKUBOT_BITBOARD_HPP
#include <bitset>

# define BOARDHEIGHT 19 * 2
# define BOARDWIDTH 20 * 2 // 1 for a seperating bit
# define BOARDSIZE BOARDHEIGHT * BOARDWIDTH
# define REALBOARDSIZE 20 * 19

typedef std::bitset<BOARDSIZE> bitboard;

class Bitboard {
protected:
	bitboard board;
public:
	Bitboard();
	Bitboard(const bitboard& b);
	~Bitboard();
	Bitboard(const Bitboard& x);
	Bitboard& operator=(const Bitboard& x);

	[[nodiscard]] unsigned int bitboard_get(unsigned int idx) const;
	[[nodiscard]] bool	tile_is_empty(unsigned int idx) const;
	[[nodiscard]] static bool	is_seperating_bit(unsigned int idx) ;
	[[nodiscard]] unsigned int	at(size_t n) const; // just a getter
	unsigned int	operator[](size_t n) const; // just a getter
	void	set(unsigned int idx, unsigned int player);

	// relational operators
	bool	operator==(const Bitboard& rhs) const;
	bool	operator!=(const Bitboard& rhs) const;
//	bool	operator<(const Bitboard& rhs);
////	bool	operator>(const Bitboard& rhs);
////	bool	operator<=(const Bitboard& rhs);
////	bool	operator>=(const Bitboard& rhs);

	[[nodiscard]] bitboard	get_empty_neighbours() const;

	void	print_board(std::ostream& o, bool colours) const;
	friend std::ostream&	operator<<(std::ostream& o, const Bitboard& b);
};


#endif //GOMOKUBOT_BITBOARD_HPP
