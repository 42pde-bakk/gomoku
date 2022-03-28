import operator
from typing import Union

import numpy as np


class Board:
	SIZE = 19

	def __init__(self):
		self.arr = np.zeros(shape = (Board.SIZE, Board.SIZE), dtype = np.int8)

	def __repr__(self):
		return str(self.arr)

	def get(self, y: int, x: int) -> Union[np.int8, None]:
		if 0 <= y < Board.SIZE and 0 <= x < Board.SIZE:
			return self.arr[y][x]
		return None

	def getwithtuple(self, t: tuple[int, int]) -> Union[np.int8, None]:
		return self.get(t[0], t[1])

	def get_board(self) -> np.ndarray:
		return self.arr

	def set(self, y: int, x: int, item) -> None:
		if 0 <= y < Board.SIZE and 0 <= x < Board.SIZE:
			self.arr[y][x] = item

	def reset(self) -> None:
		self.arr = np.zeros(shape = (Board.SIZE, Board.SIZE), dtype = np.int8)

	def get_consecutive_stones(self, start: tuple, direction: tuple) -> int:
		"""Direction in the form of (y, x)"""
		stone = self.arr[start]
		newpos = tuple(map(operator.add, start, direction))
		length = 1
		while self.get(*newpos) == stone:
			length += 1
			newpos = tuple(map(operator.add, newpos, direction))
		return length

	@staticmethod
	def get_relative_position(direction: tuple, multiplier: int):
		"""
		Retrieve a relative position from a direction
		multiplier == -1 being opposite direction
		"""
		return tuple([i * multiplier for i in direction])
