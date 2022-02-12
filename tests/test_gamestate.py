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
		h = gamestate.update_h(pos = (8, 0))
		print(f'h={h}')
		assert h == og_h - 20 + 10000
