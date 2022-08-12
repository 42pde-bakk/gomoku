from srcs.gamestate import Stone
import numpy as np
from srcs.board import Board
from typing import Union
from copy import deepcopy

class Rules:
	def __init__(self):
		self.winner = None
		self.dir = [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1)]
		self.opp = [Board.get_relative_position(direction, -1) for direction in self.dir]
		self.opp_val = [Board.get_relative_position(direction, -2) for direction in self.dir]
		self.winning_five_indices = None
		self.horizontal = [(-2, -2), (-2, -1), (-2, 0), (-2, 1), (-2, 2), (-2, 3), (-2, 4), (-2, 5)\
							, (-2, 6), (-1, -1), (-1, 0), (-1, 1), (-1, 2), (-1, 3), (-1, 4), (-1, 5)\
							, (1, -1), (1, 0), (1, 1), (1, 2), (1, 3), (1, 4), (1, 5), (2, -2), (2, -1)\
							, (2, 0), (2, 1), (2, 2), (2, 3), (2, 4), (2, 5), (2, 6)]

		self.vertical = [(-2, -2), (-1, -2), (0, -2), (1, -2), (2, -2), (3, -2), (4, -2), (5, -2)\
							, (6, -2), (-1, -1), (0, -1), (1, -1), (2, -1), (3, -1), (4, -1), (5, -1)\
							, (-1, 1), (0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (-2, 2), (-1, 2)\
							, (0, 2), (1, 2), (2, 2), (3, 2), (4, 2), (5, 2), (6, 2)]

		self.negative_slope = [(0, -1), (1, 0), (2, 1), (3, 2), (4, 3), (5, 4), (0, -2), (1, -1)\
							, (2, 0), (3, 1), (4, 2), (5, 3), (6, 4), (2, -2), (3, -1), (4, 0)\
							, (5, 1), (6, 2), (-1, 0), (0, 1), (1, 2), (2, 3), (3, 4), (4, 5)\
							, (-2, 0), (-1, 1), (0, 2), (1, 3), (2, 4), (3, 5), (4, 6), (-2, 2)\
							, (-1, 3), (0, 4), (1, 5), (2, 6)]

		self.positive_slope = [(0, -1), (-1, 0), (-2, 1), (-3, 2), (-4, 3), (-5, 4), (0, -2), (-1, -1)\
							, (-2, 0), (-3, 1), (-4, 2), (-5, 3), (-6, 4), (-2, -2), (-3, -1), (-4, 0), (-5, 1)\
							, (-6, 2), (1, 0), (0, 1), (-1, 2), (-2, 3), (-3, 4), (-4, 5), (2, 0), (1, 1), (0, 2)\
							, (-1, 3), (-2, 4), (-3, 5), (-4, 6), (2, 2), (1, 3), (0, 4), (-1, 5), (-2, 6)]

	def is_legal_move(self, row: int, col: int, player: int, board: Board) -> bool:
		if self.is_two_open_threes(row, col, player, board):
			if self.is_capturing(row, col, player, board):
				return True
			return False
		return True

	def is_open_three(self, row: int, col: int, player: int, board: Board, d: tuple) -> bool:
		inside_zero = False
		first_zero = False
		stones = 0
		for i in range(1, 5):
			rel = board.get_relative_position(d, i)
			if stones == 2:
				if board.get(row + rel[0], col + rel[1]):
					return False
				break
			if board.get(row + rel[0], col + rel[1]) == 0:
				if inside_zero:
					break
				if first_zero:
					break
				first_zero = True
			elif not self.is_not_player_check(row + rel[0], col + rel[1], player, board):
				stones += 1
				if first_zero:
					inside_zero = True
				first_zero = False
			else:
				if first_zero:
					break
				return False
		first_zero = False
		for i in range(-1, -5, -1):
			rel = board.get_relative_position(d, i)
			if stones == 2:
				if board.get(row + rel[0], col + rel[1]):
					return False
				elif inside_zero:
					return True
			if board.get(row + rel[0], col + rel[1]) == 0:
				if inside_zero:
					return False
				if first_zero:
					inside_zero = True
					first_zero = False
				else:
					first_zero = True
			elif not self.is_not_player_check(row + rel[0], col + rel[1], player, board):
				stones += 1
				if first_zero:
					inside_zero = True
			else:
				return False
		return True

	def is_two_open_threes(self, row: int, col: int, player: int, board: Board) -> bool:
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

	def is_winning_condition(self, row: int, col: int, player: int, board: Board, captures: list) -> bool:
		opponent = self.opponent_value(player)
		if self.win_by_five(row, col, player, board):
			if not self.five_can_be_broken(opponent, board):
				print('WIN \nby five in a row for player: ' + str(player))
				return True
		if self.win_by_captures(player, captures):
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
				self.save_five_beginning_indices((d_col, d_row), row, col, n, n_opp)
				return True
		return False

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

	def save_five_beginning_indices(self, direction, row, col, n, n_opp):
		"""
		For Horizontal and slopes the beginning is the left most position. For Vertical it's uppermost position.
		:param direction: (-1, 0) hor(_); (-1, -1) neg(\); (0, -1) ver(|); (1, -1) pos(/)
		:param n: multiplier for direction
		:param n_opp: multiplier for direction for positive slope
		"""
		d_col, d_row = direction
		if direction == (-1, 0):
			first_row, first_col = row + (n * d_row), col + 1 + (n * d_col)
		elif direction == (-1, -1):
			first_row, first_col = row + 1 + (n * d_row), col + 1 + (n * d_col)
		elif direction == (0, -1):
			first_row, first_col = row + 1 + (n * d_row), col + (n * d_col)
		else:
			first_row, first_col = row - 1 + (n_opp * d_row), col + 1 + (n_opp * d_col)
		self.winning_five_indices = (first_row, first_col, direction)

	def get_possible_moves(self, direction: tuple):
		if direction == (-1, 0):
			possible_moves = self.horizontal
		elif direction == (-1, -1):
			possible_moves = self.negative_slope
		elif direction == (0, -1):
			possible_moves = self.vertical
		else:
			possible_moves = self.positive_slope
		return possible_moves

	@staticmethod
	def place_on_test_board(captures, test_board: Board, value: int):
		for i in range(int(len(captures) // 2)):
			i = i * 2
			pos1_y, pos1_x = captures[i]
			pos2_y, pos2_x = captures[i + 1]
			test_board.set(pos1_y, pos1_x, value)
			test_board.set(pos2_y, pos2_x, value)

	def has_breaking_move(self, row: int, col: int, possible_moves, player: int, board: Board) -> bool:
		opponent = self.opponent_value(player)
		test_board = deepcopy(board)
		for place in possible_moves:
			rel_row, rel_col = place
			if test_board.get(row + rel_row, col + rel_col) == 0:
				captures = self.is_capturing(row + rel_row, col + rel_col, player, test_board)
				if captures is not None:
					self.place_on_test_board(captures, test_board, Stone.EMPTY.value)
					if test_board.get(row, col) == 0:
						return True
					if self.win_by_five(row, col, opponent, test_board):
						self.place_on_test_board(captures, test_board, opponent)
					else:
						return True
		return False

	def five_can_be_broken(self, player: int, board: Board) -> bool:
		row, col, direction = self.winning_five_indices
		possible_moves = self.get_possible_moves(direction)
		has_breaking_move = self.has_breaking_move(row, col, possible_moves, player, board)
		return has_breaking_move

	def is_breaking_move(self, row: int, col: int, player: int, five_indices, board: Board):
		captures = self.is_capturing(row, col, player, board)
		if captures is None:
			return False
		test_board = deepcopy(board)
		self.place_on_test_board(captures, test_board, Stone.EMPTY.value)
		opponent = self.opponent_value(player)
		five_row, five_col = five_indices
		if test_board.get(five_row, five_col) != 0 and self.win_by_five(five_row, five_col, opponent, test_board):
			return False
		return True
