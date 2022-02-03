import enum
from copy import deepcopy

import numpy as np

from srcs.board import Board


class Stone(enum.IntEnum):
	EMPTY = 0
	PLAYER_1 = 1
	PLAYER_2 = 2


class Gamestate:
	PLAYER_ONE = 0
	PLAYER_TWO = 1

	def __init__(self, parent = None):
		self.board = Board()
		self.captures = [0, 0]
		self.parent = parent
		self.children = []
		self.winner = None

	@property
	def h(self):
		return self.captures[0] - self.captures[1]

	def __lt__(self, other):
		return self.h < other.h

	def __repr__(self):
		return '\n'.join(map(str, self.board.arr))

	def player_check(self, y: int, x: int, player_to_check: int) -> bool:
		return y < 0 or y >= 19 or x < 0 or x >= 19 or self.board.get(y, x) != player_to_check

	def capture(self, pos1: tuple, pos2: tuple, capturing_player: Stone) -> None:
		pos1_y, pos1_x = pos1
		pos2_y, pos2_x = pos2
		self.captures[capturing_player - 1] += 2  # -1 because player 1 has index 0 in the captures array
		self.board.set(pos1_y, pos1_x, Stone.EMPTY.value)
		self.board.set(pos2_y, pos2_x, Stone.EMPTY.value)

	def capture_check(self, y: int, x: int, player: Stone) -> bool:
		other_player = player.get_other_player()
		for dy, dx in [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1)]:
			if self.player_check(y + dy, x + dx, other_player) and self.player_check(y + 2 * dy, x + 2 * dx, other_player) and self.player_check(y + 3 * dy, x + 3 * dx, player):
				self.capture((y + dy, x + dx), (y + 2 * dy, x + 2 * dx), player)
			print((dy, dx))
		return True

	def game_over_check(self, y: int, x: int, player: Stone) -> bool:
		for dy, dx in [(1, 0), (0, 1), (1, 1), (-1, 1)]:
			n = m = 1
			while self.board.get(y + n * dy, x + n * dx) == player.value:
				n += 1
			while self.board.get(y + m * -dy, x + m * -dx) == player.value:
				m += 1
			if n + m + 1 >= 5:
				# self.winner = player
				self.captures[player.value - 1] += 10  # Win
				return True
		return False

	def place_stone(self, y: int, x: int, player: Stone) -> None:
		self.board.set(y, x, player)
		self.capture_check(y, x, player)

	def generate_children(self, player: int) -> list:
		def touches_occupied() -> bool:
			for dy, dx in [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1)]:
				if self.board.get(y = y + dy, x = x + dx):
					return True
			return False

		self.children = []
		if self.winner or any(c >= 10 for c in self.captures):
			return []
		for (y, x), item in np.ndenumerate(self.board.arr):
			if item == Stone.EMPTY and touches_occupied():
				child = deepcopy(self)
				child.board.set(y = y, x = x, item = player)
				self.children.append(child)
		return self.children
