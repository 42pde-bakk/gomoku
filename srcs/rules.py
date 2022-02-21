from srcs.gamestate import Stone
import numpy as np


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
			opponent = 1
		else:
			opponent = 2
		return opponent

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

	#
	# def get_opponent_num(player):
	# 	if player == 1:
	# 		opponent = 2
	# 	else:
	# 		opponent = 1
	# 	return opponent

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
	def win_by_captures(player: int, captures: list):
		if captures[player - 1] == 10:
			return True
		return False

	@staticmethod
	def win_by_five(row: int, col: int, player: int, board: np.ndarray) -> bool:
		d = [(-1, 0), (-1, -1), (0, -1), (1, -1)]
		for d_col, d_row in d:
			n = 1
			n_opp = -1
			while board[row + (n * d_row)][col + (n * d_col)] and board[row + (n * d_row)][col + (n * d_col)] == player:
				n += 1
			while board[row + (n_opp * d_row)][col + (n_opp * d_col)] and board[row + (n_opp * d_row)][col + (n_opp * d_col)] == player:
				n_opp -= 1
			if n + abs(n_opp) - 1 >= 5:
				return True
		return False

	# def win_by_five(self, row: int, col: int, player: int, board: np.ndarray) -> bool:
	# 	for dy, dx in [(1, 0), (0, 1), (1, 1), (-1, 1)]:
	# 		n = m = 1
	# 		print(player)
	# 		while board[row + n * dy][col + n * dx] and board[row + n * dy][col + n * dx] == player:
	# 			n += 1
	# 		while board[row + m * -dy][col + m * -dx] and board[row + n * dy][col + n * dx] == player:
	# 			m += 1
	# 		if n + m >= 5:
	# 			self.winner = player
	# 			# self.captures[player.value - 1] += 10  # Win
	# 			return True
	# 	return False

	def can_be_captured(self):
		pass
