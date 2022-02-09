from math import inf
import numpy as np

from srcs.gamestate import Gamestate


class Minimax:
	transposition_table = {}

	def __init__(self, maxdepth: int = 3):
		self.maxdepth = maxdepth
		# self.alpha, self.beta = -inf, inf

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
				value, state = self.minimax(child, depth - 1, maximizing_player = False)
				if value > best_value:
					best_value, best_state = value, state
		else:
			best_value = float(inf)
			for child in state.children:
				value, state = self.minimax(child, depth - 1, maximizing_player = True)
				if value < best_value:
					best_value, best_state = value, state
		assert best_state is not None
		return best_value, best_state

	def alphabeta(self, state: Gamestate, depth: int, alpha: float, beta: float, maximizing_player: bool) -> tuple[int, Gamestate]:
		# Minimax.add_to_transp_table(gamestate)
		if depth == 0 or state.winner is not None:
			return state.h, state
		state.generate_children()
		if maximizing_player:
			best_value, best_state = -np.inf, None
			for child in state.children:
				value, state = self.alphabeta(child, depth - 1, alpha, beta, False)
				# print(f'+.at depth {depth}, alphabeta returned value {value}')
				if value > best_value:
					best_value, best_state = value, state
					# print(f'{value} > {best_value}')
				if best_value >= beta:
					print(f'beta cutoff, {value} >= {beta}')
					break  # Beta cutoff
				alpha = max(alpha, value)
				# print(f'updated alpha={alpha}')
			assert state is not None
			return best_value, best_state
		else:
			best_value, best_state = np.inf, None
			for child in state.children:
				value, state = self.alphabeta(child, depth - 1, alpha, beta, True)
				# print(f'-.at depth {depth}, alphabeta returned value {value}')
				if value < best_value:
					best_value, best_state = value, state
				if best_value <= alpha:
					print(f'alpha cutoff, {value} <= {alpha}')
					break  # Alpha cutoff
				beta = min(beta, value)
			assert state is not None
			return best_value, best_state
