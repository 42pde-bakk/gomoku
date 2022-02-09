import tkinter as tk
from tkinter import ttk

from srcs.gamestate import Gamestate


# from srcs.board import Board
from srcs.minimax import Minimax


class Game(tk.Frame):
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
		pass
		# print(self.gamestate)

	def create_window(self):
		lbl_name = ttk.Label(self, text = "Go Go Gomoku")
		lbl_name.pack()
		self.frm_board = ttk.Frame(self, relief = tk.RAISED, borderwidth = 10, height = 500, width = 500)
		self.frm_board.pack(padx = 25, pady = 25)
		self.new_game_bt()
		self.display_board()
		self.mainloop()

	def handle_click(self, args):
		row, col = args
		print(f"Clicked on row: {row}, col: {col}")
		self.play_game(row, col)
		# if self.board[row][col] == 0:
		#     self.buttons[row * self.size + col].destroy()

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
		self.buttons[row * self.size + col] = tk.Button(
			master = self.frm_position[row * self.size + col]
			, image = button_img
			, command = lambda row = row, column = col: self.handle_click((row, column))
			, height = 26
			, width = 26
			)
		self.buttons[row * self.size + col].pack()
		self.print_board()

	def change_player(self) -> None:
		if self.player == 1:
			self.player = 2
		else:
			self.player = 1
		self.gamestate.turn += 1

	def play_game(self, row: int, col: int) -> None:
		if self.gamestate.board.get(row, col) == 0:
			# if not self.gamestate.rules.move_is_legal(row, col):
			# 	print("Illegal move")
			# 	return
			self.gamestate.board.set(row, col, self.player)
			self.buttons[row * self.size + col].destroy()
			self.update_button(row, col)
		else:
			print("Position taken")
			return
		# Check for captures/win
		self.change_player()  # Changing player, so next move will be for the AI
		self.ai_move()

	def ai_move(self):
		if self.hotseat:
			value, state = self.minimax.minimax(state = self.gamestate, depth = self.minimax.maxdepth, maximizing_player = bool(self.player == 1))
		else:
			value, state = self.minimax.minimax(state = self.gamestate, depth = self.minimax.maxdepth, maximizing_player = False)
			col, row = state.first_move.x, state.first_move.y
			if self.gamestate.board.get(y = row, x = col) == 0:
				self.gamestate.board.set(y = row, x = col, item = self.player)
				self.buttons[row * self.size + col].destroy()
				self.update_button(row, col)
			else:
				raise ValueError()
			self.change_player()

	def delete_buttons(self):
		for row in range(len(self.buttons)):
			self.buttons[row].destroy()
			self.frm_position[row].destroy() # Could there be different lengths here? Empty positions
		self.buttons = []
		self.frm_position = []

	def reset_board(self):
		self.player = 1
		self.gamestate = Gamestate()
		self.delete_buttons()
		self.display_board()

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

	def update_board(self, row, col):
		print(f'updating board, row={row}, h={col}')
