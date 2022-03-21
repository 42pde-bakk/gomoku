from math import inf
import numpy as np

from srcs.gamestate import Gamestate


class Minimax:
	transposition_table = {}

	def __init__(self, maxdepth: int = 2):
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

	def minimax(self, state: Gamestate, depth: int, maximizing_player: bool) -> Gamestate:
		best_state = Gamestate()
		# Minimax.add_to_transp_table(gamestate, maximizing_player)
		if depth == 0 or state.winner is not None:  # state is a terminal state
			return state
		if not state.generate_children():
			return state
		if maximizing_player:
			best_state.h = -np.inf
			for child in state.children:
				state = self.minimax(child, depth - 1, maximizing_player = False)
				if state > best_state:
					best_state = state
		else:
			best_state.h = np.inf
			for child in state.children:
				state = self.minimax(child, depth - 1, maximizing_player = True)
				if state < best_state:
					best_state = state
		return best_state

	# Player 0 (User) tries to maximize the score
	# while Player 1 (bot) tries to minimize the score
	def alphabeta(self, state: Gamestate, depth: int, α: float, β: float, maximizing_player: bool) -> Gamestate:
		# Minimax.add_to_transp_table(gamestate)
		best_state = Gamestate()
		if depth == 0 or state.winner is not None:
			return state
		if not state.generate_children():
			return state
		if maximizing_player:
			best_state.h = -np.inf
			for child in state.children:
				state = self.alphabeta(child, depth - 1, α, β, False)
				if state > best_state:
					best_state = state
				if best_state.h >= β:
					break  # β cutoff
				α = max(α, state.h)
		else:
			best_state.h = np.inf
			for child in state.children:
				state = self.alphabeta(child, depth - 1, α, β, True)
				if state < best_state:
					best_state = state
				if best_state.h <= α:
					break  # α cutoff
				β = min(β, state.h)
		return best_state
