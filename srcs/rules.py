from srcs.gamestate import Stone
import numpy as np
from srcs.board import Board
from typing import Union

class Rules:
	def __init__(self):
		self.winner = None
		self.dir = [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1)]
		self.opp = [Board.get_relative_position(direction, -1) for direction in self.dir]
		self.opp_val = [Board.get_relative_position(direction, -2) for direction in self.dir]

	def is_legal_move(self, row: int, col: int, player: int, board: Board):
		if self.is_last_in_capture(row, col, player, board):
			return False
		if self.is_two_open_threes(row, col, player, board):
			return False
		return True

	def is_open_three(self, row: int, col: int, player: int, board: Board, d: tuple) -> bool:
		inside_zero = False
		stones = 0
		for i in range(1, 5):
			rel = board.get_relative_position(d, i)
			if stones == 2 and board.get(row + rel[0], col + rel[1]):
				return False
			elif stones == 2:
				return True
			if not self.is_not_player_check(row + rel[0], col + rel[1], player, board) or not board.get(row + rel[0], col + rel[1]):
				if not board.get(row + rel[0], col + rel[1]):
					if inside_zero:
						continue
					inside_zero = True
				else:
					stones += 1
		for i in range(-1, -4, -1):
			rel = board.get_relative_position(d, i)
			if stones == 2 and board.get(row + rel[0], col + rel[1]):
				return False
			elif stones == 2:
				return True
			if not self.is_not_player_check(row + rel[0], col + rel[1], player, board) or not board.get(row + rel[0], col + rel[1]):
				if not board.get(row + rel[0], col + rel[1]):
					if inside_zero:
						return False
					else:
						inside_zero = True
				else:
					stones += 1
		return False

	def is_two_open_threes(self, row: int, col: int, player: int, board: Board) -> bool:
		pass
		second_three = False
		for direction in self.dir[:4]:
			if self.is_open_three(row, col, player, board, direction):
				if second_three:
					return True
				second_three = True
		return False

	@staticmethod
	def opponent_value(player: int) -> int:
		if player == 2:
			return 1
		return 2

	def is_last_in_capture(self, row: int, col: int, player: int, board: Board) -> bool:
		# Take care of sides of the board
		opponent = self.opponent_value(player)
		d = [(-1, 0), (-1, -1), (0, -1), (1, -1), (1, 0), (1, 1), (0, 1), (-1, 1)]
		opp = [(1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1)]
		opp_plus = [(2, 0), (2, 2), (0, 2), (-2, 2), (-2, 0), (-2, -2), (0, -2), (2, -2)]
		for i in range(8):
			if not self.is_not_player_check(row + d[i][1], col + d[i][0], opponent, board):
				if not self.is_not_player_check(row + opp[i][1], col + opp[i][0], player, board):
					if not self.is_not_player_check(row + opp_plus[i][1], col + opp_plus[i][0], opponent, board):
						return True
		return False

	def is_winning_condition(self, row: int, col: int, player: int, board: Board, captures: list) -> bool:
		if self.win_by_five(row, col, player, board):
			# check if can be captured next move.
			print('WIN \nby five in a row for player: ' + str(player))
			return True
		elif self.win_by_captures(player, captures):
			print('WIN \nby 10 captured stones for player: ' + str(player))
			return True
		return False

	@staticmethod
	def win_by_captures(player: int, captures: list) -> bool:
		if captures[player - 1] == 10:
			return True
		return False

	@staticmethod
	def is_not_player_check(row: int, col: int, player_to_check: int, board: Board) -> bool:
		return row < 0 or row >= 19 or col < 0 or col >= 19 or board.get(row, col) != player_to_check

	def win_by_five(self, row: int, col: int, player: int, board: np.ndarray) -> bool:
		d = [(-1, 0), (-1, -1), (0, -1), (1, -1)]
		for d_col, d_row in d:
			n = 1
			n_opp = -1
			while not self.is_not_player_check(row + (n * d_row), col + (n * d_col), player, board):
				n += 1
			while not self.is_not_player_check(row + (n_opp * d_row), col + (n_opp * d_col), player, board):
				n_opp -= 1
			if n + abs(n_opp) - 1 >= 5:
				return True
		return False

	# Add board to Rules class
	def is_capturing(self, row: int, col: int, player: int, board: Board) -> Union[list, None]:
		opponent = self.opponent_value(player)
		first_capture = []
		for d in range(8):
			if not self.is_not_player_check(row + self.dir[d][0], col + self.dir[d][1], opponent, board) \
				and not self.is_not_player_check(row + 2 * self.dir[d][0], col + 2 * self.dir[d][1], opponent, board) \
				and not self.is_not_player_check(row + 3 * self.dir[d][0], col + 3 * self.dir[d][1], player, board):
				first_capture = first_capture + [(row + self.dir[d][0], col + self.dir[d][1]), (row + 2 * self.dir[d][0], col + 2 * self.dir[d][1])]
		if len(first_capture):
			return first_capture
		return None
