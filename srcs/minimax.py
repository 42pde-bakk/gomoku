from math import inf

from srcs.gamestate import Gamestate


class Minimax:
	transposition_table = {}

	def __init__(self, maxdepth: int = 1):
		self.maxdepth = maxdepth
		self.alpha, self.beta = -inf, inf

	@staticmethod
	def add_to_transp_table(gamestate: Gamestate) -> None:
		board_bytes = gamestate.board.arr.tobytes()
		if board_bytes in Minimax.transposition_table.keys():
			gamestate.children = Minimax.transposition_table[board_bytes].children
		else:
			gamestate.generate_children()
			Minimax.transposition_table[board_bytes] = gamestate

	def minimax(self, state: Gamestate, depth: int, maximizing_player: bool) -> tuple[int, Gamestate]:
		best_state = None
		# Minimax.add_to_transp_table(gamestate, maximizing_player)
		if depth == 0 or state.winner is not None:  # state is a terminal state
			return state.h, state
		state.generate_children()
		if maximizing_player:
			best_value = float(-inf)
			for child in state.children:
				value, state = self.minimax(child, depth - 1, not maximizing_player)
				if value > best_value:
					best_value, best_state = value, state
		else:
			best_value = float(inf)
			for child in state.children:
				value, state = self.minimax(child, depth - 1, not maximizing_player)
				if value < best_value:
					best_value, best_state = value, state
		return best_value, best_state

	def alphabeta(self, state: Gamestate, depth: int, maximizing_player: bool) -> tuple[int, Gamestate]:
		best_state = None
		# Minimax.add_to_transp_table(gamestate)
		if depth == 0 or state.winner is not None:
			return state.h, state  # Probably will change to "return gamestate"
		state.generate_children()
		exit(1)
		if maximizing_player:
			for child in state.children:
				value, state = self.alphabeta(child, depth - 1, not maximizing_player)
				if value >= self.beta:
					print(f'beta cutoff, {value} >= {self.beta}')
					break  # Beta cutoff
				if value > self.alpha:
					self.alpha, best_state = value, state
			return self.alpha, state
		else:
			for child in state.children:
				value, state = self.alphabeta(child, depth - 1, not maximizing_player)
				if value <= self.alpha:
					print(f'alpha cutoff, {value} <= {self.alpha}')
					break  # Alpha cutoff
				if value > self.beta:
					self.beta, best_state = value, state
			return self.beta, state
