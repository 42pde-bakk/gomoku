import enum
from copy import deepcopy
from random import randint

import numpy as np

from srcs.board import Board
from srcs.heuristic import get_connects_of_player


class Stone(enum.IntEnum):
	EMPTY = 0
	PLAYER_1 = 1
	PLAYER_2 = 2

	def get_other_player(self):
		if self == Stone.PLAYER_1:
			return Stone.PLAYER_2
		return Stone.PLAYER_1


class Move:
	def __init__(self, y: int, x: int):
		self.y = y
		self.x = x

	def __repr__(self):
		return f'Move: y={self.y},x={self.x}'


class Gamestate:
	PLAYER_ONE = 0
	PLAYER_TWO = 1

	def __init__(self, parent = None):
		if isinstance(parent, Gamestate):
			self.board = deepcopy(parent.board)
			self.first_move = parent.first_move
			self.parent = parent
			self.captures = deepcopy(parent.captures)
			self.children = []
			self.winner = None
			self.h = parent.h
			self.turn = parent.turn + 1
		else:
			self.board = Board()
			# Maybe store indexes of all empty cells too?
			self.captures = [0, 0]
			self.parent = parent
			self.children = []
			self.winner = None
			self.first_move = None
			self.h = 0
			self.turn = 0

	def clone(self):
		return Gamestate(self)

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
		return True

	# def game_over_check(self, y: int, x: int, player: Stone) -> bool:
	# 	for dy, dx in [(1, 0), (0, 1), (1, 1), (-1, 1)]:
	# 		n = m = 1
	# 		while self.board.get(y + n * dy, x + n * dx) == player.value:
	# 			n += 1
	# 		while self.board.get(y + m * -dy, x + m * -dx) == player.value:
	# 			m += 1
	# 		if n + m + 1 >= 5:
	# 			# self.winner = player
	# 			self.captures[player.value - 1] += 10  # Win
	# 			return True
	# 	return False

	def set_h(self) -> int:
		p1 = get_connects_of_player(self.board.arr, player = 1)
		p2 = get_connects_of_player(self.board.arr, player = 2)
		self.h = p1 - p2
		return self.h

	def place_stone(self, y: int, x: int, stone: Stone) -> None:
		self.board.set(y, x, stone.value)
		# self.capture_check(y, x, stone)
		if self.first_move is None:
			self.first_move = Move(y = y, x = x)
		self.set_h()

	def generate_children(self) -> list:
		player = self.turn % 2
		# 0 for player 1 (User)
		# 1 for player 2 (Bot)

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
				child = Gamestate(self)
				child.place_stone(y = y, x = x, stone = Stone(player + 1))
				self.children.append(child)
		return self.children
