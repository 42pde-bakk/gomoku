import numpy as np


class Gamestate:
	PLAYER_ONE = 0
	PLAYER_TWO = 1

	def __init__(self):
		self.board = np.zeros(shape=(19, 19), dtype = int)
		self.score = 0
		self.parent = None
		self.children = []

	def __repr__(self):
		return self.board.tostring()

	def place_stone(self, y: int, x: int, player: int) -> None:
		self.board[y][x] = player
