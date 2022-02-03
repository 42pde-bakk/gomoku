from srcs.gamestate import Gamestate
from srcs.minimax import Minimax


def start_gomoku():
	minimax = Minimax(maxdepth = 2)
	root_node = Gamestate()
	print(root_node)
	print(len(root_node.children))
	# print(root_node.children[0])
	# root_node.capture_check(5, 4, 1)
	a = minimax.alphabeta(gamestate = root_node, depth = minimax.maxdepth, maximizing_player = False)
	print(a)


if __name__ == '__main__':
	start_gomoku()
