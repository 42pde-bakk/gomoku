import enum
from copy import deepcopy
import random

import numpy as np

from srcs.board import Board
from srcs.heuristic import get_connects_of_player


class Stone(enum.IntEnum):
	EMPTY = 0
	PLAYER_1 = 1
	PLAYER_2 = 2

	@staticmethod
	def get_other_player(val: int):
		if val == 1:
			return 2
		return 1


class Move:
	def __init__(self, y: int, x: int, player: int):
		self.player = player
		self.y = y
		self.x = x

	def __repr__(self):
		return f'Move({self.player}): y={self.y},x={self.x}'


class Gamestate:
	PLAYER_ONE = 0
	PLAYER_TWO = 1

	def __init__(self, parent = None):
		if isinstance(parent, Gamestate):
			self.board = deepcopy(parent.board)
			self.moves = deepcopy(parent.moves)
			self.parent = parent
			self.captures = deepcopy(parent.captures)
			self.children = []
			self.winner = None
			self.h = parent.h
			self.turn = parent.turn
		else:
			self.board = Board()
			# Maybe store indexes of all empty cells too?
			self.moves = list()
			self.captures = [0, 0]
			self.parent = parent
			self.children = []
			self.winner = None
			self.h = 0
			self.turn = 0

	def clone(self):
		return Gamestate(self)

	def __lt__(self, other):
		return self.h < other.h

	def __repr__(self):
		return str(self.h)

	def player_check(self, y: int, x: int, player_to_check: int) -> bool:
		return y < 0 or y >= 19 or x < 0 or x >= 19 or self.board.get(y, x) != player_to_check

	def capture(self, pos1: tuple, pos2: tuple, capturing_player: Stone) -> None:
		pos1_y, pos1_x = pos1
		pos2_y, pos2_x = pos2
		self.captures[capturing_player - 1] += 2  # -1 because player 1 has index 0 in the captures array
		self.board.set(pos1_y, pos1_x, Stone.EMPTY.value)
		self.board.set(pos2_y, pos2_x, Stone.EMPTY.value)

	def capture_check(self, y: int, x: int, player: int) -> bool:
		other_player = Stone.get_other_player(player)
		for dy, dx in [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1)]:
			captures = [(y + dy, x + dx), (y + 2 * dy, x + 2 * dx)]
			other_stone_of_player = y + 3 * dy, x + 3 * dx
			if 0 <= other_stone_of_player[0] < Board.SIZE and 0 <= other_stone_of_player[1] < Board.SIZE:
				if self.board.arr[other_stone_of_player] == player and self.board.arr[captures[0]] == self.board.arr[captures[1]] == other_player:
					print(f'would capture')
					# TODO: implement capture mechanics
					# capture
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
		p1, game_over1 = get_connects_of_player(self.board.arr, player = 1)
		p2, game_over2 = get_connects_of_player(self.board.arr, player = 2)
		self.h = p1 - p2
		if game_over1:
			self.winner = 1
		elif game_over2:
			self.winner = 2
		return self.h

	def update_h(self, pos: tuple[int, int]) -> int:
		left = self.board.get_consecutive_stones(start = pos, direction = (0, -1))
		right = self.board.get_consecutive_stones(start = pos, direction = (0, 1))
		down = self.board.get_consecutive_stones(start = pos, direction = (1, 0))
		up = self.board.get_consecutive_stones(start = pos, direction = (-1, 0))
		downleft = self.board.get_consecutive_stones(start = pos, direction = (1, -1))
		upright = self.board.get_consecutive_stones(start = pos, direction = (-1, 1))
		downright = self.board.get_consecutive_stones(start = pos, direction = (1, 1))
		upleft = self.board.get_consecutive_stones(start = pos, direction = (-1, -1))
		h = 0
		for a, b in zip([left, down, downleft, upleft], [right, up, upright, downright]):
			if a > 2:
				h -= pow(10, a - 2)
			if b > 2:
				h -= pow(10, b - 2)
			newlength = (a - 1) + (b - 1) + 1
			if newlength > 1:
				h += pow(10, newlength - 1)
		if self.turn % 2 == 0:
			self.h += h
		else:
			self.h -= h
		return self.h

	def place_stone(self, y: int, x: int, stone: int) -> None:
		self.board.set(y, x, stone)
		self.capture_check(y, x, stone)
		move = Move(y = y, x = x, player = stone)
		self.moves.append(move)
		self.update_h(pos = (y, x))
		self.turn += 1

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
		# TODO: change enumeration to np.nonzero or np.where(arr == 0)
		for (y, x), item in np.ndenumerate(self.board.arr):
			if item == Stone.EMPTY and touches_occupied():
				child = Gamestate(self)
				child.place_stone(y = y, x = x, stone = player + 1)
				self.children.append(child)
		random.shuffle(self.children)
		self.children.sort(reverse = bool(player == 1))
		return self.children
