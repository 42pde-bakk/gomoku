import numpy as np

from unittest import TestCase
from srcs.board import Board


class TestBoard(TestCase):
	def test_reset(self):
		board = Board()
		board.set(7, 9, 8)
		board.set(6, 10, 7)
		board.set(5, 11, 9)
		board.set(4, 12, 10)
		board.reset()
		assert np.array_equal(board.arr, np.zeros(shape = (Board.SIZE, Board.SIZE), dtype = np.int8))

	def test_get(self):
		board = Board()
		assert board.get(y = 5, x = 4) == 0

	def test_get_bad(self):
		board = Board()
		assert board.get(y = 5, x = -1) is None
		assert board.get(y = 5, x = 20) is None
		assert board.get(y = -2, x = 5) is None
		assert board.get(y = 21, x = 8) is None

	def test_get_board(self):
		board = Board()
		assert isinstance(board.get_board(), np.ndarray)

	def test_set(self):
		board = Board()
		x, y, item = 6, 9, 69
		board.set(y = y, x = x, item = item)
		assert board.get(y = y, x = x) == item

	def test_set_bad(self):
		board = Board()
		board.set(y = 7, x = -1, item = 7)
		assert board.get(y = 7, x = 19 - 1) == 0

	def test_get_consecutive_stones_empty(self):
		board = Board()
		board.arr[(0, 0)] = 1
		board.arr[(4, 5)] = 2
		assert board.get_consecutive_stones(start = (0, 0), direction = (1, 0)) == 1
		assert board.get_consecutive_stones(start = (0, 0), direction = (-1, 0)) == 1
		assert board.get_consecutive_stones(start = (0, 0), direction = (1, 1)) == 1
		assert board.get_consecutive_stones(start = (0, 0), direction = (-1, -1)) == 1
		assert board.get_consecutive_stones(start = (4, 5), direction = (1, 0)) == 1
		assert board.get_consecutive_stones(start = (4, 5), direction = (-1, 0)) == 1
		assert board.get_consecutive_stones(start = (4, 5), direction = (1, 1)) == 1
		assert board.get_consecutive_stones(start = (4, 5), direction = (-1, -1)) == 1

	def test_get_consecutive_stones(self):
		board = Board()
		board.arr[(0, 0)] = 1
		board.arr[(0, 1)] = 1
		board.arr[(0, 2)] = 1
		board.arr[(0, 3)] = 1
		board.arr[(0, 4)] = 1
		assert board.get_consecutive_stones(start = (0, 0), direction = (0, 1)) == 5
		assert board.get_consecutive_stones(start = (0, 4), direction = (0, -1)) == 5
