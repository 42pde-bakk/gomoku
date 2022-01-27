from srcs.gamestate import Gamestate
from math import inf


class Minimax:
	def __init__(self, maxdepth: int = 5):
		self.maxdepth = maxdepth
		self.alpha, self.beta = -inf, inf

	def minimax(self, gamestate: Gamestate, depth: int, maximizing_player: bool):
		if depth == 0 or not gamestate.children:
			return gamestate.score
		if maximizing_player:
			value = -inf
			for child in gamestate.children:
				value = max(value, self.minimax(child, depth - 1, not maximizing_player))
		else:
			value = inf
			for child in gamestate.children:
				value = min(value, self.minimax(child, depth - 1, not maximizing_player))
		return value

	def alphabeta(self, gamestate: Gamestate, depth: int, maximizing_player: bool):
		if depth == 0 or not gamestate.children:
			return gamestate.score  # Probably will change to "return gamestate"
		if maximizing_player:
			value = -inf
			for child in gamestate.children:
				value = max(value, self.alphabeta(child, depth - 1, not maximizing_player))
				if value >= self.beta:
					break  # Beta cutoff
				self.alpha = max(self.alpha, value)
		else:
			value = inf
			for child in gamestate.children:
				value = min(value, self.alphabeta(child, depth - 1, not maximizing_player))
				if value <= self.alpha:
					break  # Alpha cutoff
				self.beta = max(self.beta, value)
		return value
