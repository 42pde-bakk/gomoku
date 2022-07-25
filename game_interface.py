import time
import enum
import numpy as np
import tkinter as tk
from tkinter import ttk
from srcs.gamestate import Gamestate, Stone, Move
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


class MoveSnapshot(Move):
	def __init__(self, row: int, col: int, player: int):
		super().__init__(row, col, player)
		self.capturing = False
		self.capture_indices = []


def get_portnb(fname: str) -> int:
	with open(fname, 'r') as f:
		return int(f.read())


class Game(tk.Frame):
	rules = Rules()

	def __init__(self, size, game_mode: GameMode, second_bot_path=None, master=None): #change game_mode to int
		super().__init__(master)
		self.pack()
		self.gamestate = Gamestate()
		self.buttons = []
		self.frm_position = []
		self.ordered_moves = []
		self.size = size
		self.player = 1
		self.previous_player = 0
		self.minimax = Minimax()
		self.game_mode = game_mode
		self.frm_board = None
		self.hotseat_move = [0, 0]
		self.white = tk.PhotoImage(file='assets/white.png')
		self.black = tk.PhotoImage(file='assets/black.png')
		self.gray = tk.PhotoImage(file='assets/gray.png')
		self.red = tk.PhotoImage(file="assets/red.png")
		self.bot_socket = BotSocket(get_portnb('algo/portnb.txt'))
		self.bot_socket_2 = BotSocket(get_portnb(f'{second_bot_path}/algo/portnb.txt')) if second_bot_path else None

	def print_board(self):
		print(self.gamestate.board.get_board())

	def create_game_window(self):
		lbl_name = ttk.Label(self, text=f"Go Go Gomoku\nGame Mode: {self.game_mode}")
		lbl_name.pack()
		self.frm_board = ttk.Frame(self, relief=tk.RAISED, borderwidth=10, height=500, width=500)
		self.frm_board.pack(padx=25, pady=25)

	def create_options_window(self):
		frm_options = ttk.Frame(self, relief=tk.RIDGE, borderwidth=10).pack(padx=25, pady=25)
		self.display_captures(frm_options)
		self.undo_move_bt(frm_options)
		self.new_game_bt(frm_options)
		self.choose_different_game_om(frm_options)

	def create_game(self):
		self.create_game_window()
		self.create_options_window()
		self.display_board()
		self.mainloop()

	def display_board(self) -> None:
		for row in range(self.size):
			for col in range(self.size):
				self.frm_position.append(ttk.Frame(
					master=self.frm_board
					, relief=tk.RIDGE
					, borderwidth=1
				))
				button_img = self.pick_color(row, col)
				self.frm_position[row * self.size + col].grid(row=row, column=col, padx=1, pady=1)
				self.buttons.append(tk.Button(
					master=self.frm_position[row * self.size + col],
					image=button_img,
					command=lambda row=row, column=col: self.handle_click((row, column)),
					height=26,
					width=26
				))
				self.buttons[row * self.size + col].pack()

	def handle_click(self, args) -> None:
		row, col = args
		print(f"Clicked on row: {row}, col: {col}")
		if self.game_mode == GameMode.VERSUS_AI:
			if self.player == 2:
				print(f"Not your turn. Current player: {self.player}")
				return
		if self.game_mode == GameMode.HOTSEAT:
			if self.player == self.previous_player:
				return
		self.previous_player = self.player
		self.play_game(row, col)

	def pick_color(self, row: int, col: int):
		if self.gamestate.board.get(row, col) == 0:
			button_img = self.gray
		elif self.gamestate.board.get(row, col) == 2:
			button_img = self.white
		else:
			button_img = self.black
		return button_img

	def update_button(self, row: int, col: int) -> None:
		print(f"placing a new button at row: {row}, col: {col}")
		button_img = self.pick_color(row, col)
		self.buttons[row * self.size + col].config(image=button_img)
		self.update()

	def change_player(self) -> None:
		if self.player == 1:
			self.player = 2
		else:
			self.player = 1
		self.print_board()

	def play_game(self, row: int, col: int) -> None:
		if self.gamestate.board.get(row, col) == 0:
			if not Game.rules.is_legal_move(row, col, self.player, self.gamestate.board):
				print("Illegal move")
				self.previous_player = Gamestate.get_other_player(self.player)
				return
			self.ordered_moves.append(MoveSnapshot(row, col, self.player))
			self.gamestate.place_stone(y=row, x=col, stone=self.player)
			self.update_button(row, col)
		else:
			print("Position taken")
			self.previous_player = Gamestate.get_other_player(self.player)
			return
		if not self.after_move_check(row, col):
			self.change_player()
			self.select_next_move()

	def after_move_check(self, row: int, col: int) -> bool:
		""" Check for captures and wins"""
		if self.game_mode == GameMode.HOTSEAT:
			self.update_button(self.hotseat_move[0], self.hotseat_move[1])
		self.handle_captures(row, col)
		if Game.rules.is_winning_condition(row, col, self.player, self.gamestate.board, self.gamestate.captures):
			if congratulate_winner(self.player):
				self.reset_board()
				return True
		return False

	def get_ai_move(self) -> Move:
		if self.game_mode == GameMode.BOT_POT and self.bot_socket_2 is not None:
			if self.player == 2:
				self.bot_socket_2.send_gamestate(self.gamestate)
				return self.bot_socket_2.receive_move()
			else:
				self.bot_socket.send_gamestate(self.gamestate)
				return self.bot_socket.receive_move()

		self.bot_socket.send_gamestate(self.gamestate)
		return self.bot_socket.receive_move()

	def select_next_move(self):
		self.gamestate.moves.clear()
		if self.game_mode == GameMode.HOTSEAT:
			self.play_hotseat()
		elif self.game_mode == GameMode.BOT_POT:
			self.play_bot_pot()
		else:
			self.play_vs_ai()

	def play_hotseat(self):
		move = self.get_ai_move()
		col, row = move.x, move.y
		self.hotseat_move[0], self.hotseat_move[1] = row, col
		print(f"Suggested row: {self.hotseat_move[0]}, col: {self.hotseat_move[1]}")
		self.display_suggested_move(row, col)

	def display_suggested_move(self, row: int, col: int) -> None:
		self.buttons[row * self.size + col].config(image=self.red)
		self.update()

	def play_vs_ai(self):
		time_start = time.time()
		move = self.get_ai_move()
		col, row = move.x, move.y
		print(f'In {time.time() - time_start:.2f}s the AI decided to move to y,x={row, col}')
		if self.gamestate.board.get(y=row, x=col) == 0:
			# Handle rules here -> not Game.rules.is_legal_move
			self.ordered_moves.append(MoveSnapshot(row, col, self.player))
			self.gamestate.place_stone(y=row, x=col, stone=self.player)
			self.update_button(row, col)
			if self.after_move_check(row, col):
				return
		else:
			raise ValueError()
		self.change_player()

	def play_bot_pot(self):
		# self.gamestate.place_stone(y=10, x=10, stone=self.player)
		# self.update_button(0, 0)
		# self.change_player()
		while self.game_mode == GameMode.BOT_POT:
			self.play_vs_ai()

	def reset_pieces(self):
		button_img = self.gray
		for row in range(self.size):
			for col in range(self.size):
				self.buttons[row * self.size + col].config(image=button_img)

	def reset_board(self):
		self.player = 1
		self.gamestate = Gamestate()
		self.reset_pieces()
		self.update_captures()
		self.previous_player = 0

	def new_game_bt(self, frm_options):
		bt_new_game = ttk.Button(
			master=frm_options,
			text="NEW GAME",
			command=self.reset_board,
			width=15,
		)
		bt_new_game.pack()

	def change_game_mode(self, choice) -> None:
		if choice == GameMode.BOT_POT.__str__():
			self.game_mode = GameMode.BOT_POT
			self.play_bot_pot()
		elif choice == GameMode.HOTSEAT.__str__():
			self.game_mode = GameMode.HOTSEAT
		else:
			self.game_mode = GameMode.VERSUS_AI
		self.previous_player = 0

	def choose_different_game_om(self, frm_options):
		lbl_choose_game_mode = ttk.Label(frm_options, text=f"Change Game Mode").pack()
		options = ['Versus ai', 'Hotseat', 'Bot pot', 'Versus ai']
		clicked = tk.StringVar()
		# clicked.set(options[0])
		om_choose_game = ttk.OptionMenu(
			frm_options,
			clicked,
			*options,
			command=self.change_game_mode,
		)
		om_choose_game.pack()

	def handle_captures(self, row, col) -> None:
		capture_check = Game.rules.is_capturing(row, col, self.player, self.gamestate.board)
		if capture_check is not None:
			self.ordered_moves[-1].capturing = True
			for i in range(int(len(capture_check) // 2)):
				i = i * 2
				self.gamestate.capture(capture_check[i], capture_check[i + 1], self.player)
				self.remove_captured(capture_check[i], capture_check[i + 1])
				self.update_captures()
				self.update()
				self.ordered_moves[-1].capture_indices.append(capture_check[i])
				self.ordered_moves[-1].capture_indices.append(capture_check[i + 1])

	def remove_captured(self, pos1: tuple, pos2: tuple):
		pos1_row, pos1_col = pos1
		pos2_row, pos2_col = pos2
		self.buttons[pos1_row * self.size + pos1_col].config(image=self.gray)
		self.buttons[pos2_row * self.size + pos2_col].config(image=self.gray)

	def update_captures(self):
		text = '''
		Player {} has {} captures
		Player {} has {} captures
		'''.format(1, self.gamestate.captures[0], 2, self.gamestate.captures[1])
		self.lbl_captures1.configure(text=text)
		self.update()

	def display_captures(self, frm_options):
		text = '''
		Player {} has {} captures
		Player {} has {} captures
		'''.format(1, self.gamestate.captures[0], 2, self.gamestate.captures[1])
		self.lbl_captures1 = ttk.Label(frm_options,
									   text=text)
		self.lbl_captures1.pack(side='left')

	def update_board(self, row, col):
		print(f'updating board, row={row}, h={col}')

	def recreate_captured(self, captured, opponent):
		for stone in captured:
			row, col = stone
			self.gamestate.board.set(row, col, opponent)
			stone_color = self.pick_color(row, col)
			self.buttons[row * self.size + col].config(image=stone_color)
			self.update()

	def undo_move(self):
		print(self.game_mode)
		if self.game_mode == GameMode.BOT_POT:
			return
		if self.ordered_moves:
			last_move = self.ordered_moves.pop()
			row, col = last_move.y, last_move.x
			self.gamestate.board.set(row, col, Stone.EMPTY.value)
			self.buttons[row * self.size + col].config(image=self.gray)
			if last_move.capturing:
				self.recreate_captured(last_move.capture_indices, self.gamestate.get_other_player(last_move.player))
				num_captured = len(last_move.capture_indices)
				self.gamestate.captures[last_move.player - 1] -= num_captured
				self.update_captures()
			if self.game_mode == GameMode.HOTSEAT:
				self.previous_player = self.player
			self.change_player()

	def undo_move_bt(self, frm_options):
		bt_undo_move = ttk.Button(
			master=frm_options,
			text="UNDO MOVE",
			command=self.undo_move,
			width=15,
		)
		bt_undo_move.pack()