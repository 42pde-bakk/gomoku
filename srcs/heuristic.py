import numpy as np


def get_connects_of_player(arr: np.ndarray, player: int):
	# Only checks to the right, downwards and down-right
	connections = {i: 0 for i in range(1, 5)}
	rightchecked = set()
	downchecked = set()
	downrightchecked = set()
	for idx, item in np.ndenumerate(arr):
		y, x = idx
		if item != player:
			continue
		x2 = x + 1
		if (y, x2) not in rightchecked:
			while x2 < arr.shape[0] and arr[y][x2] == player:  # check to the right
				rightchecked.add((y, x2))
				x2 += 1
			connections[x2 - x] += 1

		y2 = y + 1
		if (y2, x) not in downchecked:
			while y2 < arr.shape[1] and arr[y2][x] == player:  # check downwards
				downchecked.add((y2, x))
				y2 += 1
			connections[y2 - y] += 1

		y2, x2 = y + 1, x + 1
		if (y2, x2) not in downrightchecked:
			while x2 < arr.shape[0] and arr[y2][x2] == player:  # check diagonally down and right
				downrightchecked.add((y2, x2))
				y2 += 1
				x2 += 1
			connections[y2 - y] += 1
	return connections[2] * 10 + connections[3] * 100 + connections[4] * 1000
