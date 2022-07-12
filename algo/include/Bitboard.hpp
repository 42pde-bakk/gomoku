//
// Created by Peer De bakker on 3/29/22.
//

#ifndef GOMOKUBOT_BITBOARD_HPP
#define GOMOKUBOT_BITBOARD_HPP

#include <bitset>

# define BOARDHEIGHT 19
# define BOARDWIDTH 40 // +1 for a seperating bit
# define BOARDSIZE BOARDHEIGHT * BOARDWIDTH
# define REALBOARDWIDTH 20
# define REALBOARDSIZE REALBOARDWIDTH * BOARDHEIGHT

typedef std::bitset<BOARDSIZE> bitboard;

class Bitboard {
protected:
	bitboard board;
public:
	Bitboard();

	explicit Bitboard(const bitboard &b);

	~Bitboard();

	Bitboard(const Bitboard &x);

	Bitboard &operator=(const Bitboard &x);

	static bool isvalid_tile(unsigned int idx);

	[[nodiscard]] unsigned int bitboard_get(unsigned int idx) const;

	[[nodiscard]] bool tile_is_empty(unsigned int idx) const;

	[[nodiscard]] static bool isSeperatingBitIndex(unsigned int idx);

	[[nodiscard]] unsigned int at(size_t n) const; // just a getter
	unsigned int operator[](size_t n) const; // just a getter
	void set(unsigned int idx, unsigned int player);

	void clear_tile(unsigned int idx);

	// relational operators
	bool operator==(const Bitboard &rhs) const;

	bool operator!=(const Bitboard &rhs) const;
//	bool	operator<(const Bitboard& rhs);
////	bool	operator>(const Bitboard& rhs);
////	bool	operator<=(const Bitboard& rhs);
////	bool	operator>=(const Bitboard& rhs);

	[[nodiscard]] bitboard get_empty_neighbours() const;

	void print_board(std::ostream &o, bool colours) const;

	friend std::ostream &operator<<(std::ostream &o, const Bitboard &b);

	[[nodiscard]] bool none() const;

	[[nodiscard]] const bitboard &get() const;

	[[nodiscard]] unsigned int count() const;
};


#endif //GOMOKUBOT_BITBOARD_HPP
