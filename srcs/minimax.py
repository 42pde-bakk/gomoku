from srcs.gamestate import Gamestate, Stone
from math import inf


class Minimax:
	transposition_table = {}

	def __init__(self, maxdepth: int = 3):
		self.maxdepth = maxdepth
		self.alpha, self.beta = -inf, inf

	@staticmethod
	def add_to_transp_table(gamestate: Gamestate, maximizing_player: bool) -> None:
		board_bytes = gamestate.board.tobytes()
		if board_bytes in Minimax.transposition_table.keys():
			gamestate.children = Minimax.transposition_table[board_bytes].children
		else:
			gamestate.generate_children((int(maximizing_player) + 1))
			Minimax.transposition_table[board_bytes] = gamestate

	def minimax(self, gamestate: Gamestate, depth: int, maximizing_player: bool) -> int:
		Minimax.add_to_transp_table(gamestate, maximizing_player)
		if depth == 0 or not gamestate.children:
			return gamestate.h
		if maximizing_player:
			value = -inf
			for child in gamestate.children:
				value = max(value, self.minimax(child, depth - 1, not maximizing_player))
		else:
			value = inf
			for child in gamestate.children:
				value = min(value, self.minimax(child, depth - 1, not maximizing_player))
		return value

	def alphabeta(self, gamestate: Gamestate, depth: int, maximizing_player: bool) -> int:
		Minimax.add_to_transp_table(gamestate, maximizing_player)
		if depth == 0 or not gamestate.children:
			return gamestate.h  # Probably will change to "return gamestate"
		if maximizing_player:
			value = -inf
			for child in gamestate.children:
				value = max(value, self.alphabeta(child, depth - 1, not maximizing_player))
				if value >= self.beta:
					print(f'beta cutoff, {value} >= {self.beta}')
					break  # Beta cutoff
				self.alpha = max(self.alpha, value)
		else:
			value = inf
			for child in gamestate.children:
				value = min(value, self.alphabeta(child, depth - 1, not maximizing_player))
				if value <= self.alpha:
					print(f'alpha cutoff, {value} <= {self.alpha}')
					break  # Alpha cutoff
				self.beta = max(self.beta, value)
		return value
