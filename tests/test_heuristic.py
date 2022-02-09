import numpy as np
from unittest import TestCase
from srcs.heuristic import get_connects_of_player


class TestHeuristic(TestCase):
	def test_empty(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		assert get_connects_of_player(board, player = 1) == 0
		assert get_connects_of_player(board, player = 2) == 0

	def test_four(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[2][10:14] = 1
		assert get_connects_of_player(arr = board, player = 1) == 1000

	def test_two_fours(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[0][0:4] = 1
		board[0:4, 0] = 1
		connects = get_connects_of_player(arr = board, player = 1)
		assert connects == 2000
