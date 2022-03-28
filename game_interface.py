import time
import enum
import numpy as np
import tkinter as tk
from tkinter import ttk
from srcs.gamestate import Gamestate, Stone
from srcs.rules import Rules
from srcs.minimax import Minimax
from srcs.bot_socket import BotSocket
from srcs.gui import congratulate_winner


class GameMode(enum.IntEnum):
	VERSUS_AI = 1
	HOTSEAT = 2
	BOT_POT = 3

	def __str__(self):
		return f"{self.name.capitalize().replace('_', ' ')}"


def get_portnb(fname: str) -> int:
	with open(fname, 'r') as f:
		return int(f.read())


class Game(tk.Frame):
	rules = Rules()

	def __init__(self, size, game_mode: GameMode, master=None): #change game_mode to int
		super().__init__(master)
		self.pack()
		self.gamestate = Gamestate()
		self.buttons = []
		self.frm_position = []
		self.size = size
		self.player = 1
		self.minimax = Minimax()
		self.frm_board = None
		self.white = tk.PhotoImage(file = 'assets/white.png')
		self.black = tk.PhotoImage(file = 'assets/black.png')
		self.gray = tk.PhotoImage(file = 'assets/gray.png')
		self.bot_socket = BotSocket(get_portnb('algo/portnb.txt'))
		self.red = tk.PhotoImage(file = "assets/red.png")
		self.game_mode = game_mode

	def print_board(self):
		print(self.gamestate.board.get_board())

	def create_game_window(self):
		lbl_name = ttk.Label(self, text = f"Go Go Gomoku\nGame Mode: {self.game_mode}")
		lbl_name.pack()
		self.frm_board = ttk.Frame(self, relief = tk.RAISED, borderwidth = 10, height = 500, width = 500)
		self.frm_board.pack(padx = 25, pady = 25)

	def create_options_window(self):
		frm_options = ttk.Frame(self, relief = tk.RIDGE, borderwidth = 10).pack(padx = 25, pady = 25)
		self.display_captures(frm_options)
		self.new_game_bt(frm_options)
		self.choose_different_game_om(frm_options)

	def create_game(self):
		self.create_game_window()
		self.create_options_window()
		self.display_board()
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
				self.frm_position[row * self.size + col].grid(row = row, column = col, padx = 1, pady = 1)
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
		self.buttons[row * self.size + col].config(image = button_img)
		self.print_board()
		self.update()

	def change_player(self) -> None:
		if self.player == 1:
			self.player = 2
		else:
			self.player = 1

	def play_game(self, row: int, col: int) -> None:
		if self.gamestate.board.get(row, col) == 0:
			if not Game.rules.is_legal_move(row, col, self.player, self.gamestate.board):
				print("Illegal move")
				return
			self.gamestate.place_stone(y = row, x = col, stone = self.player)
			self.update_button(row, col)
		else:
			print("Position taken")
			return
		self.after_move_check(row, col)
		self.change_player()
		self.select_next_move()

	def after_move_check(self, row, col):
		""" Check for captures and wins"""
		self.handle_captures(row, col)
		if Game.rules.is_winning_condition(row, col, self.player, self.gamestate.board, self.gamestate.captures):
			if congratulate_winner(self.player):
				self.reset_board()
				return

	def select_next_move(self):
		self.gamestate.moves.clear()
		if self.game_mode == GameMode.HOTSEAT:
			self.play_hotseat()
		elif self.game_mode == GameMode.BOT_POT:
			self.play_bot_pot()
		else:
			self.play_vs_ai()

	def play_hotseat(self):
		value, state = self.minimax.minimax(state = self.gamestate, depth = self.minimax.maxdepth,
											maximizing_player = bool(self.player == 1))
		col, row = state.moves[0].x, state.moves[0].y

	def play_vs_ai(self):
		time_start = time.time()
		value, state = self.minimax.alphabeta(state = self.gamestate, depth = 2, α = -np.inf, β = np.inf,
											  maximizing_player = False)
		col, row = state.moves[0].x, state.moves[0].y
		print(f'In {time.time() - time_start:.2f}s the AI decided to move to y,x={row, col}, heur={state.h}')
		print(f'moves: {state.moves}')
		if self.gamestate.board.get(y = row, x = col) == 0:
			# Handle rules here -> not Game.rules.is_legal_move
			self.gamestate.place_stone(y = row, x = col, stone = self.player)
			self.update_button(row, col)
			self.after_move_check(row, col)
		else:
			raise ValueError()
		self.change_player()

	def play_bot_pot(self):
		print("Playing bot pot")

	def reset_pieces(self):
		button_img = self.gray
		for row in range(self.size):
			for col in range(self.size):
				self.buttons[row * self.size + col].config(image = button_img)

	def reset_board(self):
		self.player = 1
		self.gamestate = Gamestate()
		self.reset_pieces()
		self.update_captures()

	def new_game_bt(self, frm_options):
		bt_new_game = ttk.Button(
			master = frm_options,
			text = "NEW GAME",
			command = self.reset_board,
			width = 15,
			# height=3,
			# bg="gray",
			# fg="red"
		)
		bt_new_game.pack()

	def change_game_mode(self, choice):
		# 1. Pick game_mode
		# 2. change game_mode
		# 3. reset board
		print("executed: " + choice)
		pass

	def choose_different_game_om(self, frm_options):
		lbl_choose_game_mode = ttk.Label(frm_options, text = f"Change Game Mode").pack()
		options = {'Versus ai', 'Hotseat', 'Bot pot'}
		clicked = tk.StringVar()
		clicked.set('Versus ai')
		om_choose_game = ttk.OptionMenu(
			frm_options,
			clicked,
			*options,
			# master=self,
			# variable=clicked,
			# value='Versus ai',
			# # values=options,
			command = self.change_game_mode,
			# width=25,
			# height=5,
			# bg="gray"
			# fg="red"
		)
		# om_choose_game.config("")
		om_choose_game.pack()

	def handle_captures(self, row, col):
		capture_check = Game.rules.is_capturing(row, col, self.player, self.gamestate.board)
		if capture_check is not None:
			for i in range(int(len(capture_check) // 2)):
				i = i * 2
				self.gamestate.capture(capture_check[i], capture_check[i + 1], self.player)
				self.remove_captured(capture_check[i], capture_check[i + 1])
				self.update_captures()
				self.update()

	def remove_captured(self, pos1: tuple, pos2: tuple):
		pos1_row, pos1_col = pos1
		pos2_row, pos2_col = pos2
		self.buttons[pos1_row * self.size + pos1_col].config(image = self.gray)
		self.buttons[pos2_row * self.size + pos2_col].config(image = self.gray)

	def update_captures(self):
		text = '''
		Player {} has {} captures
		Player {} has {} captures
		'''.format(1, self.gamestate.captures[0], 2, self.gamestate.captures[1])
		self.lbl_captures1.configure(text = text)
		self.update()

	def display_captures(self, frm_options):
		text = '''
		Player {} has {} captures
		Player {} has {} captures
		'''.format(1, self.gamestate.captures[0], 2, self.gamestate.captures[1])
		self.lbl_captures1 = ttk.Label(frm_options,
									   text = text)
		self.lbl_captures1.pack(side = 'left')

	def update_board(self, row, col):
		print(f'updating board, row={row}, h={col}')
