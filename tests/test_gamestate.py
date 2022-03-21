from unittest import TestCase
from srcs.gamestate import Gamestate


class TestGamestate(TestCase):
	def test_set_h(self):
		gamestate = Gamestate()
		gamestate.board.arr[(6, 0)] = 1
		gamestate.board.arr[(7, 0)] = 1
		gamestate.board.arr[(9, 0)] = 1
		gamestate.board.arr[(10, 0)] = 1
		h = gamestate.set_h()
		assert h == 20

	def test_update_h(self):
		gamestate = Gamestate()
		gamestate.board.arr[(6, 0)] = 1
		gamestate.board.arr[(7, 0)] = 1
		gamestate.board.arr[(9, 0)] = 1
		gamestate.board.arr[(10, 0)] = 1
		og_h = gamestate.set_h()
		gamestate.board.arr[(8, 0)] = 1
		h = gamestate.update_h(pos = (8, 0), player = 0)
		print(f'h={h}')
		assert h == og_h - 20 + 10000

	def test_same(self):
		gamestate = Gamestate()
		for move, item in zip([(7, 8), (9, 10), (8, 9)], [1, 2, 1]):
			gamestate.board.arr[move] = item
			updated_h = gamestate.update_h(pos = move, player = item - 1)
			set_h = gamestate.set_h()
			assert updated_h == set_h

	def test_same_2(self):
		gamestate = Gamestate()
		for move, item in zip([(9, 9), (7, 8), (8, 10), (8, 9)], [1, 2, 1, 2]):
			gamestate.place_stone(y = move[0], x = move[1], player = item - 1)
			updated_h = gamestate.h
			set_h = gamestate.set_h()
			assert updated_h == set_h
