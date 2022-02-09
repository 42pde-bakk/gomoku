# class Rules:
# 	def __init__(self):
# 		pass
#
# 	def move_is_legal(board, mv_row, mv_col, player):
# 		# opponent = get_opponent_num(player)
# 		# if is_last_in_capture(board, mv_row, mv_col, player, opponent):
# 		# 	return False
# 		# if is_two_open_threes():
# 		#     return False
# 		return True
#
# 	def is_two_open_threes(board, mv_row, mv_col, player):
# 		pass
#
# 	def is_last_in_capture(board, mv_row, mv_col, player, opponent):
# 		# Take care of sides of the board
# 		d = [(-1, 0), (-1, -1), (0, -1), (1, -1), (1, 0), (1, 1),  (0, 1), (-1, 1)]
# 		opp = [(1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1)]
# 		opp_plus = [(2, 0), (2, 2), (0, 2), (-2, 2), (-2, 0), (-2, -2), (0, -2), (2, -2)]
# 		for i in range(8):
# 			if board[mv_row + d[i][1]][mv_col + d[i][0]]:
# 				if board[mv_row + opp[i][1]][mv_col + opp[i][0]] == player:
# 					if board[mv_row + opp_plus[i][1]][mv_col + opp_plus[i][0]] == opponent:
# 						return True
# 		return False
#
# 	def get_opponent_num(player):
# 		if player == 1:
# 			opponent = 2
# 		else:
# 			opponent = 1
# 		return opponent
#
# 	def is_winning_condition(self):
# 		pass
#
# 	def win_by_captures(self):
# 		pass
#
# 	def win_by_five(self, y: int, x: int, player: Stone) -> bool:
# 		for dy, dx in [(1, 0), (0, 1), (1, 1), (-1, 1)]:
# 			n = m = 1
# 			while self.board.get(y + n * dy, x + n * dx) == player.value:
# 				n += 1
# 			while self.board.get(y + m * -dy, x + m * -dx) == player.value:
# 				m += 1
# 			if n + m + 1 >= 5:
# 				# self.winner = player
# 				self.captures[player.value - 1] += 10  # Win
# 				return True
# 		return False
#
# 	def can_be_captured(self):
# 		pass
