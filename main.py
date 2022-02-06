import argparse
from game_interface import Game


def main(args: argparse.Namespace) -> None:
	board = Game(size = 19, hotseat = args.hotseat)
	board.print_board()
	board.create_window()


def parse_args() -> argparse.Namespace:
	parser = argparse.ArgumentParser()
	parser.add_argument('--hotseat', action = 'store_true', default = False)

	args = parser.parse_args()
	return args


if __name__ == "__main__":
	main(parse_args())
