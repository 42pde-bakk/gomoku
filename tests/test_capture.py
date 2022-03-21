from unittest import TestCase
from srcs.gamestate import Gamestate


class TestCapture(TestCase):
	def test_capture_pair(self):
		gamestate = Gamestate()
		gamestate.place_stone(1, 1, 0)
		gamestate.place_stone(0, 0, 1)
		gamestate.place_stone(2, 2, 0)
		gamestate.place_stone(3, 3, 1)
		assert gamestate.captures == [0, 2]

	def test_capture_pair2(self):
		gamestate = Gamestate()
		gamestate.place_stone(1, 1, 0)
		gamestate.place_stone(0, 0, 1)
		gamestate.place_stone(1, 2, 0)
		gamestate.place_stone(1, 3, 1)
		assert all(caps == 0 for caps in gamestate.captures)

	def test_edges(self):
		gamestate = Gamestate()
		gamestate.place_stone(16, 16, 0)
		gamestate.place_stone(15, 15, 1)
		gamestate.place_stone(17, 17, 0)
		gamestate.place_stone(18, 18, 1)
		assert gamestate.captures == list([0, 2])
