import numpy as np
from unittest import TestCase
from srcs.heuristic import get_connects_of_player


class TestHeuristic(TestCase):
	def test_empty(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		connects1, _ = get_connects_of_player(board, player = 1)
		connects2, _ = get_connects_of_player(board, player = 2)
		assert connects1 == connects2 == 0

	def test_three_right(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[2][10:13] = 2
		connects, _ = get_connects_of_player(arr = board, player = 2)
		assert connects == 100

	def test_three_down(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[2][10:13] = 2
		connects1, _ = get_connects_of_player(arr = board, player = 2)
		assert connects1 == 100

	def test_three_diagonal(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[range(2, 5), range(2, 5)] = 2
		connects, _ = get_connects_of_player(arr = board, player = 2)
		assert connects == 100

	def test_four_right(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[2][10:14] = 1
		connects, _ = get_connects_of_player(arr = board, player = 1)
		assert connects == 1000

	def test_four_down(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[2][10:14] = 1
		connects, _ = get_connects_of_player(arr = board, player = 1)
		assert connects == 1000

	def test_four_diagonal(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[range(2, 6), range(2, 6)] = 1
		connects, _ = get_connects_of_player(arr = board, player = 1)
		assert connects == 1000

	def test_two_fours(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[0][0:4] = 1
		board[0:4, 0] = 1
		connects, _ = get_connects_of_player(arr = board, player = 1)
		assert connects == 2000

	def test_two_players(self):
		board = np.zeros(shape=(19, 19), dtype = np.int8)
		board[0][0] = 1
		board[0][1:4] = 2
		board[1][1:3] = 1
		connects1, _ = get_connects_of_player(board, player = 1)
		connects2, _ = get_connects_of_player(board, player = 2)
		assert connects1 - connects2 < 0

	def test_custom(self):
		board = np.zeros(shape = (19, 19), dtype = np.int8)
		board[9][9:11] = 1
		board[9][11] = 2
		board[8][12] = 2
		connects1, _ = get_connects_of_player(board, player = 1)
		connects2, _ = get_connects_of_player(board, player = 2)
		print(board, connects1 - connects2)
		assert connects1 - connects2 == 0
