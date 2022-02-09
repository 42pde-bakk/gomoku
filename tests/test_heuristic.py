import numpy as np
from unittest import TestCase
from srcs.heuristic import get_connects_of_player


class TestHeuristic(TestCase):
	def test_empty(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		assert get_connects_of_player(board, player = 1) == 0
		assert get_connects_of_player(board, player = 2) == 0

	def test_three_right(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[2][10:13] = 2
		assert get_connects_of_player(arr = board, player = 2) == 100

	def test_three_down(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[2][10:13] = 2
		assert get_connects_of_player(arr = board, player = 2) == 100

	def test_three_diagonal(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[range(2, 5), range(2, 5)] = 2
		connects = get_connects_of_player(arr = board, player = 2)
		assert connects == 100

	def test_four_right(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[2][10:14] = 1
		assert get_connects_of_player(arr = board, player = 1) == 1000

	def test_four_down(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[2][10:14] = 1
		assert get_connects_of_player(arr = board, player = 1) == 1000

	def test_four_diagonal(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[range(2, 6), range(2, 6)] = 1
		connects = get_connects_of_player(arr = board, player = 1)
		assert connects == 1000

	def test_two_fours(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[0][0:4] = 1
		board[0:4, 0] = 1
		connects = get_connects_of_player(arr = board, player = 1)
		assert connects == 2000

	def test_two_players(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[0][0] = 1
		board[0][1:4] = 2
		board[1][1:3] = 1
		connects1 = get_connects_of_player(board, player = 1)
		connects2 = get_connects_of_player(board, player = 2)
		assert connects1 - connects2 < 0
