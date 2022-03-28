import argparse
import cProfile
import pstats
from game_interface import Game, GameMode


def select_mode(args: argparse.Namespace) -> None:
	if args.hotseat:
		return GameMode.HOTSEAT
	elif args.bot_pot:
		return GameMode.BOT_POT
	else:
		return GameMode.VERSUS_AI

def main(args: argparse.Namespace) -> None:
	if args.cprofile:
		pr = cProfile.Profile()
		pr.enable()
	board = Game(size = 19, game_mode = select_mode(args))
	board.print_board()
	board.create_game()
	if args.cprofile:
		pr.disable()
		stats = pstats.Stats(pr)
		stats.sort_stats('tottime').print_stats(10)


def parse_args() -> argparse.Namespace:
	parser = argparse.ArgumentParser()
	parser.add_argument('--hotseat', action = 'store_true', default = False)
	parser.add_argument('--bot_pot', action = 'store_true', default = False)
	parser.add_argument('--cprofile', action = 'store_true', default = False)

	args = parser.parse_args()
	return args


if __name__ == "__main__":
	main(parse_args())
