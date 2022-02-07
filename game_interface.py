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
		print(self.gamestate)

	def create_window(self):
		lbl_name = ttk.Label(self, text = "Go Go Gomoku")
		lbl_name.pack()
		self.frm_board = ttk.Frame(self, relief = tk.RAISED, borderwidth = 10, height = 500, width = 500)
		self.frm_board.pack(padx = 25, pady = 25)
		self.new_game_bt()
		self.display_board()
		self.mainloop()

	def handle_click(self, args):
		i, j = args
		print(f"Clicked on row: {i}, col: {j}")
		self.play_game(i, j)
		# if self.board[i][j] == 0:
		#     self.buttons[i * self.size + j].destroy()

	def pick_color(self, i, j):
			if self.gamestate.board.get(i, j) == 0:
				button_img = self.gray
			elif self.gamestate.board.get(i, j) == 1:
				button_img = self.white
			else:
				button_img = self.black
			return button_img
  
	def display_board(self):
		for i in range(self.size):
			for j in range(self.size):
				self.frm_position.append(ttk.Frame(
					master = self.frm_board
					, relief = tk.RIDGE
					, borderwidth = 1
					))
				button_img = self.pick_color(i, j)
				self.frm_position[i * self.size + j].grid(row = i, column = j, padx = 5, pady = 5)
				self.buttons.append(tk.Button(
					master = self.frm_position[i * self.size + j],
					image = button_img,
					command = lambda row = i, column = j: self.handle_click((row, column)),
					height = 26,
					width = 26
				))
				self.buttons[i * self.size + j].pack()

	def update_button(self, i, j):
		# self.board[i][j] = self.player
		# move_is_legal(self.board, i, j)
		button_img = self.pick_color(i, j)
		self.buttons[i * self.size + j] = tk.Button(
			master = self.frm_position[i * self.size + j]\
			, image = button_img\
			, command = lambda row = i, column = j: self.handle_click((row, column))
			, height = 26
			, width = 26
			)
		self.buttons[i * self.size + j].pack()
		self.print_board()

	def change_player(self):
		if self.player == 1:
			self.player = 2
		else:
			self.player = 1
		self.gamestate.turn += 1

	def play_game(self, i, j):
		if self.gamestate.board.get(i, j) == 0:
			# if not self.gamestate.rules.move_is_legal(i, j):
			# 	print("Illegal move")
			# 	return
			self.gamestate.board.set(i, j, self.player)
			self.buttons[i * self.size + j].destroy()
			self.update_button(i, j)
		else:
			print("Position taken")
			return
		# Check for captures/win
		self.change_player()  # Changing player, so next move will be for the AI
		self.ai_move()

	def ai_move(self):
		if self.hotseat:
			value, state = self.minimax.minimax(state = self.gamestate, depth = self.minimax.maxdepth, maximizing_player = bool(self.player == 1))
			# gotta extract the last move from here?
			# Or I could just take that board and push it to the tk window, not sure tho
			# self.change_player()
		else:
			value, state = self.minimax.minimax(state = self.gamestate, depth = self.minimax.maxdepth, maximizing_player = False)
			i, j = state.last_move.x, state.last_move.y
			if self.gamestate.board.get(i, j) == 0:
				self.gamestate.board.set(i, j, self.player)
				self.buttons[i * self.size + j].destroy()
				self.update_button(i, j)
			else:
				raise ValueError()
			print(f'last_move={state.last_move}')
			self.change_player()


	def delete_buttons(self):
		for i in range(len(self.buttons)):
			self.buttons[i].destroy()
			self.frm_position[i].destroy() # Could there be different lengths here? Empty positions
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

	def update_board(self, i, j):
		print(f'updating board, i={i}, h={j}')
