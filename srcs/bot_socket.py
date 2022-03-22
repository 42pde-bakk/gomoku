import socket
import struct
from srcs.gamestate import Gamestate
import numpy as np


class BotSocket:
	HOST, PORT = "localhost", 4242

	def __init__(self):
		self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.s.connect((BotSocket.HOST, BotSocket.PORT))

	def send_gamestate(self, gs: Gamestate) -> None:
		print(f'gs.turn = {gs.turn}')
		self.s.sendall(struct.pack('i', gs.turn))  # Turn number
		stones = np.argwhere(gs.board.arr != 0)
		print(f'stones_amount = {len(stones)}')
		self.s.sendall(struct.pack('i', len(stones)))  # Amount of stones on the board

		for idx in stones:
			print(f'stone: {idx} => {gs.board.arr[tuple(idx)]}')
			# self.s.sendall(struct.pack('iii', int(idx[0]), int(idx[1]), int(gs.board.arr[tuple(idx)])))
			self.s.sendall(struct.pack('iii', idx[0], idx[1], gs.board.arr[tuple(idx)]))

	def receive_move(self):
		bitches = self.s.recv(4 * 3)  # 3 ints
		print(f'bytes: {bitches}')
		y, x, stone = struct.unpack('iii', bitches)
		print(f'unpacked = {y, x, stone}')
