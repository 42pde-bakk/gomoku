from typing import Union

import numpy as np


class Board:
	SIZE = 19

	def __init__(self):
		self.arr = np.zeros(shape = (Board.SIZE, Board.SIZE), dtype = np.int8)

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
