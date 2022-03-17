from srcs.gamestate import Stone
import numpy as np
from srcs.board import Board
from typing import Union


class Rules:
	def __init__(self):
		self.winner = None

	def is_legal_move(self, row, col, player, board):
		if self.is_last_in_capture(row, col, player, board):
			return False
		# opponent = get_opponent_num(player)
		# if is_last_in_capture(board, mv_row, mv_col, player, opponent):
		# 	return False
		# if is_two_open_threes():
		#     return False
		return True
	#
	# def is_two_open_threes(board, mv_row, mv_col, player):
	# 	pass
	#

	@staticmethod
	def opponent_value(player):
		if player == 2:
			return 1
		return 2

	def is_last_in_capture(self, row, col, player, board):
		# Take care of sides of the board
		opponent = self.opponent_value(player)
		d = [(-1, 0), (-1, -1), (0, -1), (1, -1), (1, 0), (1, 1),  (0, 1), (-1, 1)]
		opp = [(1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1)]
		opp_plus = [(2, 0), (2, 2), (0, 2), (-2, 2), (-2, 0), (-2, -2), (0, -2), (2, -2)]
		for i in range(8):
			if board[row + d[i][1]][col + d[i][0]]:
				if board[row + opp[i][1]][col + opp[i][0]] == player:
					if board[row + opp_plus[i][1]][col + opp_plus[i][0]] == opponent:
						return True
		return False

	def is_winning_condition(self, row: int, col: int, player: int, board: np.ndarray, captures: list) -> bool:
		if self.win_by_five(row, col, player, board):
			# check if can be captured next move.
			print('WIN \n\n\n\n\n\n\nby five in a row for player: ' + str(player))
			return True
		elif self.win_by_captures(player, captures):
			print('WIN by captures')
			return True
		return False

	@staticmethod
	def win_by_captures(player: int, captures: list) -> bool:
		return bool(captures[player - 1] >= 10)

	@staticmethod
	def win_by_five(row: int, col: int, player: int, board: np.ndarray) -> bool:
		d = [(-1, 0), (-1, -1), (0, -1), (1, -1)]
		for d_col, d_row in d:
			n = 1
			n_opp = -1
			while board[row + (n * d_row)][col + (n * d_col)] and board[row + (n * d_row)][col + (n * d_col)] == player:
				n += 1
			while board[row + (n_opp * d_row)][col + (n_opp * d_col)]\
				and board[row + (n_opp * d_row)][col + (n_opp * d_col)] == player:
				n_opp -= 1
			if n + abs(n_opp) - 1 >= 5:
				return True
		return False

	@staticmethod
	def player_check(row: int, col: int, player_to_check: int, board: Board) -> bool:
		return row < 0 or row >= 19 or col < 0 or col >= 19 or board.get(row, col) != player_to_check

	# Add board to Rules class
	def is_capturing(self, row: int, col: int, player: int, board: Board) -> Union[list, None]:
		opponent = self.opponent_value(player)
		for dy, dx in [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1)]:
			if not self.player_check(row + dy, col + dx, opponent, board)\
					and not self.player_check(row + 2 * dy, col + 2 * dx, opponent, board)\
					and not self.player_check(row + 3 * dy, col + 3 * dx, player, board):
				print("CAPTURED\n\n\n\n")
				print(f'x1:{col + dx}, y1:{row + dy}\nx2:{row + 2 * dy}, y2:{col + 2 * dx}')
				print('Player:', player)
				return [(row + dy, col + dx), (row + 2 * dy, col + 2 * dx)]
		return None
