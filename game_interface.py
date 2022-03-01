import time
import numpy as np
import tkinter as tk
from tkinter import ttk
from srcs.gamestate import Gamestate, Stone
from srcs.rules import Rules
from srcs.minimax import Minimax

class Game(tk.Frame):
	rules = Rules()

	def __init__(self, size, hotseat: bool, master = None):
		super().__init__(master)
		self.pack()
		self.gamestate = Gamestate()
		self.buttons = []
		self.frm_position = []
		self.size = size
		self.player = 1
		self.minimax = Minimax()
		self.hotseat = hotseat
		self.frm_board = None
		self.white = tk.PhotoImage(file = 'assets/white.png')
		self.black = tk.PhotoImage(file = 'assets/black.png')
		self.gray = tk.PhotoImage(file = 'assets/gray.png')

	def print_board(self):
		print(self.gamestate.board.get_board())

	def create_window(self):
		lbl_name = ttk.Label(self, text = "Go Go Gomoku")
		lbl_name.pack()
		self.frm_board = ttk.Frame(self, relief = tk.RAISED, borderwidth = 10, height = 500, width = 500)
		self.frm_board.pack(padx = 25, pady = 25)
		self.new_game_bt()
		self.display_board()
		self.display_captures()
		self.mainloop()

	def handle_click(self, args):
		row, col = args
		print(f"Clicked on row: {row}, col: {col}")
		self.play_game(row, col)

	def pick_color(self, row: int, col: int):
		if self.gamestate.board.get(row, col) == 0:
			button_img = self.gray
		elif self.gamestate.board.get(row, col) == 1:
			button_img = self.white
		else:
			button_img = self.black
		return button_img

	def display_board(self) -> None:
		for row in range(self.size):
			for col in range(self.size):
				self.frm_position.append(ttk.Frame(
					master = self.frm_board
					, relief = tk.RIDGE
					, borderwidth = 1
					))
				button_img = self.pick_color(row, col)
				self.frm_position[row * self.size + col].grid(row = row, column = col, padx = 5, pady = 5)
				self.buttons.append(tk.Button(
					master = self.frm_position[row * self.size + col],
					image = button_img,
					command = lambda row = row, column = col: self.handle_click((row, column)),
					height = 26,
					width = 26
				))
				self.buttons[row * self.size + col].pack()

	def update_button(self, row: int, col: int) -> None:
		button_img = self.pick_color(row, col)
		self.buttons[row * self.size + col].config(image=button_img)
		self.print_board()

	def change_player(self) -> None:
		if self.player == 1:
			self.player = 2
		else:
			self.player = 1

	def play_game(self, row: int, col: int) -> None:
		if self.gamestate.board.get(row, col) == 0:
			if not Game.rules.is_legal_move(row, col, self.player, self.gamestate.board.get_board()):
				print("Illegal move")
				return
			self.gamestate.place_stone(y = row, x = col, stone = self.player)
			self.update_button(row, col)
		else:
			print("Position taken")
			return
		# Check for captures/win
		self.handle_captures(row, col)
		Game.rules.is_winning_condition(row, col, self.player, self.gamestate.board.get_board(), self.gamestate.captures)
		self.change_player()  # Changing player, so next move will be for the AI
		self.ai_move()

	def ai_move(self):
		self.gamestate.moves.clear()
		if self.hotseat:
			value, state = self.minimax.minimax(state = self.gamestate, depth = self.minimax.maxdepth, maximizing_player = bool(self.player == 1))
		else:
			time_start = time.time()
			value, state = self.minimax.alphabeta(state = self.gamestate, depth = 2, α = -np.inf, β = np.inf, maximizing_player = False)
			col, row = state.moves[0].x, state.moves[0].y
			print(f'In {time.time() - time_start:.2f}s the AI decided to move to y,x={row, col}, heur={state.h}')
			print(f'moves: {state.moves}')
			if self.gamestate.board.get(y = row, x = col) == 0:
				self.handle_captures(row, col)
				# self.buttons[row * self.size + col].destroy()
				self.gamestate.place_stone(y = row, x = col, stone = self.player)
				self.update_button(row, col)
				while state.parent != self.gamestate:
					state = state.parent
				if state.winner:
					exit(1)
			else:
				raise ValueError()
			self.change_player()

	def reset_pieces(self):
		button_img = self.gray
		for row in range(self.size):
			for col in range(self.size):
				self.buttons[row * self.size + col].config(image=button_img)

	def delete_buttons(self):
		for row in range(len(self.buttons)):
			self.buttons[row].destroy()
			self.frm_position[row].destroy()  # Could there be different lengths here? Empty positions
		self.buttons = []
		self.frm_position = []

	def reset_board(self):
		self.player = 1
		self.gamestate = Gamestate()
		self.reset_pieces()
		# self.update_captures()

	def new_game_bt(self):
		bt_new_game = tk.Button(
			text = "NEW GAME",
			command = self.reset_board,
			width = 25,
			height = 5,
			bg = "gray",
			fg = "red"
		)
		bt_new_game.pack()

	def handle_captures(self, row, col):
		capture_check = Game.rules.is_capturing(row, col, self.player, self.gamestate.board)
		if capture_check is not None:
			self.gamestate.capture(capture_check[0], capture_check[1], self.player)
			self.update_captures(capture_check[0], capture_check[1])

	def remove_captured(self, pos1: tuple, pos2: tuple):
		pos1_row, pos1_col = pos1
		pos2_row, pos2_col = pos2
		self.buttons[pos1_row * self.size + pos1_col].config(image=self.gray)
		self.buttons[pos2_row * self.size + pos2_col].config(image=self.gray)

	def update_captures(self, pos1: tuple, pos2: tuple):
		self.remove_captured(pos1, pos2)
		self.lbl_captures1.configure(text=f"Player {self.player} has {self.gamestate.captures[self.player]} captures")
		self.lbl_captures2.configure(text=f"Player {self.player - 1} has {self.gamestate.captures[self.player - 1]} captures")

	def display_captures(self):
		self.lbl_captures1 = ttk.Label(self,
								text=f"Player {self.player} has {self.gamestate.captures[self.player - 1]} captures")
		self.lbl_captures1.pack()
		self.lbl_captures2 = ttk.Label(self,
								text=f"Player {self.player - 1} has {self.gamestate.captures[self.player - 1]} captures")
		self.lbl_captures2.pack()

	def update_board(self, row, col):
		print(f'updating board, row={row}, h={col}')
