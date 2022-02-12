import argparse
import cProfile
import pstats

from game_interface import Game


def main(args: argparse.Namespace) -> None:
	if args.cprofile:
		pr = cProfile.Profile()
		pr.enable()
	board = Game(size = 19, hotseat = args.hotseat)
	board.print_board()
	board.create_window()
	if args.cprofile:
		pr.disable()
		stats = pstats.Stats(pr)
		stats.sort_stats('tottime').print_stats(10)


def parse_args() -> argparse.Namespace:
	parser = argparse.ArgumentParser()
	parser.add_argument('--hotseat', action = 'store_true', default = False)
	parser.add_argument('--cprofile', action = 'store_true', default = False)

	args = parser.parse_args()
	return args


if __name__ == "__main__":
	main(parse_args())
