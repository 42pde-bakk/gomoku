from typing import Union

import numpy as np


class Board:
	def __init__(self):
		self.arr = np.zeros(shape = (19, 19), dtype = np.int8)

	def get(self, y: int, x: int) -> Union[np.int8, None]:
		if 0 <= y < 19 and 0 <= x < 19:
			return self.arr[y][x]
		return None

	def getwithtuple(self, t: tuple[int, int]) -> Union[np.int8, None]:
		return self.get(t[0], t[1])

	def get_board(self):
		return self.arr

	def set(self, y: int, x: int, item) -> None:
		if 0 <= y < 19 and 0 <= x < 19:
			self.arr[y][x] = item
